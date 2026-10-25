// GetCandidateList(HIMC himc, unsigned long dwIndex, CANDIDATELIST** ppCandList) -- DXUT ImeUi.cpp (Unicode IMM): size the candidate list, allocate it through ImeUiCallback_Malloc and fill it; returns its size
// Leaf: ?GetCandidateList@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2670, 61 bytes (+ int3 padding). __fastcall (himc ecx, dwIndex edx,
// ppCandList on the stack, `ret 4`): file-static in ImeUi.cpp (FableWin.pdb "GetCandidateList") but address-taken
// (ImeUi_Initialize 0x00CB5243 stores it into the _GetCandidateList pointer 0x0143CD00 when the IMM is Unicode), so
// LTCG kept the declared convention. The manifest label "IME_GetCompositionStringW" is descriptive.
// Callees: the _ImmGetCandidateList pointer 0x0143CD5C (WINAPI; ImmGetCandidateListW/A thunk), the
// ImeUiCallback_Malloc pointer 0x0143E514 (no calling convention in this SDK drop, so /Gr __fastcall).
// Shape: the DXUT body verbatim; the second call passes the malloc result straight from eax.
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;
struct tagCANDIDATELIST;

extern unsigned long (__stdcall* _ImmGetCandidateList)(HIMC himc, unsigned long deIndex, tagCANDIDATELIST* lpCandList,
                                                       unsigned long dwBufLen);           // 0x0143CD5C
extern void* (__fastcall* ImeUiCallback_Malloc)(unsigned int bytes);                    // 0x0143E514

unsigned long __fastcall GetCandidateList(HIMC himc, unsigned long dwIndex, tagCANDIDATELIST** ppCandList)
{
    unsigned long dwBufLen = _ImmGetCandidateList(himc, dwIndex, 0, 0);
    if (dwBufLen)
    {
        *ppCandList = (tagCANDIDATELIST*)ImeUiCallback_Malloc(dwBufLen);
        dwBufLen = _ImmGetCandidateList(himc, dwIndex, *ppCandList, dwBufLen);
    }
    return dwBufLen;
}