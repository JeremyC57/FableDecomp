// QuadricOptimiserInternals::COptimisedPrimitive::Optimise(float maxCost) -- contract vertex pairs cheapest first until the cheapest costs more than the limit: drop contractions that no longer exist, re-cost illegal fold-overs with the high penalty, perform the rest
// Grade: DIFFER(383v383) -- register naming only: retail keeps the copied contraction's Data (and the erased node) in edi and Info in esi, ours the reverse; every other byte equal. This wave: PDB respell (Optimise / IsIllegal / DetectFoldOver(const&) / PerformContraction(&) / UpdateCost(const CCountedPointer&)) =, the real same-TU UpdateCost body (erase + insert of (Cost, contraction)) =, header-form split release `--Count; if (Count == 0)` =. Earlier: copy-ctor load/store orders, raw Data local, local info in the release, if/else vs continue/break, shared engine/CCountedPointer.h (worse).
// Leaf: ?Optimise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXM@Z
// Identity: 0x00ACD960 is ?Optimise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXM@Z, not the catalog's 'SimplifyByVertexPairs' (no PDB has that name): its only caller 0x00ACDAE0 walks the sub-meshes' CCountedPointer<COptimisedPrimitive> vectors calling it with the float, and 0x00ACDAE0 is called from C3DQuadricBasedMeshOptimiser::OptimiseMesh 0x00AA41E0 with quality*quality before CollapseSegments(quality*2) 0x00AC8680 -- COptimisedMesh::Optimise(float); the PDB COptimisedPrimitive float members are only Optimise and CollapseSegments. 0x00AC8360 = CVertexPairContraction::IsIllegal (the class's only no-argument bool member in Ego_r/Ego_d; probable). Local model: engine/COptimisedPrimitive.h is in the global namespace and flattens the multimap to bytes.
// Callees: STLport _Rb_tree_rebalance_for_erase 0x0042A34E (labelled CMemoryAllocatorVariableSize::Erase), node free 0x00BFEA14, operator delete 0x00BFE9BC, CVertexPairContraction 0x00AC8360 (bool), COptimisedPrimitive::DetectFoldOver 0x00ACD900 / UpdateCost 0x00AC8A60 (same-TU stand-in body) / PerformContraction 0x00ACD840, multimap::erase(iterator) 0x00AD3C30 (labelled BTreeNode_Erase).
// Layout: Ego_r.pdb VertexPairContractions multimap<float, CCountedPointer<CVertexPairContraction>> at +0x44 less 0x10 (four 12-byte retail CArrays) = +0x34; STLport node value at +0x10 (key) / +0x14 Data / +0x18 Info; CCPPointerInfo {Count, Deleter, Object}.
// Shape: the first erase is inlined, the second calls the out-of-line erase; UpdateCost needs a same-TU stand-in body or the local's words are re-read after the call.
struct CCPPointerInfo {
    long  Count;                                // +0
    void (__fastcall *Deleter)(void* object);   // +4
    void* Object;                               // +8
};
void __cdecl operator_delete_info(void* p);     // stand-in name, see below
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        Data = data;
        Info = info;
        if (info)
            ++info->Count;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->Count == 0) {
            Info->Deleter(Info->Object);
            delete Info;
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
struct SRbNode {
    long     Color;
    SRbNode* Parent;
    SRbNode* Left;
    SRbNode* Right;
};
SRbNode* __cdecl RebalanceForErase(SRbNode* z, SRbNode*& root, SRbNode*& leftmost, SRbNode*& rightmost);   // 0x0042A34E
void __cdecl FreeNode(void* p);                 // 0x00BFEA14
template <class K, class V> struct SMapNode : SRbNode {
    K first;                                    // +0x10
    V second;                                   // +0x14
};
template <class K, class V> struct CRetailMultimap {   // STLport multimap (12 bytes): header node, count, compare
    typedef SMapNode<K, V> Node;
    SRbNode* Header;
    long     Count;
    long     Compare;
    struct iterator {
        SRbNode* NodePtr;
        iterator(SRbNode* node) : NodePtr(node) { }
        iterator(const iterator& o) : NodePtr(o.NodePtr) { }
        Node* operator->() const { return (Node*)NodePtr; }
    };
    long size() const { return Count; }
    iterator begin() const { return iterator(Header->Left); }
    void erase(iterator position);              // 0x00AD3C30
    __forceinline void erase_inline(iterator position)
    {
        Node* y = (Node*)RebalanceForErase(position.NodePtr, Header->Parent, Header->Left, Header->Right);
        y->second.~V();
        if (y)
            FreeNode(y);
        --Count;
    }
};
namespace QuadricOptimiserInternals {
class CVertexPairContraction { public:
    bool  Exists;                               // +0x00
    bool  Unsplit;                              // +0x01
    unsigned char _pad02[0x2];
    unsigned char SubContractions[0xc];         // +0x04 CArray<CSubContraction> (retail 12 bytes)
    float Weight2;                              // +0x10
    float Cost;                                 // +0x14
    bool IsIllegal();                           // 0x00AC8360
};
typedef CCountedPointer<CVertexPairContraction> CContractionPointer;
class COptimisedPrimitive { public:
    unsigned char _pad00[0x34];
    CRetailMultimap<float, CContractionPointer> VertexPairContractions;   // +0x34
    unsigned char _pad40[0x3c];
    float Parameters_HighPenalty;               // +0x7c COptimisationParameters::HighPenalty
    void Optimise(float maxCost);
private:
    bool DetectFoldOver(const CVertexPairContraction& contraction);   // 0x00ACD900
    void UpdateCost(const CContractionPointer& contraction);          // 0x00AC8A60
    void PerformContraction(CVertexPairContraction& contraction);     // 0x00ACD840
};

void UpdateCostBody(CVertexPairContraction* contraction, COptimisedPrimitive* primitive);   // stand-in callee
// Stand-in body (the real UpdateCost 0x00AC8A60 sits in this TU): it reads the pointer without letting its address escape,
// so VC7.1 keeps the local's two words in esi/edi across the call (the landed same-TU stand-in idiom).
#pragma auto_inline(off)
__declspec(noinline) void COptimisedPrimitive::UpdateCost(const CContractionPointer& contraction)
{
    UpdateCostBody(contraction.Data, this);
}
#pragma auto_inline(on)

void COptimisedPrimitive::Optimise(float maxCost)
{
    while (VertexPairContractions.size() != 0) {
        CContractionPointer contraction = VertexPairContractions.begin()->second;
        if (!contraction->Exists) {
            CRetailMultimap<float, CContractionPointer>::iterator first = VertexPairContractions.begin();
            first->second->Exists = false;
            VertexPairContractions.erase_inline(first);
            continue;
        }
        if (contraction->Cost > maxCost)
            return;
        if (contraction->IsIllegal() && DetectFoldOver(*contraction)) {
            contraction->Cost += Parameters_HighPenalty;
            UpdateCost(contraction);
        } else {
            PerformContraction(*contraction);
            CRetailMultimap<float, CContractionPointer>::iterator first = VertexPairContractions.begin();
            first->second->Exists = false;
            VertexPairContractions.erase(first);
        }
    }
}
}
