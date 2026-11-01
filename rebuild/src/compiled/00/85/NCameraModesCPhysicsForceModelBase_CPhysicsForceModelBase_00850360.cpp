// NCameraModes::CPhysicsForceModelBase::~CPhysicsForceModelBase() -- compiler-generated camera force-model destructor: destroy the ground, themes and things collision-point objects (each three STLport lists)
// Leaf: ??1CPhysicsForceModelBase@NCameraModes@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00850360, 137 bytes, `jmp` to the last list destructor.
// Identity (proven): FableWin.pdb ??1CPhysicsForceModelBase@NCameraModes@@QAE@XZ (0x0288D2B0, non-virtual) beside
//   ??_GCPhysicsForceModelBase@NCameraModes@@QAEPAXI@Z (0x0288D270, non-virtual), boost::checked_delete /
//   scoped_ptr<CPhysicsForceModelBase> (0x0288D150, 0x0288CA80, 0x0288D390); retail: the non-virtual ??_G 0x008503F0 calls
//   this body, and so do checked_delete 0x00850510, ~scoped_ptr 0x00850530, scoped_ptr::reset 0x00850550 and the
//   CPhysicsLeaf<..., CPhysicsForceModelGotoOutsideSimple, ...> ctor 0x0085C950 (reset of its force model). The class ctor
//   0x00850410 (bound ??0CPhysicsForceModelBase@NCameraModes@@QAE@AAVCBase@1@ABVCGameCameraManager@@AAVCPhysicsState@1@@Z)
//   builds the same three 0x24-byte CPhysicsCollisionPointsObject members at +0x08 / +0x2c / +0x50 (0x00850410 calls
//   CPhysicsCollisionPointsObject(long, float, float, long, long, long) 0x008FB170 on each). reloc_bindings already binds
//   ??1CPhysicsForceModelBase@NCameraModes@@QAE@XZ to 0x00850360. The catalog label
//   CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: std::_List_base<pair<float, long> >::~_List_base 0x008501E0 (out of line; ~list 0x00850220 is its `jmp` thunk),
//   std::_List_base<pair<float, list<C3DVector> > >::clear 0x00850280, free 0x00BFEA14, std::_List_base<C3DVector>::
//   ~_List_base 0x0048CA60 (out of line, kept from an earlier TU; ~list<C3DVector> 0x0048CF80 is its `jmp` thunk).
// Layout (Ego_r.pdb members at retail STLport sizes, list = 4 bytes; no engine header): CPhysicsCollisionPointsObject
//   { Points +0x00 list<C3DVector>; SmartPointList +0x04 list<pair<float, list<C3DVector> > >; NumTestsPerTheta +0x08
//   list<pair<float, long> >; ScanDensity +0x0c; StartTheta +0x10; DeltaTheta +0x14; DebugGraphicsRed/Green/Blue +0x18..+0x20 }
//   (0x24); CPhysicsForceModelBase { vptr; LinkedState +0x04; Things +0x08; Themes +0x2c; Ground +0x50; LastAcceleration
//   +0x74; ScanCenter +0x80; AverageCamPos +0x8c; ScanRadSquared +0x98; TriggerFrame +0x9c; AllowedToCullPhysics +0xa0;
//   Owner +0xa4; CameraManager +0xa8 } (0xac, the size 0x008505AD allocates).
// Shape: no vtable store (compiler-generated destructor of a class with virtuals but no virtual destructor); the three
//   implicit ~CPhysicsCollisionPointsObject expand inline (out of line copy 0x00850330 in the same TU); VC7.1's own
//   inlining decides per list type: ~_List_base<pair<float,long> > and ~_List_base<C3DVector> (clear inlined) stay calls,
//   ~_List_base<pair<float,list> > expands around its out-of-line clear().
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
template <class _Tp, class _Alloc> class _List_base {
protected:
    typedef _List_node<_Tp> _Node;
public:
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
} // namespace std

class C3DVector { public: float X; float Y; float Z; };
class CGameCameraManager;

namespace NCameraModes {
class CBase;
class CPhysicsState;
class CPhysicsCollisionPointsObject {
public:
    CPhysicsCollisionPointsObject(long scanDensity, float startTheta, float deltaTheta, long red, long green, long blue);   // 0x008FB170
    std::list<C3DVector>                                    Points;            // +0x00
    std::list<std::pair<float, std::list<C3DVector> > >     SmartPointList;    // +0x04
    std::list<std::pair<float, long> >                      NumTestsPerTheta;  // +0x08
    long                                                    ScanDensity;       // +0x0c
    float                                                   StartTheta;        // +0x10
    float                                                   DeltaTheta;        // +0x14
    long                                                    DebugGraphicsRed;  // +0x18
    long                                                    DebugGraphicsGreen;   // +0x1c
    long                                                    DebugGraphicsBlue; // +0x20
};
class CPhysicsForceModelBase {
public:
    CPhysicsForceModelBase(CBase& owner, const CGameCameraManager& cameraManager, CPhysicsState& linkedState);   // 0x00850410
    virtual void VirtualSlot0_008fa660();               // slot 0 of vtable 0x0127592C (name not recovered; the class has virtuals, no virtual dtor)
    CPhysicsState*                LinkedState;          // +0x04
    CPhysicsCollisionPointsObject Things;               // +0x08
    CPhysicsCollisionPointsObject Themes;               // +0x2c
    CPhysicsCollisionPointsObject Ground;               // +0x50
    C3DVector                     LastAcceleration;     // +0x74
    C3DVector                     ScanCenter;           // +0x80
    C3DVector                     AverageCamPos;        // +0x8c
    float                         ScanRadSquared;       // +0x98
    long                          TriggerFrame;         // +0x9c
    bool                          AllowedToCullPhysics; // +0xa0
    CBase*                        Owner;                // +0xa4
    CGameCameraManager*           CameraManager;        // +0xa8
};
} // namespace NCameraModes

namespace boost {
// 0x00850510: `delete x` of the force model (the free template functions of this TU are __fastcall, /Gr).
template <class T> inline void __fastcall checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}
template void __fastcall checked_delete<NCameraModes::CPhysicsForceModelBase>(NCameraModes::CPhysicsForceModelBase* x);
} // namespace boost