// CEngineInternalPrimitiveParticleGroup::`scalar deleting destructor'(unsigned int flags) -- destroy FreePrimitiveIDs, PrimitiveIDToIterator and Primitives, restore the CIVCountedPointeeBase vtable, free the object when bit 0 is set
// Grade: DIFFER(69v82) -- the user `__forceinline ~Group() {}` stores the group vtable at entry and leaves list<CEnginePrimitiveHandle>::~list out of line; with no declared destructor (retail's shape) VC7.1 calls the implicit ??1 instead of inlining it; novtable drops the ??_G entirely
// Leaf: ??_GCEngineInternalPrimitiveParticleGroup
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBA1E0, 81 bytes, thiscall, `ret 4`, returns this.
// Identity: slot 0 of vtable 0x012A59D4 (CEngineInternalPrimitiveParticleGroup, rtti-vtables.tsv). Catalog
//   "CGuiControlTreePane_OnFileLoadComplete" is propagated and wrong.
// Callees: list<long>::~list 0x0042AB73 (FreePrimitiveIDs, out of line), free 0x00BFEA14, list<CEnginePrimitiveHandle>::clear
//   0x00834F90, operator delete 0x00BFE9BC.
// Layout: the retail group of the landed destructor 0x00BBA140 / ClearAllChildPrimitives 0x00BBD200 (Primitives +0x48,
//   PrimitiveIDToIterator +0x4c, FreePrimitiveIDs +0x58; 0x14 below the PDB offsets of
//   engine/CEngineInternalPrimitiveParticleGroup.h, so `this` is a local model).
// Shape: the same member teardown as the landed ??1 at 0x00BBA140, inlined into the deleting destructor. The group
//   declares no destructor, so its compiler-generated one stores no vtable of its own; only the root's inline
//   `virtual ~CIVCountedPointeeBase() {}` stores 0x012354D0, after the members. The factory below references the
//   vtable so the deleting destructor is emitted.
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
    virtual ~CIVCountedPointeeBase() { }
};

struct CEngineInternalPrimitiveParticleGroup : CIVCountedPointeeBase {   // vtable 0x012A59D4
    unsigned char        _pad04[0x48 - 0x04];
    CPrimitiveHandleList Primitives;                        // +0x48
    CIteratorArray       PrimitiveIDToIterator;             // +0x4c
    CIdList              FreePrimitiveIDs;                  // +0x58
    __forceinline ~CEngineInternalPrimitiveParticleGroup() { }
};

void* MakeParticleGroup()
{
    return new CEngineInternalPrimitiveParticleGroup;
}
