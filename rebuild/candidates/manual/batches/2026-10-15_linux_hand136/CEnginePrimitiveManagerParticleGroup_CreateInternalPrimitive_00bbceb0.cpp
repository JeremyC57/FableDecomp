// CEnginePrimitiveManagerParticleGroup::CreateInternalPrimitive(long) -- allocate and construct a particle-group primitive (0xac bytes: base type 0x18, flags 0x80000, two empty lists, an empty id array, a zeroed bounding sphere); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBCEB0, length 115 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @
// 0x00B8FF00 (0x18, 0x80000), malloc @ 0x00BFEA0E (each list's sentinel node). Vtable 0x012A59D4 =
// CEngineInternalPrimitiveParticleGroup (rtti-vtables.tsv). The type argument is unused (`ret 4`).
// Layout: the retail group of the landed destructor 0x00BBA140 / AddChildPrimitive 0x00BBD1A0: the 4-byte STLport
// list Primitives (+0x48, 16-byte nodes holding a CEnginePrimitiveHandle), the three-pointer CArray
// PrimitiveIDToIterator (+0x4c), the 4-byte list<long> FreePrimitiveIDs (+0x58, 12-byte nodes), the bounding sphere
// at +0x5c; 0xac bytes (the PDB's 0xc0 less the wider STL containers).
// Shape: the inlined group constructor: base call, vtable store, then the member constructors in order -- each
// list zeroes its proxy, mallocs and self-links a local node and stores it, the array and the sphere zero their
// words from the shared zero register edi; the two mallocs' argument words are popped together (`add esp,8`).
// The group constructor must be `__forceinline` (plain `inline` leaves it out of line behind a null test).
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

template<class T> struct CRetailList {             // STLport list: the node pointer only
    struct SNode { SNode* m_pNext; SNode* m_pPrev; T m_value; };
    SNode* m_pNode;
    CRetailList() : m_pNode(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pNode = node;
    }
};

struct CEnginePrimitiveHandle { void* m_vptr; void* m_pData; };   // 8 bytes (vptr + counted pointer)
struct CPrimitiveIterator { unsigned char m_used; void* m_pNode; };
struct CIteratorArray {                            // retail CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    CIteratorArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};
struct C3DBoundingSphere {                         // centre + radius, zeroed by its inline constructor (as dwords)
    long m_centreX, m_centreY, m_centreZ, m_radius;
    C3DBoundingSphere() : m_centreX(0), m_centreY(0), m_centreZ(0), m_radius(0) { }
};

struct CEngineInternalPrimitiveParticleGroup : CEngineInternalPrimitiveBase {   // vtable 0x012A59D4; 0xac bytes (retail)
    CRetailList<CEnginePrimitiveHandle> Primitives;            // +0x48 (16-byte nodes)
    CIteratorArray                      PrimitiveIDToIterator; // +0x4c
    CRetailList<long>                   FreePrimitiveIDs;      // +0x58 (12-byte nodes)
    C3DBoundingSphere                   BSphere;               // +0x5c
    unsigned char                       _rest6c[0xac - 0x6c];  // DistanceFromCam, WorldMat, flags (not initialised here)
    virtual void v00();
    __forceinline CEngineInternalPrimitiveParticleGroup(long type, unsigned long flags)
        : CEngineInternalPrimitiveBase(type, flags) { }
};

struct CEnginePrimitiveManagerParticleGroup {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerParticleGroup::CreateInternalPrimitive(long type)
{
    return new CEngineInternalPrimitiveParticleGroup(0x18, 0x80000);
}
