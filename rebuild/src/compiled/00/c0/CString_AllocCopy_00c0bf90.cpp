// CString::AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const -- give dest a new buffer holding nCopyLen characters from nCopyIndex (plus room for nExtraLen)
// Leaf: ?AllocCopy@CString@@QBEXAAV1@HHH@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BF90, 78 bytes, `ret 10h`; the memcpy is the inline intrinsic (rep movsd / rep movsb).
// Identity (two sources): (1) the libpk MFC-derived CString's Mid 0x00C0DF50, Right 0x00C0E0F0 and Left 0x00C0E1F0 (landed) bind it as
//   ?AllocCopy@CString@@QBEXAAV1@HHH@Z; (2) the body is MFC strcore.cpp CString::AllocCopy: nNewLen = nCopyLen + nExtraLen; zero ->
//   dest.Init() (the nil characters 0x0139C900 stored), else dest.AllocBuffer(nNewLen) (0x00C0BDA0) and memcpy(dest.m_pchData,
//   m_pchData + nCopyIndex, nCopyLen).
// Callees: ?AllocBuffer@CString@@QAEXH@Z 0x00C0BDA0; data ?_afxPchNil@@3PADA 0x0139C900 (see engine/CString.h on the spelling).
// Header: engine/CString.h (FABLE_RETAIL_CSTRING_ALLOCCOPY new).
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#include <string.h>
#include "engine/CString.h"
void CString::AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
{
    int nNewLen = nCopyLen + nExtraLen;
    if (nNewLen == 0)
    {
        dest.Init();
    }
    else
    {
        dest.AllocBuffer(nNewLen);
        memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen * sizeof(char));
    }
}