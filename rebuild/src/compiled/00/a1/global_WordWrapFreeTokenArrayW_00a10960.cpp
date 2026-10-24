// WordWrap_FreeTokenArrayW(unsigned short** tokens) -- delete every wide-string token of a null-terminated token array, then the array itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10960, length 47 bytes (+ int3 padding).
// Callees: operator delete @ 0x00BFE9BC (MSVCR71 scalar `??3@YAXPAX@Z`).
// Module: the DirectX-sample word-wrap code after WordWrap_TokenizeW 0x00A10720 / TokenizeA 0x00A10800; this and its
// byte twin 0x00A10990 have no callers. Name descriptive (no PDB name).
// Shape: `__fastcall` (array in ecx, plain `ret`); null test of the array, a do-while over the tokens through a
// walking pointer while the array base waits in a copy for the final delete: retail walks the
// PARAMETER itself (esi) and keeps the copy in edi (a separate walking local swaps esi/edi). `delete[]` of a char-sized array
// with no `operator delete[]` declared binds to the scalar operator delete import, as retail does.
// Leaf: WordWrap_FreeTokenArrayW
void __fastcall WordWrap_FreeTokenArrayW(unsigned short** tokens)
{
    if (tokens) {
        unsigned short** array = tokens;
        for (; *tokens; ++tokens)
            delete[] *tokens;
        delete[] array;
    }
}