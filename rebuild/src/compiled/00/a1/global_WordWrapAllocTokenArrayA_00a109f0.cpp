// WordWrap_AllocTokenArrayA(char*** tokens, long count) -- allocate a null-terminated array of `count` narrow-string token pointers into *tokens unless one is already there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A109F0, length 39 bytes.
// Callees: operator new @ 0x00BFEA1A (MSVCR71 scalar `??2@YAPAXI@Z`).
// Module: the DirectX-sample word-wrap code (WordWrap_SetOption 0x00A0FDB0 .. WordWrap_TokenizeA 0x00A10800); the
// narrow twin of 0x00A109C0 (byte-identical body), no callers. Name descriptive (no PDB name).
// Shape: as 0x00A109C0 -- `__fastcall`, `new T*[count + 1]` through the scalar operator new (no `operator new[]`
// declared in the TU), terminator stored through the returned pointer.
// Leaf: WordWrap_AllocTokenArrayA
void __fastcall WordWrap_AllocTokenArrayA(char*** tokens, long count)
{
    if (*tokens == 0) {
        *tokens = new char*[count + 1];
        (*tokens)[count] = 0;
    }
}