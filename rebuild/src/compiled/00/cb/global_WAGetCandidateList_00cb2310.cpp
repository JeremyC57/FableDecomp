// WA_GetCandidateList(HIMC himc, unsigned long dwIndex, CANDIDATELIST** ppCandList) -- DXUT ImeUi.cpp ANSI-IMM wrapper: fetch the candidate list with ImmGetCandidateListA, build a wide copy (header memcpy, every string widened with MultiByteToWideChar in g_uCodePage, offsets rebased) and return its size
// Leaf: ?WA_GetCandidateList@@YIKPAUHIMC__@@KPAPAUtagCANDIDATELIST@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB2310, 302 bytes, __fastcall (himc ecx, dwIndex edx, ppCandList on the stack,
//   `ret 4`); ImeUi.cpp is a /Gr TU (see the landed GetCandidateList 0x00CB2670 / CheckInputLocale 0x00CB37A0).
// Identity (two sources): (1) FableWin.pdb + Ego_r.pdb name sets list ImeUi.cpp's WA_GetCandidateList, donor-ordered
//   after ComposeCandidateLine and before WA_ImmGetCompositionString / GetCandidateList (retail: this row, then
//   GetCandidateList 0x00CB2670); (2) its only reference is ImeUi_Initialize (0x00CB5269 stores its address into the
//   _GetCandidateList pointer when the IMM is not Unicode, where the landed GetCandidateList is the Unicode choice), and
//   the body is the SDK's "W from A" wrapper: ImmGetCandidateListA twice, then MultiByteToWideChar per candidate.
// Label: the manifest's "IME_GetCandidateListW" is descriptive.
// Callees: imm32 ImmGetCandidateListA (import thunk 0x00CB551C), the ImeUiCallback_Malloc pointer 0x0143E514 and the
//   ImeUiCallback_Free pointer 0x0143CF08 (__fastcall under /Gr), kernel32 MultiByteToWideChar (IAT 0x0143FE74).
// Globals: g_uCodePage 0x0143E8BC.
// Shape: the SDK body verbatim: the second malloc is not null-tested, the header copy is an inline memcpy
//   (rep movsd / rep movsb), the loop counter of the rebasing loop lives in the dead ppCandList slot.
struct HIMC__ { int unused; };   typedef HIMC__* HIMC;
typedef struct tagCANDIDATELIST {
    unsigned long dwSize;          // +0x00
    unsigned long dwStyle;         // +0x04
    unsigned long dwCount;         // +0x08
    unsigned long dwSelection;     // +0x0C
    unsigned long dwPageStart;     // +0x10
    unsigned long dwPageSize;      // +0x14
    unsigned long dwOffset[1];     // +0x18
} CANDIDATELIST, *LPCANDIDATELIST;

extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int count);
#pragma intrinsic(memcpy)

extern "C" unsigned long __stdcall ImmGetCandidateListA(HIMC himc, unsigned long deIndex, LPCANDIDATELIST lpCandList,
                                                        unsigned long dwBufLen);           // import thunk 0x00CB551C
extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int CodePage, unsigned long dwFlags,
    const char* lpMultiByteStr, int cbMultiByte, unsigned short* lpWideCharStr, int cchWideChar);   // [0x0143FE74]

extern void* (__fastcall* ImeUiCallback_Malloc)(unsigned int bytes);                    // 0x0143E514
extern void (__fastcall* ImeUiCallback_Free)(void* ptr);                                // 0x0143CF08
extern unsigned int g_uCodePage;                                                        // 0x0143E8BC

unsigned long __fastcall WA_GetCandidateList(HIMC himc, unsigned long dwIndex, LPCANDIDATELIST* ppCandList)
{
    // Get the candidate list in ANSI, then convert it to Unicode.
    unsigned long dwBufLen = ImmGetCandidateListA(himc, dwIndex, 0, 0);
    if (dwBufLen)
    {
        LPCANDIDATELIST pCandList = (LPCANDIDATELIST)ImeUiCallback_Malloc(dwBufLen);
        if (pCandList)
        {
            dwBufLen = ImmGetCandidateListA(himc, dwIndex, pCandList, dwBufLen);
            if (dwBufLen)
            {
                int i;
                unsigned long dwWideBufLen = 0;
                for (i = 0; i < (int)pCandList->dwCount; i++)
                    dwWideBufLen += MultiByteToWideChar(g_uCodePage, 0, (char*)pCandList + pCandList->dwOffset[i], -1, 0, 0) * sizeof(unsigned short);
                dwWideBufLen += pCandList->dwOffset[0];
                *ppCandList = (LPCANDIDATELIST)ImeUiCallback_Malloc(dwWideBufLen);
                LPCANDIDATELIST pCandListW = *ppCandList;
                memcpy(pCandListW, pCandList, pCandList->dwOffset[0]);
                unsigned short* pwz = (unsigned short*)((char*)pCandListW + pCandList->dwOffset[0]);
                for (i = 0; i < (int)pCandList->dwCount; i++)
                {
                    pCandListW->dwOffset[i] = (char*)pwz - (char*)pCandListW;
                    pwz += MultiByteToWideChar(g_uCodePage, 0, (char*)pCandList + pCandList->dwOffset[i], -1, pwz, 256);
                }
                dwBufLen = dwWideBufLen;
            }
            ImeUiCallback_Free(pCandList);
        }
    }
    return dwBufLen;
}