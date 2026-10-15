// COptimisedPrimitive::UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction) -- re-sort a live vertex-pair contraction after its cost changed: erase its entry from the cost-ordered multimap and re-insert (cost, contraction), remembering the new position in the contraction
// Leaf: UpdateCost
// Grade: DIFFER(126v121) -- ours keeps the contraction reference in edi (retail esi, map in edi), stores the new ListIterator AFTER the pair's counted-pointer release (retail before it, re-reading contraction.Data right after the insert call) and releases with `dec [eax] ; cmp [esi],0` (retail `dec [esi] ; jne`); tried: whole-expression assignment, __forceinline pair/pointer dtors, a complete single-inheritance owner class for the 4-byte member pointer, a named iterator local, a user iterator operator= with a direct Info release
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC8A60, length 121 bytes (padded to 128).
// Callees: multimap<float,CCountedPointer<CVertexPairContraction>>::erase(iterator) @ 0x00AD3C30 (manifest "BTreeNode_Erase"),
//          ::insert(const value_type&) @ 0x00AD54B0 (manifest "BTree_FindAndInsert"; iterator by hidden pointer),
//          operator delete @ 0x00BFE9BC (the pointer-info block).
// Layout: retail COptimisedPrimitive::VertexPairContractions (12-byte multimap) +0x34; CVertexPairContraction (retail,
// 4 bytes below the PDB after its 16->12-byte CArray): Exists +0, Cost +0x14, ListIterator +0x24.
// CCountedPointer = {Data, Info}; CCPPointerInfo = {count, destroy (member pointer), object}.
// Shape: the erase iterator is a by-value class with a user copy constructor (built in the argument area:
// `push ecx ; mov edx,esp ; mov [edx],eax`); the inserted pair is a frame temporary whose counted pointer is
// copied with an AddRef and released inline after the call; the insert result lands in the dead parameter slot.
struct CReleaseTarget {};                          // the owner object (single inheritance: 4-byte member pointer)
struct CCPPointerInfo {
    long count;                                     // +0
    void (CReleaseTarget::*destroy)();              // +4
    CReleaseTarget* object;                         // +8
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->count; }
    __forceinline ~CCountedPointer()
    {
        CCPPointerInfo* info = Info;
        if (info && --info->count == 0) {
            (info->object->*info->destroy)();
            operator delete(info);
        }
    }
    T* operator->() const { return Data; }
};
struct SContractionNode;
struct CContractionIterator {                       // the multimap iterator (one node pointer)
    SContractionNode* m_node;
    CContractionIterator() {}
    CContractionIterator(const CContractionIterator& o) : m_node(o.m_node) {}
};
struct CVertexPairContraction;
struct SContractionValue {                          // pair<const float, CCountedPointer<CVertexPairContraction> >
    float                                   first;
    CCountedPointer<CVertexPairContraction> second;
    SContractionValue(const float& f, const CCountedPointer<CVertexPairContraction>& s) : first(f), second(s) {}
    __forceinline ~SContractionValue() {}
};
struct CContractionMap {                            // multimap<float, CCountedPointer<CVertexPairContraction> > (12 bytes)
    void* m_allocator; void* m_head; unsigned long m_size;
    void erase(CContractionIterator where);                     // 0x00AD3C30
    CContractionIterator insert(const SContractionValue& value);// 0x00AD54B0
};
struct CVertexPairContraction {                     // QuadricOptimiserInternals::CVertexPairContraction (retail)
    bool                 Exists;                    // +0x00
    bool                 Unsplit;                   // +0x01
    unsigned char        SubContractions[0xc];      // +0x04
    float                Weight2;                   // +0x10
    float                Cost;                      // +0x14
    float                ResultingPosition[3];      // +0x18
    CContractionIterator ListIterator;              // +0x24
};

class COptimisedPrimitive {
public:
    unsigned char   _pad00[0x34];
    CContractionMap VertexPairContractions;         // +0x34
    void UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction);
};

void COptimisedPrimitive::UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction)
{
    if (contraction->Exists)
    {
        VertexPairContractions.erase(contraction->ListIterator);
        CContractionIterator position = VertexPairContractions.insert(SContractionValue(contraction->Cost, contraction));
        contraction->ListIterator = position;
    }
}
