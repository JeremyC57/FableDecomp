// operator+(const CCharString& string, char ch) -- return a copy of the string with one character appended
// Grade: DIFFER(126v130) -- The shared copy operator leaves a four-byte deficit in the inline reference-count increment at 0x0099F7BF.
// Leaf: ??H@YI?AVCCharString@@ABV0@D@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F7A0, 130 bytes, free __fastcall (hidden result pointer in ecx, string in edx, ch on the
//   stack), `ret 4`.
// Identity: Ego_d.pdb lib_char_string.obj operator+ (params `const CCharString& string`, `char ch`; local `CCharString result_string`;
//   debug length 124); Ego_r.pdb / FableWin.pdb ??H@YI?AVCCharString@@ABV0@D@Z, the last of the five CCharString operator+ overloads
//   after RemoveWhiteSpaceBeginEnd (Ego_r donor 0x00AFB020) -- retail's neighbours are RemoveWhiteSpaceBeginEnd 0x0099F560 and
//   GFIntToCharString 0x0099F830. The ecx / edx / stack argument use fits only this overload.
// Callees: CCharString::operator+=(char) 0x0099F180, CCharString::UnassignString 0x0099E9B0; CCharString::NoStrings 0x013BD800.
//   The copy constructor (out-of-line copy 0x0099EC30) and the destructor (0x0099EAE0) are this TU's own bodies, expanded inline.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
#define FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
#include "fable_string.h"

CCharString::CCharString()
    : PStringData(0)
{
    ++NoStrings;
}

CCharString::CCharString(const CCharString& string)
    : PStringData(0)
{
    ++NoStrings;
    if (&string != this)
    {
        if (PStringData)
            UnassignString();
        if (string.PStringData)
        {
            PStringData = string.PStringData;
            ++PStringData->NoRefs;
        }
    }
}

CCharString::~CCharString()
{
    UnassignString();
    --NoStrings;
}

const CCharString& CCharString::operator=(const CCharString& string)
{
    if (&string != this)
    {
        if (PStringData)
            UnassignString();
        if (string.PStringData)
        {
            PStringData = string.PStringData;
            PStringData->NoRefs += 1;
        }
    }
    return *this;
}

CCharString FABLE_FASTCALL operator+(const CCharString& string, char ch)
{
    CCharString result_string;
    result_string = string;
    result_string += ch;
    return result_string;
}
