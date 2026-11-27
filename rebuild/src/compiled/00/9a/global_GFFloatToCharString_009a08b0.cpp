// 009a08b0 GFFloatToCharString
// Leaf: ?GFFloatToCharString@@YI?AVCCharString@@NJ@Z
// Flags: none
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING_FORMAT_PASS29
// GFFloatToCharString formats a double and removes trailing fractional zeroes.
// Ego_r 00AFBA90, FableWin 02F636C0 and the Ego_d procedure agree on the free fastcall, double n and long precision.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
#define FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
#define FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23
#include "fable_string.h"
#ifdef _MSC_VER
#include <float.h>
#else
#include <math.h>
#endif

CCharString FABLE_FASTCALL GFFloatToCharString(double n, long precision)
{
    CCharString temp;
    bool nip_trailing_zeroes = false;
#ifdef _MSC_VER
    if (_isnan(n))
#else
    if (isnan(n))
#endif
        temp.Sprintf("%.*f", precision, 0.0);
    else
        temp.Sprintf("%.*f", precision, n);

    for (long c1 = 0; c1 < temp.GetLength(); ++c1) {
        if (temp.GetAsCharArray()[c1] == '.') {
            nip_trailing_zeroes = true;
            break;
        }
    }
    if (nip_trailing_zeroes) {
        while (temp.GetLength() > 0) {
            char ch = temp.GetAsCharArray()[temp.GetLength() - 1];
            if (ch == '.') {
                temp += '0';
                break;
            } else if (ch == '0') {
                temp.Truncate(temp.GetLength() - 1);
            } else {
                break;
            }
        }
    }
    return temp;
}