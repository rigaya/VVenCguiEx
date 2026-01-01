// -----------------------------------------------------------------------------------------
// x264guiEx/x265guiEx/svtAV1guiEx/ffmpegOut/QSVEnc/NVEnc/VCEEnc by rigaya
// -----------------------------------------------------------------------------------------
// The MIT License
//
// Copyright (c) 2010-2022 rigaya
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

#ifndef _AUO_OPTIONS_H_
#define _AUO_OPTIONS_H_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <string>
#include <limits>

#include "auo.h"
#include "auo_settings.h"

//エンコードモード
enum {
    ENC_RC_QP = 0,
    ENC_RC_BITRATE,
};

//QPの最大値
const int ENC_QP_MAX_8BIT  = 69;
const int ENC_QP_MAX_10BIT = 81;

//差がこのくらいなら等しいとみなす(オプション用なのでこのくらいで十分)
const float EPS_FLOAT = 1.0e-4f;

#if ENCODER_X265
static const int AUO_KEYINT_MAX_AUTO = 0;
#elif ENCODER_X264 || ENCODER_SVTAV1
static const int AUO_KEYINT_MAX_AUTO = -1;
#endif

//マクロブロックタイプの一般的なオプション
enum {
    MB_PARTITION_NONE = 0x00000000,
    MB_PARTITION_P8x8 = 0x00000001,
    MB_PARTITION_B8x8 = 0x00000002,
    MB_PARTITION_P4x4 = 0x00000004,
    MB_PARTITION_I8x8 = 0x00000008,
    MB_PARTITION_I4x4 = 0x00000010,
    MB_PARTITION_ALL  = 0x0000001F,
};

enum {
    OUT_CSP_YUV400,
    OUT_CSP_YV12,
    OUT_CSP_YUV422,
    OUT_CSP_YUV444,
    OUT_CSP_RGB,
    OUT_CSP_YUY2,
    OUT_CSP_NV12,
    OUT_CSP_NV16,
    OUT_CSP_P010,
    OUT_CSP_YUV444_16,
    OUT_CSP_RGBA,
    OUT_CSP_RGBA_16,
};

enum {
    YC48_COLMAT_CONV_AUTO,
    YC48_COLMAT_CONV_NONE,
    YC48_COLMAT_CONV_BT709,
};

enum {
    AUTO_NPASS_NONE = 0,
    AUTO_NPASS_AMP,
    AUTO_NPASS_CRF,
};

//x264のinput-cspとして使用するもの
//OUT_CSP_NV12, OUT_CSP_YUV444, OUT_CSP_RGB に合わせる
static const TCHAR * const specify_csp[] = {
    _T("yuv400"), //OUT_CSP_YUV400
    _T("yuv420"), //OUT_CSP_YV12
    _T("yuv422"), //OUT_CSP_YUV422
    _T("yuv444"), //OUT_CSP_YUV444
    _T("rgb")   //OUT_CSP_RGB
};
//文字列を引数にとるオプションの引数リスト
//OUT_CSP_NV12, OUT_CSP_YUV444, OUT_CSP_RGB に合わせる
const ENC_OPTION_STR list_output_csp[] = {
    { _T("yuv400"), AUO_MES_UNKNOWN, L"yuv400" },
    { _T("yuv420"), AUO_MES_UNKNOWN, L"yuv420" },
    { _T("yuv422"), AUO_MES_UNKNOWN, L"yuv422" },
    { _T("yuv444"), AUO_MES_UNKNOWN, L"yuv444" },
    { _T("rgb"),    AUO_MES_UNKNOWN, L"rgb"    },
    { nullptr,    AUO_MES_UNKNOWN, 0      }
};

typedef struct CX_DESC_AUO {
    const wchar_t *desc;
    AuoMes mes;
    int value;
} CX_DESC_AUO;

enum {
    COLOR_MATRIX_AUTO = 0,
};
const int COLOR_MATRIX_THRESHOLD = 720;

const CX_DESC_AUO list_color_range[] = {
    { L"auto",  AUO_MES_UNKNOWN, 0 },
    { L"pc",    AUO_MES_UNKNOWN, 1 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_colorprim[] = {
    { L"unknown",   AUO_OPTION_VUI_UNDEF, 2   },
    { L"auto",      AUO_OPTION_VUI_AUTO, COLOR_MATRIX_AUTO   },
    { L"bt709",     AUO_MES_UNKNOWN, 1   },
    //{ L"smpte170m", AUO_MES_UNKNOWN, 3 },
    { L"bt470m",   AUO_MES_UNKNOWN, 4   },
    { L"bt470bg",  AUO_MES_UNKNOWN, 5   },
    { L"bt601",    AUO_MES_UNKNOWN, 6   },
    { L"smpte240", AUO_MES_UNKNOWN, 7   },
    { L"film",     AUO_MES_UNKNOWN, 8   },
    { L"bt2020",   AUO_MES_UNKNOWN, 9   },
    { L"xyz",      AUO_MES_UNKNOWN, 10  },
    { L"smpte431", AUO_MES_UNKNOWN, 11  },
    { L"smpte432", AUO_MES_UNKNOWN, 12  },
    { L"ebu3213",  AUO_MES_UNKNOWN, 22  },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_transfer[] = {
    { L"unknown",       AUO_OPTION_VUI_UNDEF, 2  },
    { L"auto",          AUO_OPTION_VUI_AUTO, COLOR_MATRIX_AUTO  },
    { L"bt709",         AUO_MES_UNKNOWN, 1  },
    //{ L"smpte170m",     AUO_MES_UNKNOWN, 3    },
    { L"bt470m",        AUO_MES_UNKNOWN,  4 },
    { L"bt470bg",       AUO_MES_UNKNOWN,  5 },
    { L"bt601",         AUO_MES_UNKNOWN,  6 },
    { L"smpte240",      AUO_MES_UNKNOWN,  7 },
    { L"linear",        AUO_MES_UNKNOWN,  8 },
    { L"log100",        AUO_MES_UNKNOWN,  9 },
    { L"log100-sqrt10", AUO_MES_UNKNOWN, 10 },
    { L"iec61966",      AUO_MES_UNKNOWN, 11 },
    { L"bt1361",        AUO_MES_UNKNOWN, 12 },
    { L"srgb",          AUO_MES_UNKNOWN, 13 },
    { L"bt2020-10",     AUO_MES_UNKNOWN, 14 },
    { L"bt2020-12",     AUO_MES_UNKNOWN, 15 },
    { L"smpte2084",     AUO_MES_UNKNOWN, 16 },
    { L"smpte428",      AUO_MES_UNKNOWN, 17 },
    { L"hlg",           AUO_MES_UNKNOWN, 18 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_colormatrix[] = {
    { L"unknown",     AUO_OPTION_VUI_UNDEF, 2  },
    { L"auto",        AUO_OPTION_VUI_AUTO, COLOR_MATRIX_AUTO  },
    { L"bt709",       AUO_MES_UNKNOWN, 1  },
    { L"identity",    AUO_MES_UNKNOWN, 0  },
    //{ L"smpte170m",   AUO_MES_UNKNOWN,  3 },
    { L"fcc",         AUO_MES_UNKNOWN,  4 },
    { L"bt470bg",     AUO_MES_UNKNOWN,  5 },
    { L"bt601",       AUO_MES_UNKNOWN,  6 },
    { L"smpte240",    AUO_MES_UNKNOWN,  7 },
    { L"ycgco",       AUO_MES_UNKNOWN,  8 },
    { L"bt2020-ncl",  AUO_MES_UNKNOWN,  9 },
    { L"bt2020-cl",   AUO_MES_UNKNOWN, 10 },
    { L"smpte2085",   AUO_MES_UNKNOWN, 11 },
    { L"chroma-ncl",  AUO_MES_UNKNOWN, 12 },
    { L"chroma-cl",   AUO_MES_UNKNOWN, 13 },
    { L"ictcp",       AUO_MES_UNKNOWN, 14 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const ENC_OPTION_STR list_videoformat[] = {
    { _T("undef"),     AUO_OPTION_VUI_UNDEF, L"指定なし"  },
    { _T("ntsc"),      AUO_MES_UNKNOWN,      L"ntsc"      },
    { _T("component"), AUO_MES_UNKNOWN,      L"component" },
    { _T("pal"),       AUO_MES_UNKNOWN,      L"pal"       },
    { _T("secam"),     AUO_MES_UNKNOWN,      L"secam"     },
    { _T("mac"),       AUO_MES_UNKNOWN,      L"mac"       },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const ENC_OPTION_STR list_log_type[] = {
    { _T("none"),    AUO_MES_UNKNOWN, L"none"    },
    { _T("error"),   AUO_MES_UNKNOWN, L"error"   },
    { _T("warning"), AUO_MES_UNKNOWN, L"warning" },
    { _T("info"),    AUO_MES_UNKNOWN, L"info"    },
    { _T("debug"),   AUO_MES_UNKNOWN, L"debug"   },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};

#pragma pack(push,1)
typedef struct {
    int x;
    int y;
} INT2;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
    float x;
    float y;
} FLOAT2;
#pragma pack(pop)




//x264パラメータ構造体
//guiExで使いやすいよう、適当に
//並び順に意味はない
#pragma pack(push,1)
typedef struct {
    int     preset;            //enc-mode
    int     rate_control;      //rate-control
    int     bit_depth;         //bit-depth
    int     output_csp;
    int     profile;
    int     level;
    int     tier;
    int     pass;
    int     qp;                  //qp
    int     qpa;                 //qpa
    int     bitrate;             //bitrate
    int     gopsize;
    int     lookahead;

    int     colormatrix;
    int     colorprim;
    int     transfer;
    int     fullrange;
    int     sar_x;
    int     sar_y;

    int     refreshtype;
    int     refreshsec;
    int     intraperiod;

    int     threads;
} CONF_ENC;
#pragma pack(pop)

static const wchar_t *get_chr_from_value(const CX_DESC_AUO *list, int v) {
    for (int i = 0; list[i].desc; i++)
        if (list[i].value == v)
            return list[i].desc;
    return L"unknown";
}

static int get_cx_index(const CX_DESC_AUO *list, int v) {
    for (int i = 0; list[i].desc; i++)
        if (list[i].value == v)
            return i;
    return 0;
}

static int get_cx_index(const CX_DESC_AUO *list, const wchar_t *chr) {
    for (int i = 0; list[i].desc; i++)
        if (0 == wcsicmp(list[i].desc, chr))
            return i;
    return 0;
}

static int get_cx_value(const CX_DESC_AUO *list, const wchar_t *chr) {
    for (int i = 0; list[i].desc; i++)
        if (0 == wcsicmp(list[i].desc, chr))
            return list[i].value;
    return 0;
}

static int get_value_from_chr(const CX_DESC_AUO *list, const wchar_t *chr) {
    for (int i = 0; list[i].desc; i++)
        if (wcsicmp(list[i].desc, chr) == 0)
            return list[i].value;
    return -1;
}

static const wchar_t *get_cx_desc(const CX_DESC_AUO *list, int v) {
    for (int i = 0; list[i].desc; i++)
        if (list[i].value == v)
            return list[i].desc;
    return nullptr;
}


const CX_DESC_AUO list_enc_mode[] = {
    { L"faster", AUO_OPTION_ENC_MODE_FASTER, 0 },
    { L"fast",   AUO_OPTION_ENC_MODE_FAST,   1 },
    { L"medium", AUO_OPTION_ENC_MODE_MEDIUM, 2 },
    { L"slow",   AUO_OPTION_ENC_MODE_SLOW,   3 },
    { L"slower", AUO_OPTION_ENC_MODE_SLOWER, 4 },
    { nullptr,  AUO_MES_UNKNOWN,  0 }
};

const CX_DESC_AUO list_rc[] = {
    { L"CQP", AUO_OPTION_RC_CQP,  0 },
    { L"VBR", AUO_OPTION_RC_VBR,  1 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};

const CX_DESC_AUO list_on_off_default[] = {
    { L"-1: default", AUO_MES_UNKNOWN, -1 },
    { L"0: off", AUO_MES_UNKNOWN, 0 },
    { L"1: on", AUO_MES_UNKNOWN, 1 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_color_format[] = {
    { L"420",    AUO_MES_UNKNOWN, OUT_CSP_YV12 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_profile_vvc[] = {
    { L"auto",            AUO_MES_UNKNOWN, 0 },
    { L"main10",          AUO_MES_UNKNOWN, 1 },
    { L"main10_stillpic", AUO_MES_UNKNOWN, 2 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_tier_vvc[] = {
    { L"main", AUO_MES_UNKNOWN, 0 },
    { L"high", AUO_MES_UNKNOWN, 1 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_level_vvc[] = {
    { L"auto", AUO_MES_UNKNOWN,  0 },
    { L"1.0",  AUO_MES_UNKNOWN, 10 },
    { L"2.0",  AUO_MES_UNKNOWN, 20 },
    { L"2.1",  AUO_MES_UNKNOWN, 21 },
    { L"3.0",  AUO_MES_UNKNOWN, 30 },
    { L"3.1",  AUO_MES_UNKNOWN, 31 },
    { L"4.0",  AUO_MES_UNKNOWN, 40 },
    { L"4.1",  AUO_MES_UNKNOWN, 41 },
    { L"5.0",  AUO_MES_UNKNOWN, 50 },
    { L"5.1",  AUO_MES_UNKNOWN, 51 },
    { L"5.2",  AUO_MES_UNKNOWN, 52 },
    { L"6.0",  AUO_MES_UNKNOWN, 60 },
    { L"6.1",  AUO_MES_UNKNOWN, 61 },
    { L"6.2",  AUO_MES_UNKNOWN, 62 },
    { L"6.3",  AUO_MES_UNKNOWN, 63 },
    { nullptr, AUO_MES_UNKNOWN, 0 }
};
const CX_DESC_AUO list_refresh_type[] = {
    { L"idr",     AUO_OPTION_REFRESH_TYPE_IDR,     0 },
    { L"cra",     AUO_OPTION_REFRESH_TYPE_CRA,     1 },
    { L"idr2",    AUO_OPTION_REFRESH_TYPE_IDR2,    2 },
    { L"cra_cre", AUO_OPTION_REFRESH_TYPE_CRA_CRE, 3 },
    { nullptr,    AUO_MES_UNKNOWN,  0 }
};
const CX_DESC_AUO list_lookahead[] = {
    { L"-1",   AUO_MES_UNKNOWN, -1 },
    { L"0",    AUO_MES_UNKNOWN,  0 },
    { L"1",    AUO_MES_UNKNOWN,  1 },
    { nullptr, AUO_MES_UNKNOWN,  0 }
};


typedef struct {
    TCHAR *long_name;
    TCHAR *short_name;
    DWORD type;
    const ENC_OPTION_STR *list;
    size_t p_offset;
} X264_OPTIONS;

enum {
    ARG_TYPE_UNKNOWN = 0,
    ARG_TYPE_LONG    = 1,
    ARG_TYPE_SHORT   = 2,
};

typedef struct CMD_ARG {
    int arg_type;       //LONGかSHORTか
    TCHAR *option_name;  //オプション名(最初の"--"なし)
    TCHAR *value;        //オプションの値
    BOOL value_had_dQB; //前後に'"'があったか
    BOOL ret;           //構造体に読み込まれたかどうか
    BOOL type_mediainfo; //MediaInfoの書式だったかどうか
} CMD_ARG;

bool ishighbitdepth(const CONF_ENC *enc);

void set_guiEx_auto_sar(int *sar_x, int *sar_y, int width, int height);

//コマンドラインの解析・生成
int set_cmd(CONF_ENC *cx, const TCHAR *cmd, const bool ignore_parse_err);
int parse_cmd(CONF_ENC *cx, const TCHAR *cmd, const bool ignore_parse_err);
tstring gen_cmd(const CONF_ENC *cx, bool save_disabled_prm);
CONF_ENC get_default_prm();
void set_ex_stg_ptr(guiEx_settings *_ex_stg);
void set_auto_colormatrix(CONF_ENC *cx, int height);

#endif //_AUO_OPTIONS_H_
