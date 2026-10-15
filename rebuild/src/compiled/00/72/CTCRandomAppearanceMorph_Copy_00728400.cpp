// CTCRandomAppearanceMorph::Copy(const CTCRandomAppearanceMorph& other) -- adopt the other morph's mesh-morph set (counted pointer) and seed, then rebuild the morph set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728400, length 87 bytes.
// Callees: the counted block's destroy function (`mov ecx,[eax+8] ; call [eax+4]`), operator delete @ 0x00BFE9BC,
// CTCRandomAppearanceMorph::UpdateMorphSet() @ 0x007280C0.
// Shape: the inlined CCountedPointer::operator= (the landed _Move<CCountedPointer*> 0x0084F130 model): the source's object
// (ebp) and block (edi) are read into locals before the block compare -- here the OBJECT first --, the destination block is
// released with the member re-read after the decrement (`dec [eax] ; mov eax,[esi+14h] ; cmp [eax],0`), then the object /
// block stores and the source block's add-ref; then the Seed copy and the rebuild call.
// Layout = Ego_r.pdb CTCRandomAppearanceMorph (0x30): CTCAppearanceMorphBase 0x10, PMeshMorphSet
// (CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>, object + block) +0x10, Seed +0x18, PrimHandles +0x1c,
// PTCSkeletalMorph +0x2c (PDB vector 16 bytes). No engine header; modelled locally.
struct SCountBlock {
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

struct CMeshMorphSet;
struct CCountedPointer_MeshMorphSet {
    CMeshMorphSet* m_pObject;                               // +0
    SCountBlock*   m_pCount;                                // +4
    CCountedPointer_MeshMorphSet& operator=(const CCountedPointer_MeshMorphSet& other)
    {
        CMeshMorphSet* object = other.m_pObject;
        SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    delete m_pCount;
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
};

struct CTCRandomAppearanceMorph {
    virtual void v00();
    unsigned char                _base[0x0c];               // +0x04 CTCAppearanceMorphBase body
    CCountedPointer_MeshMorphSet PMeshMorphSet;             // +0x10
    unsigned long                Seed;                      // +0x18
    void UpdateMorphSet();                                  // 0x007280C0
    void Copy(const CTCRandomAppearanceMorph& other);
};

void CTCRandomAppearanceMorph::Copy(const CTCRandomAppearanceMorph& other)
{
    PMeshMorphSet = other.PMeshMorphSet;
    Seed = other.Seed;
    UpdateMorphSet();
}