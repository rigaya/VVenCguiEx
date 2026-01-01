// -----------------------------------------------------------------------------------------
// VVenCguiEx by rigaya
// -----------------------------------------------------------------------------------------
// The MIT License
//
// Copyright (c) 2019 rigaya
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// --------------------------------------------------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <cmath>
#include <limits.h>
#include <vector>
#include <string>
#include <sstream>

#include "h264_level.h"
#include "auo_util.h"
#include "auo_conf.h"
#include "auo_options.h"

#include "shellapi.h"
#pragma comment(lib, "Shell32.lib")


static guiEx_settings *ex_stg = nullptr;

void set_ex_stg_ptr(guiEx_settings *_ex_stg) {
    ex_stg = _ex_stg;
}

int parse_one_option(CONF_ENC *cx, const TCHAR *option_name, const std::vector<tstring>& argv, int &i, int nArgNum) {
#define IS_OPTION(x) (tstring(_T(x)) == option_name)
    auto to_int = [](int *value, const tstring& argv) {
        try {
            *value = std::stoi(argv);
        } catch (...) {
            return 1;
        }
        return 0;
    };
    auto list_to_int = [](int *value, const CX_DESC_AUO *list, const tstring& argv) {
        auto val = get_value_from_chr(list, tchar_to_wstring(argv).c_str());
        if (val == -1) {
            return 1;
        }
        *value = val;
        return 0;
    };
#define OPT_NUM(str, val) \
    if (IS_OPTION(str)) { \
        i++; \
        return to_int(&cx->val, argv[i]); \
    }
#define OPT_LST(str, list, val) \
    if (IS_OPTION(str)) { \
        i++; \
        return list_to_int(&cx->val, list, argv[i]); \
    }
    OPT_LST("RateControl", list_rc, rate_control);
    if (IS_OPTION("TargetBitrate")) {
        i++;
        int value = 0;
        if (_stscanf_s(argv[i].c_str(), _T("%dk"), &value) != 1) {
            return 1;
        }
        cx->bitrate = value;
        if (cx->bitrate == 0) {
            cx->rate_control = get_cx_value(list_rc, L"CQP");
        }
        return 0;
    }
    OPT_NUM("QP", qp);
    OPT_NUM("InputBitDepth", bit_depth);
    OPT_LST("InputChromaFormat", list_color_format, output_csp);
    OPT_LST("preset", list_enc_mode, preset);
    OPT_LST("Profile", list_profile_vvc, profile);
    OPT_LST("Level", list_level_vvc, level);
    OPT_LST("Tier", list_tier_vvc, tier);
    OPT_NUM("OutputBitDepth", bit_depth);
    OPT_NUM("Pass", pass);
    OPT_NUM("PerceptQPA", qpa);

    OPT_NUM("GOPSize", gopsize);
    OPT_LST("DecodingRefreshType", list_refresh_type, refreshtype);
    OPT_NUM("RefreshSec", refreshsec);
    OPT_NUM("IntraPeriod", intraperiod);

    OPT_LST("MatrixCoefficients", list_colormatrix, colormatrix);
    OPT_LST("ColourPrimaries", list_colorprim, colorprim);
    OPT_LST("TransferCharacteristics", list_transfer, transfer);
    OPT_NUM("VideoFullRange", fullrange);
    OPT_NUM("SarWidth", sar_x);
    OPT_NUM("SarHeight", sar_y);

    OPT_LST("LookAhead", list_lookahead, lookahead);
    OPT_NUM("Threads", threads);
    return 1;
#undef OPT_NUM
#undef OPT_LST
#undef IS_OPTION
}

int set_cmd(CONF_ENC *cx, const TCHAR *cmd, const bool ignore_parse_err) {
    auto args = sep_cmd(cmd);
    for (int i = 0; i < args.size(); i++) {
        const TCHAR *option_name = nullptr;
        if (args[i][0] == '|') {
            break;
        } else if (args[i][0] == '-' && args[i][1] == 'q') {
            option_name = &args[i][1];
        } else if (args[i][0] == '-' && args[i][1] == '-') {
            option_name = &args[i][2];
        } else {
            if (ignore_parse_err) continue;
            return 1;
        }
        if (args[i].length() > 0) {
            auto sts = parse_one_option(cx, option_name, args, i, (int)args.size());
            if (!ignore_parse_err && sts != 0) {
                return sts;
            }
        }
    }
    return 0;
}

CONF_ENC get_default_prm() {
    CONF_ENC prm;
    memset(&prm, 0, sizeof(prm));
    set_cmd(&prm, ex_stg->s_enc.default_cmd, true);
    return prm;
}

int parse_cmd(CONF_ENC *cx, const TCHAR *cmd, const bool ignore_parse_err) {
    *cx = get_default_prm();
    return set_cmd(cx, cmd, ignore_parse_err);
}

tstring gen_cmd(const CONF_ENC *cx, bool save_disabled_prm) {
    std::basic_stringstream<TCHAR> cmd;
    CONF_ENC encPrmDefault = get_default_prm();
#define OPT_NUM(str, opt) if ((cx->opt) != (encPrmDefault.opt)) cmd << _T(" --") << _T(str) << _T(" ") << (int)(cx->opt);
#define OPT_LST(str, list, opt) if ((cx->opt) != (encPrmDefault.opt)) { \
    auto ptr = get_cx_desc(list, cx->opt); \
    if (ptr) cmd << _T(" --") << _T(str) << _T(" ") << wstring_to_tstring(ptr); \
}
    if (save_disabled_prm) {
        OPT_LST("RateControl", list_rc, rate_control);
    }
    if (cx->rate_control == get_cx_value(list_rc, L"CQP")) {
        cmd << _T(" --QP ") << (int)(cx->qp);
    } else {
        cmd << _T(" --TargetBitrate ") << (int)(cx->bitrate) << _T("k");
    }
    cmd << _T(" --InputBitDepth ") << (int)(cx->bit_depth);
    cmd << _T(" --InternalBitDepth ") << (int)(cx->bit_depth);
    cmd << _T(" --OutputBitDepth ") << (int)(cx->bit_depth);
    OPT_LST("InputChromaFormat", list_color_format, output_csp);
    OPT_LST("preset", list_enc_mode, preset);
    OPT_LST("Profile", list_profile_vvc, profile);
    OPT_LST("Level", list_level_vvc, level);
    OPT_LST("Tier", list_tier_vvc, tier);
    OPT_NUM("Pass", pass);
    OPT_NUM("PerceptQPA", qpa);

    OPT_NUM("GOPSize", gopsize);
    OPT_LST("DecodingRefreshType", list_refresh_type, refreshtype);
    OPT_NUM("RefreshSec", refreshsec);
    OPT_NUM("IntraPeriod", intraperiod);

    OPT_LST("MatrixCoefficients", list_colormatrix, colormatrix);
    OPT_LST("ColourPrimaries", list_colorprim, colorprim);
    OPT_LST("TransferCharacteristics", list_transfer, transfer);
    OPT_NUM("VideoFullRange", fullrange);
    OPT_NUM("SarWidth", sar_x);
    OPT_NUM("SarHeight", sar_y);

    OPT_LST("LookAhead", list_lookahead, lookahead);
    OPT_NUM("Threads", threads);

    return cmd.str();
}

void set_auto_colormatrix(CONF_ENC *cx, int height) {
    if (cx->colormatrix == COLOR_MATRIX_AUTO)
        cx->colormatrix = (height >= COLOR_MATRIX_THRESHOLD) ? get_cx_value(list_colormatrix, L"bt709") : get_cx_value(list_colormatrix, L"bt601");
    if (cx->colorprim == COLOR_MATRIX_AUTO)
        cx->colorprim = (height >= COLOR_MATRIX_THRESHOLD) ? get_cx_value(list_colorprim, L"bt709") : get_cx_value(list_colormatrix, L"bt601");
    if (cx->transfer == COLOR_MATRIX_AUTO)
        cx->transfer = (height >= COLOR_MATRIX_THRESHOLD) ? get_cx_value(list_transfer, L"bt709") : get_cx_value(list_colormatrix, L"bt601");
}
