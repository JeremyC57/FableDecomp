// NCameraModes::CPCMouseLook::OnCameraInsideObservedThing() -- restore the last camera state, drop the queued mouse inputs and run a frame update at once
// Leaf: ?OnCameraInsideObservedThing@CPCMouseLook@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873DB0, 86 bytes (int3 padding 0x00873E06..0F), thiscall, tail `jmp CBase::FrameUpdate`.
// Identity (proven, two sources): (1) the only reference is slot 3 of vtable 0x01277784 (RTTI .?AVCPCMouseLook@NCameraModes@@ :
//   NCameraModes::CBase); CBase's own slot 3 (vtable 0x0124DD74) is 0x0056E150, an empty `ret` -- the base
//   ?OnCameraInsideObservedThing@CBase@NCameraModes@@UAEXXZ of the name sets; (2) Ego_r.pdb donor order OnDie 0x008618F0 -> Recentre
//   0x00861900 -> OnCameraInsideObservedThing 0x00861B20 -> Update 0x00861B80 = retail OnDie 0x00873BB0 (slot 22) -> 0x00873BC0
//   (slot 16) -> this row (slot 3) -> Update 0x00873E10 (slot 1); Ego_r's copy is 0x60 bytes (this one 0x56), Recentre 0x220 (retail
//   0x1F0). Ego_r / FableWin ?OnCameraInsideObservedThing@CPCMouseLook@NCameraModes@@UAEXXZ. The catalog "vfunc_3" is a placeholder.
// Callees: free 0x00BFEA14 (the list node allocator's deallocate), CBase::FrameUpdate() 0x0084DC10 (?FrameUpdate@CBase@NCameraModes@@QAEXXZ,
//   both PDBs; non-virtual -- its body calls Update through slot 1; the landed callers bind stand-ins: NAME_FIXES.tsv).
// Body: `StateCurrent = StateLast;` (the trivially copyable 32-byte CRandomPacketOfVariablesDefiningTheCameraState: `rep movsd` x8 from
//   +0xEC to +0xCC), `MouseInputs.clear();` (STLport 4.6 _List_base::clear from _list.c, expanded inline: free every node, then the
//   sentinel points at itself), then `FrameUpdate();` as the tail jump.
// Layout: retail CBase is 0xA4 (the landed 0x0087BBC0; Ego_r 0xC8 less two CPhysicsState lists x2 and one vector): Ego_r members at
//   retail offsets (CameraFOV +0x84 .. as FrameUpdate reads them); CPCMouseLook: MouseInputs (list<C2DVector>) +0xA4, AxisX/Y/Z
//   +0xA8/+0xB4/+0xC0, StateCurrent +0xCC, StateLast +0xEC; sizeof 0x10C. Only slots 0-3 of CBase's 24-slot vtable are declared
//   (the override needs slot 3; no vtable is emitted here). No engine header exists for the NCameraModes classes.
#include <stddef.h>
#include "rebuild_abi.h"
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    void clear();
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;           // STLport 4.6 _list.h
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*) this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*) __cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    void clear() { _List_base<_Tp, _Alloc>::clear(); }
};
}   // namespace std

class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };

namespace NCameraModes {
enum EMode { };
class CBase {                                           // vtable 0x0124DD74; retail sizeof 0xA4
public:
    virtual ~CBase();                                   // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual EMode GetModeType() const = 0;              // slot 2
    virtual void OnCameraInsideObservedThing();         // slot 3 0x0056E150 (`ret`)
    void FrameUpdate();                                 // 0x0084DC10 ?FrameUpdate@CBase@NCameraModes@@QAEXXZ
    float         DefaultFOV;                           // +0x04
    float         DefaultRollAngle;                     // +0x08
    float         DefaultObservedPositionZOffset;       // +0x0C
    unsigned char LookAt_[0x30];                        // CPhysicsState LookAt +0x10
    void*         CameraManager;                        // +0x40 CGameCameraManager*
    unsigned char PCameraModeDef_[0x4];                 // CDefPointer<const CCameraModeDef> PCameraModeDef +0x44
    unsigned char SubComponents_[0xC];                  // vector<CCountedPointer<CSubComponentBase> > SubComponents +0x48
    unsigned char GoTo_[0x30];                          // CPhysicsState GoTo +0x54
    float         CameraFOV;                            // +0x84
    float         RollAngle;                            // +0x88
    float         ObservedPositionZOffset;              // +0x8C
    C2DVector     RightStickVector;                     // +0x90
    C2DVector     LeftStickVector;                      // +0x98
    bool          ViewOccluded;                         // +0xA0
};

class CPCMouseLook : public CBase {                     // vtable 0x01277784; retail sizeof 0x10C
public:
    class CRandomPacketOfVariablesDefiningTheCameraState {   // Ego_r 0x20
    public:
        float     AngleXY;                              // +0x00
        float     AngleUpDown;                          // +0x04
        float     DollyLength;                          // +0x08
        float     DeltaAngleXY;                         // +0x0C
        long      RecentreTimer;                        // +0x10
        C3DVector ViewPosOffset;                        // +0x14
    };
    virtual void OnCameraInsideObservedThing();
    std::list<C2DVector>                          MouseInputs;    // +0xA4
    C3DVector                                     AxisX;          // +0xA8
    C3DVector                                     AxisY;          // +0xB4
    C3DVector                                     AxisZ;          // +0xC0
    CRandomPacketOfVariablesDefiningTheCameraState StateCurrent;  // +0xCC
    CRandomPacketOfVariablesDefiningTheCameraState StateLast;     // +0xEC
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CBase) == 0xA4);
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CPCMouseLook::CRandomPacketOfVariablesDefiningTheCameraState) == 0x20);
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CPCMouseLook) == 0x10C);
FABLE_STATIC_ASSERT(offsetof(NCameraModes::CPCMouseLook, MouseInputs) == 0xA4);
FABLE_STATIC_ASSERT(offsetof(NCameraModes::CPCMouseLook, StateCurrent) == 0xCC);
FABLE_STATIC_ASSERT(offsetof(NCameraModes::CPCMouseLook, StateLast) == 0xEC);

namespace NCameraModes {

void CPCMouseLook::OnCameraInsideObservedThing()
{
    StateCurrent = StateLast;
    MouseInputs.clear();
    FrameUpdate();
}
}