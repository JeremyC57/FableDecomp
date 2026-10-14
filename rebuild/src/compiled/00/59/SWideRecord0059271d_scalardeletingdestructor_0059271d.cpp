// SWideRecord_0059271d::`scalar deleting destructor'(unsigned int flags) -- destroy the CWideString at +0x1c (0x0099B510) and free the record for flag bit 0 (`pop ecx` after the delete: the size-optimised module)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059271D, length 31 bytes.
// Leaf: ??_GSWideRecord_0059271d@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: no vtable (a non-polymorphic class deleted with `delete`), `this` in esi, `test byte ptr [esp+8],1`, `ret 4`.
#pragma optimize("s", on)
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CWideString {
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
struct SWideRecord_0059271d {
    unsigned char _pad[0x1c];
    CWideString   m_text;                          // +0x1c
    ~SWideRecord_0059271d() { }
};

void Kill_0059271d(SWideRecord_0059271d* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}