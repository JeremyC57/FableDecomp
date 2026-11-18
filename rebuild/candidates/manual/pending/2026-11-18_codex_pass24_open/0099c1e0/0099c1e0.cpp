// CWideString::Replace -- Replace nonoverlapping wide-string occurrences in a copy-on-write string.
// Grade: DIFFER(613v609) -- The current prefix lifetime leaves a larger frame and different ownership-counter and destructor scheduling. The public underlying append specialization and reference-count ownership are present.
// Leaf: ?Replace@CWideString@@QAEXABV1@0@Z
// Retail: 0x0099C1E0, 609 bytes, thiscall.
#define FABLE_RETAIL_CWIDESTRING_REPLACE_PASS24
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY
#define FABLE_RETAIL_CWIDESTRING_SPRINTF_ORIGINAL
#define FABLE_RETAIL_STLP_CHAR_TRAITS_LENGTH
#include "fable_string.h"
#include <wchar.h>
#ifdef _MSC_VER
#pragma function(wcslen)
#endif

CWideString::CWideString() : PStringData(0) { ++NoStrings; }
CWideString::~CWideString() { UnassignString(); --NoStrings; }

const CWideString& CWideString::operator=(const CWideString& other)
{
    if (this != &other && PStringData != other.PStringData) {
        if (PStringData) UnassignString();
        if (other.PStringData) {
            PStringData = other.PStringData;
            ++PStringData->NoRefs;
        }
    }
    return *this;
}

const CWideString& CWideString::operator+=(const CWideString& other)
{
    if (PStringData) {
        MakeOwnCopyOfData();
        PStringData->Append(other.GetAsCharArray(), -1);
    } else if (&other != this && other.PStringData) {
        PStringData = other.PStringData;
        ++PStringData->NoRefs;
    }
    return *this;
}

void CWideString::Replace(const CWideString& What, const CWideString& With)
{
    CWideString ret_str;
    wchar_t* data = const_cast<wchar_t*>(GetAsCharArray());
    wchar_t* what = const_cast<wchar_t*>(What.GetAsCharArray());
    wchar_t* prev = data;
    wchar_t* res = wcsstr(data,what);
    while (res) {
        { CWideString temp;
        temp.SetAsSubString(prev, res - prev);
        res += wcslen(what);
        ret_str += temp; }
        ret_str += With;
        prev = res;
        res = wcsstr(res,what);
    }
    ret_str += CWideString(prev);
    *this = ret_str;
}
