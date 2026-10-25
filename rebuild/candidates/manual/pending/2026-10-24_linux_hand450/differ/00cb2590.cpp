// ComposeCandidateLine(int index, const unsigned short* pszCandidate) -- DXUT ImeUi.cpp: build candidate line `index`: a 1-digit label, a space (vertical list), the candidate text, a trailing space (horizontal list), NUL
// Grade: DIFFER(105v87) -- LTCG custom convention: retail gets (index + g_iCandListIndexBase) in EAX and &g_szCandidate[index] in ECX precomputed by the caller, pszCandidate in EDX
// Leaf: ?ComposeCandidateLine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB2590, 87 bytes. File-static in ImeUi.cpp (FableWin.pdb
// "ComposeCandidateLine"), one caller (0x00CB4ECC). The manifest label "ImeUI_FormatCompStringDisplay" is descriptive.
// Globals: g_szCandidate (MAX_CANDLIST x 256 TCHAR), g_iCandListIndexBase, g_bVerticalCand 0x013AC748.
// Shape: the 2004/2005 SDK body -- no COUNTOF bound in the copy loop, and the trailing space for a horizontal list.
extern unsigned short g_szCandidate[10][256];
extern int            g_iCandListIndexBase;
extern bool           g_bVerticalCand;             // 0x013AC748

void __fastcall ComposeCandidateLine(int index, const unsigned short* pszCandidate)
{
    unsigned short* psz = g_szCandidate[index];
    *psz++ = (unsigned short)(L'0' + ((index + g_iCandListIndexBase) % 10));
    if (g_bVerticalCand)
    {
        *psz++ = L' ';
    }
    while (*pszCandidate)
    {
        *psz++ = *pszCandidate++;
    }
    if (!g_bVerticalCand)
    {
        *psz++ = L' ';
    }
    *psz = 0;
}
