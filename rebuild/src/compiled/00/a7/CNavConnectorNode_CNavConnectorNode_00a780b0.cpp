// CNavConnectorNode::~CNavConnectorNode() -- unlink the two connected leaf nodes: drop each from the other's neighbour list (swap the hit to the back, shrink by one) and remove the region connection between their regions
// Leaf: ??1CNavConnectorNode@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A780B0, 238 bytes, plain `ret`. Ego_r.pdb CNavConnectorNode { CNavNavigableLeafNode* Neighbour1 +0,
//   Neighbour2 +4 }.
// Callees: STLport 4.6 std::__find<const CANavQuadTreeNode**, CNavNavigableLeafNode*> (random-access, 4x unrolled; ICF-folded at 0x00A7D050,
//   the same TU-visible template body as the landed CNavNavigableLeafNode::IsNeighbourOf 0x00A76C30), CSmallVector<const
//   CANavQuadTreeNode*,8>::resize 0x00A7DB10 (the inline pop_back, default fill `T()` stored per call), CANavQuadTreeNode::GetRegion
//   (virtual slot 7), CNavRegion::RemoveConnectionToRegion 0x00A78060.
// Shape: std::find with the member itself as the `const T&` value (T = CNavNavigableLeafNode*, so `&Neighbour2` / `this` are pushed
//   directly) and one shared iterator-tag temporary; the visible __find body tells VC7.1 it writes no memory, so the first list's data
//   pointer stays in ebp across both calls (the draft's opaque Find_00a7d050 swapped ebx/ebp). Swaps through std::swap.
class CNavRegion { public:
    void RemoveConnectionToRegion(CNavRegion* other);      // 0x00A78060
};
class CANavQuadTreeNode;
template <class T, int N> class CSmallVector { public:
    T*          m_data;                                    // +0
    signed char m_size;                                    // +4
    signed char m_capacity;                                // +5
    void resize(long size, const T& fill = T());           // 0x00A7DB10
    T* begin() { return m_data; }
    T* end() { return m_data + m_size; }
    void pop_back() { resize(m_size - 1); }
};
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual CNavRegion* GetRegion() const;                 // slot 7 (+0x1c)
    C2DVector     Position;                                // +0x04
    unsigned char QuadTreeLevel;                           // +0x0c
    unsigned char NavLayer;                                // +0x0d
    bool          LeafNode;                                // +0x0e
    bool          Blocked;                                 // +0x0f
};
class CNavNavigableLeafNode : public CANavQuadTreeNode { public:
    CNavRegion*                                  PRegion;            // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8>    Neighbours;         // +0x14
    unsigned char                                PreferabilityCost;  // +0x1c
};
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag: input_iterator_tag {};
struct bidirectional_iterator_tag: forward_iterator_tag {};
struct random_access_iterator_tag: bidirectional_iterator_tag {};
template<class I,class T> __declspec(noinline) I __fastcall __find(I first,I last,const T&value,const random_access_iterator_tag&) {
 long count=(last-first)>>2;
 for(;count>0;--count) {
  if(*first==value)return first; ++first;
  if(*first==value)return first; ++first;
  if(*first==value)return first; ++first;
  if(*first==value)return first; ++first;
 }
 switch(last-first) {
 case 3: if(*first==value)return first; ++first;
 case 2: if(*first==value)return first; ++first;
 case 1: if(*first==value)return first; ++first;
 case 0: default:return last;
 }
}
template<class I,class T> inline I find(I first,I last,const T&value) { return __find(first,last,value,random_access_iterator_tag()); }
template<class T> inline void swap(T& a,T& b) { T tmp=a; a=b; b=tmp; }
}
class CNavConnectorNode { public:
    CNavNavigableLeafNode* Neighbour1;                     // +0
    CNavNavigableLeafNode* Neighbour2;                     // +4
    ~CNavConnectorNode();
};

CNavConnectorNode::~CNavConnectorNode()
{
    const CANavQuadTreeNode** it1 = std::find(Neighbour1->Neighbours.begin(), Neighbour1->Neighbours.end(), Neighbour2);
    const CANavQuadTreeNode** it2 = std::find(Neighbour2->Neighbours.begin(), Neighbour2->Neighbours.end(), Neighbour1);
    if (it1 - Neighbour1->Neighbours.begin() < Neighbour1->Neighbours.m_size - 1)
        std::swap(*it1, Neighbour1->Neighbours.m_data[Neighbour1->Neighbours.m_size - 1]);
    if (it2 - Neighbour2->Neighbours.begin() < Neighbour2->Neighbours.m_size - 1)
        std::swap(*it2, Neighbour2->Neighbours.m_data[Neighbour2->Neighbours.m_size - 1]);
    Neighbour1->Neighbours.pop_back();
    Neighbour2->Neighbours.pop_back();
    CNavRegion* region = Neighbour1->GetRegion();
    region->RemoveConnectionToRegion(Neighbour2->GetRegion());
}