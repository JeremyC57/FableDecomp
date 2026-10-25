// std::pair::`scalar deleting destructor'(unsigned int flags) -- (std::pair<unsigned __int64, std::list<C2DLineF> >) the compiler-generated deleting destructor: the implicit ~pair runs second's destructor (std::list<C2DLineF> at +0x8), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_G?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A81D60, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: std::pair<unsigned __int64, std::list<C2DLineF> >::`scalar deleting destructor' -- probable; evidence: the only destructible member is the list<C2DLineF> at +8 (destructor 0x005127A0 = ??1?$list@VC2DLineF@@...@std@@QAE@XZ, a landed binding); FableWin.pdb's ??_G name set holds ??_G?$pair@_KV?$list@VC2DLineF@@...@std@@@std@@QAEPAXI@Z, the element of Ego_r.pdb CNavigationLayer::SwitchableLineLists (CVectorMap<unsigned __int64, list<C2DLineF> >), and the body sits in the navigation-layer module (CNavQuadTree / CNavigationLayer 0x00A77480..0x00A85700); CNavigationAreaUpdate::CSwitchableBlockage has the same layout but its code lives at 0x0051xxxx.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: std::list<C2DLineF>::~list() 0x005127A0, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Point1, Point2; };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    ~list();                                                 // 0x005127A0
    void* _M_node;
};
}
void DeleteOne(std::pair<unsigned __int64, std::list<C2DLineF> >* p) { delete p; }