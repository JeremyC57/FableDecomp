// CTreePointer_00c555e0::CTreePointer_00c555e0() -- constructor of a 12-byte member of the libpk decoded-stream voice: the first and third words cleared (the middle word is left unset)
// Leaf: ??0CTreePointer_00c555e0@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6BFF0, 16 bytes, thiscall, plain `ret`, returns `this`.
// Callers: 0x00C55626 / 0x00C55631, the landed constructor 0x00C555E0 (vtable 0x012B8AE0), for its members at +0x224 and
//   +0x230 (0xC apart).
// Name: the reloc binding of the landed caller already spells this address `??0CTreePointer_00c555e0@@QAE@XZ`; kept so the
//   address has one name (descriptive; libpk has no PDB). Label correction: the catalog's "CBaseIntelligentPointer" is
//   propagated across the byte-identical body (CBaseIntelligentPointer is an engine class whose ctor is not this code).
// Shape: an init list of the two words; `mov eax,ecx` first because a constructor returns `this`.
struct CTreePointer_00c555e0 {                  // 12 bytes
    void*         m_pFirst;                     // +0x00
    unsigned long m_middle;                     // +0x04 (not initialised)
    void*         m_pLast;                      // +0x08
    CTreePointer_00c555e0();
};

CTreePointer_00c555e0::CTreePointer_00c555e0()
    : m_pFirst(0), m_pLast(0)
{
}