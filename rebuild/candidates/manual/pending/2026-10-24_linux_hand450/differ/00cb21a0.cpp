// _strtoul(const char* psz, unsigned short** ppEnd, int nBase) -- DXUT ImeUi.cpp: parse a "0x"-prefixed hexadecimal or a decimal unsigned number (the end pointer and base are ignored)
// Grade: DIFFER(148v143) -- LTCG custom convention: retail takes psz in EDX (ecx unused); ours takes it in ecx
// Leaf: ?_strtoul@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB21A0, 143 bytes. File-static in ImeUi.cpp (FableWin.pdb
// "_strtoul"); its only caller is CheckInputLocale 0x00CB3877 (`lea edx,[esp+8] ; call` for
// `g_uCodePage = _strtoul(szCodePage, NULL, 0)`), so LTCG dropped the two dead arguments and moved psz to edx.
// The manifest label "ParseIntFromString" is descriptive.
// Shape: the DXUT body verbatim; external here so the TU emits it.
unsigned long __fastcall _strtoul(const char* psz, unsigned short**, int)
{
    if (!psz)
        return 0;

    unsigned long ulRet = 0;
    if (psz[0] == '0' && (psz[1] == 'x' || psz[1] == 'X'))
    {
        psz += 2;
        unsigned long ul = 0;
        while (*psz)
        {
            if ('0' <= *psz && *psz <= '9')
                ul = *psz - '0';
            else if ('A' <= *psz && *psz <= 'F')
                ul = *psz - 'A' + 10;
            else if ('a' <= *psz && *psz <= 'f')
                ul = *psz - 'a' + 10;
            else
                break;
            ulRet = ulRet * 16 + ul;
            psz++;
        }
    }
    else {
        while (*psz && ('0' <= *psz && *psz <= '9'))
        {
            ulRet = ulRet * 10 + (*psz - '0');
            psz++;
        }
    }
    return ulRet;
}
