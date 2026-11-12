// CString::MakeUpper() -- MFC strcore.cpp: unshare the buffer (CopyBeforeWrite) and upper-case the characters in place with _strupr
// Leaf: ?MakeUpper@CString@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0C910, 22 bytes: push esi ; mov esi,ecx ; call 0x00C0BE20 ; mov eax,[esi] ; push eax ;
// call [0x0144002C] ; add esp,4 ; pop esi ; ret.
// The libpk MFC-derived CString (engine/CString.h: m_pchData at +0, CStringData {nRefs, nDataLength, nAllocLength} in front).
// Callees: CString::CopyBeforeWrite @ 0x00C0BE20 (unshares when nRefs > 1; ?CopyBeforeWrite@CString@@QAEXXZ as row 0x00C0C630
// binds it) and MSVCR71 _strupr (IAT 0x0144002C). Caller 0x00C2E610 (CBankProcessor::AddSampleNameTriggers) already binds
// ?MakeUpper@CString@@QAEXXZ. The catalog label (std::list<CMessageEvent const*>::~list, "DtorMsgEventList_A") misread the
// _strupr import as free. Name: MFC's public CString source (vendor-code naming rule); libpk has no PDB.
#define FABLE_RETAIL_CSTRING_MAKEUPPER
#include "engine/CString.h"

extern "C" __declspec(dllimport) char* __cdecl _strupr(char* text);   // IAT 0x0144002C

void CString::MakeUpper()
{
    CopyBeforeWrite();
    _strupr(m_pchData);
}
