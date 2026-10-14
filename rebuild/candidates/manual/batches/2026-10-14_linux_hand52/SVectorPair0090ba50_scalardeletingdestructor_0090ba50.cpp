// SVectorPair_0090ba50::`scalar deleting destructor'(unsigned int flags) -- free the POD vector blocks at +0xc then +0 (0x00BFEA14, each when present) and the record for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090BA50, length 56 bytes.
// Leaf: ??_GSVectorPair_0090ba50@@QAEPAXI@Z
// The chunk label is propagated; named by effect (two byte-identical bodies 0x007F3F20 / 0x0090BA50). Shape: members destroyed in reverse order, `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };
struct SVectorPair_0090ba50 {
    CRetailVector_pod m_a;                         // +0x00
    CRetailVector_pod m_b;                         // +0x0c
    ~SVectorPair_0090ba50() { }
};

void Kill_0090ba50(SVectorPair_0090ba50* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
