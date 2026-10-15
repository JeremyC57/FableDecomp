// CEngineInternalPrimitiveParticleGroup::~CEngineInternalPrimitiveParticleGroup() -- destroy FreePrimitiveIDs (+0x58), free the PrimitiveIDToIterator array (+0x4c), clear and free the Primitives list (+0x48), then the inlined CIVCountedPointeeBase destructor stores its vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBA140, length 62 bytes.
// Callees: list<long>::~list @ 0x0042AB73 (out of line: clear, free the node), free @ 0x00BFEA14,
// list<CEnginePrimitiveHandle>::clear @ 0x00834F90 (manifest label "SortTreeRecursively" is propagated).
// Identity: called by the deleting destructors of the particle group classes (0x00BBA240 = slot 0
// of vtable 0x012A5A54 CEngineInternalPrimitiveParticleDecalGroup, and five siblings), so this is
// the shared group destructor; the manifest label "CTCAssassinRush::FinishMove" is propagated.
// Layout: the retail group (landed 0x00BBD1A0 model) holds the 4-byte STLport list Primitives at
// +0x48, the three-pointer CArray PrimitiveIDToIterator at +0x4c and the 4-byte list
// FreePrimitiveIDs at +0x58 (the PDB containers are wider). Vtable 0x012354D0 = CIVCountedPointeeBase:
// the store of the root's inline destructor; the group and its intermediate bases store none
// (`__declspec(novtable)`). Members go in reverse declaration order; the Primitives list's inline
// destructor reads its node back through the list's address after clear().
// Leaf: ??1CEngineInternalPrimitiveParticleGroup
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

struct CPrimitiveHandleList {                               // STLport list<CEnginePrimitiveHandle>
    void* m_pNode;
    void clear();                                           // 0x00834F90
    ~CPrimitiveHandleList()
    {
        clear();
        void* node = m_pNode;
        if (node != 0)
            free(node);
    }
};

struct CPrimitiveIterator { unsigned char m_used; void* m_pNode; };

struct CIteratorArray {                                     // CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    ~CIteratorArray()
    {
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};

struct CIdList {                                            // STLport list<long>
    void* m_pNode;
    ~CIdList();                                             // 0x0042AB73
};

struct CIVCountedPointeeBase {                              // vtable 0x012354D0
    virtual void v00();
    ~CIVCountedPointeeBase() { }
};

struct __declspec(novtable) CEngineInternalPrimitiveParticleGroup : CIVCountedPointeeBase {
    unsigned char        _pad04[0x48 - 0x04];
    CPrimitiveHandleList Primitives;                        // +0x48
    CIteratorArray       PrimitiveIDToIterator;             // +0x4c
    CIdList              FreePrimitiveIDs;                  // +0x58
    virtual void v01();
    ~CEngineInternalPrimitiveParticleGroup();
};

CEngineInternalPrimitiveParticleGroup::~CEngineInternalPrimitiveParticleGroup()
{
}