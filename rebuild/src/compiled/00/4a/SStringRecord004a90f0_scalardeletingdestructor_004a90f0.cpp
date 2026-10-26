// SStringRecord_004a90f0::`scalar deleting destructor'(unsigned int flags) -- destroy the six CCharStrings of a 0x1c-byte record (+0x18, +0x14, +0x10, +0xc, +4, +0; 0x0099EAE0) and free it for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A90F0, length 80 bytes.
// Leaf: ??_GSStringRecord_004a90f0@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: no vtable (a non-polymorphic class deleted with `delete`), `this` in esi, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct SStringRecord_004a90f0 {
    CCharString m_0;                               // +0x00
    CCharString m_4;                               // +0x04
    long        m_8;                               // +0x08
    CCharString m_c;                               // +0x0c
    CCharString m_10;                              // +0x10
    CCharString m_14;                              // +0x14
    CCharString m_18;                              // +0x18
    ~SStringRecord_004a90f0() { }
};

void Kill_004a90f0(SStringRecord_004a90f0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}