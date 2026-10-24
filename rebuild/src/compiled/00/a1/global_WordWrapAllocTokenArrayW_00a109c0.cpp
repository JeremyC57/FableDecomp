// WordWrap_AllocTokenArrayW(unsigned short*** tokens, long count) -- allocate a null-terminated array of `count` wide-string token pointers into *tokens unless one is already there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A109C0, length 39 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (MSVCR71 scalar `??2@YAPAXI@Z`).
// Module: the DirectX-sample word-wrap code linked into Fable.exe (WordWrap_SetOption 0x00A0FDB0 ..
// WordWrap_TokenizeW 0x00A10720 / TokenizeA 0x00A10800); this helper and its byte twin 0x00A109F0 sit right
// after the tokenizers with no callers (dead W/A pair kept by the linker). Name descriptive (no PDB name).
// Shape: `__fastcall` (array slot in ecx, count in edx, plain `ret`); `new T*[count + 1]` gives
// `lea eax,[esi*4+4]` and, with no `operator new[]` declared in the TU (the era's sample code included no
// <new>), VC7.1 binds the array new to the SCALAR operator new import, as retail does. The terminator is
// stored through the returned pointer (`mov [eax+esi*4],0`).
// Leaf: WordWrap_AllocTokenArrayW
void __fastcall WordWrap_AllocTokenArrayW(unsigned short*** tokens, long count)
{
    if (*tokens == 0) {
        *tokens = new unsigned short*[count + 1];
        (*tokens)[count] = 0;
    }
}