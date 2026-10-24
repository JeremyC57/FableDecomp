// WordWrap_FreeTokenArrayA(char** tokens) -- delete every narrow-string token of a null-terminated token array, then the array itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A10990, length 47 bytes (+ int3 padding).
// Callees: operator delete @ 0x00BFE9BC (MSVCR71 scalar `??3@YAXPAX@Z`).
// Module: the DirectX-sample word-wrap code; the narrow twin of 0x00A10960 (byte-identical body), no callers.
// Name descriptive (no PDB name).
// Shape: `__fastcall` (array in ecx, plain `ret`); null test of the array, a do-while over the tokens through a
// walking pointer while the array base waits in a copy for the final delete: retail walks the
// PARAMETER itself (esi) and keeps the copy in edi (a separate walking local swaps esi/edi). `delete[]` of a char-sized array
// with no `operator delete[]` declared binds to the scalar operator delete import, as retail does.
// Leaf: WordWrap_FreeTokenArrayA
void __fastcall WordWrap_FreeTokenArrayA(char** tokens)
{
    if (tokens) {
        char** array = tokens;
        for (; *tokens; ++tokens)
            delete[] *tokens;
        delete[] array;
    }
}