// NCameraModes::CPhysicsLeaf::FrameUpdate() -- camera physics leaf <CPhysicsTargetModelGoTo, CPhysicsForceModelGotoOutsideSimple, CPhysicsCoordCartesian>: run the force model's and the coordinate model's frame updates, then reset the go-to physics state (CullPhysics off, both attractor lists cleared)
// Leaf: ?FrameUpdate@?$CPhysicsLeaf@VCPhysicsTargetModelGoTo@NCameraModes@@VCPhysicsForceModelGotoOutsideSimple@2@VCPhysicsCoordCartesian@2@@NCameraModes@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00850640, 44 bytes, thiscall, tail jump to the second list clear.
// Identity (proven): the only reference is RTTI vtable 0x01275974 (.?AV?$CPhysicsLeaf@VCPhysicsTargetModelGoTo@NCameraModes@@VCPhysicsForceModelGotoOutsideSimple@2@VCPhysicsCoordCartesian@2@@NCameraModes@@)
//   slot 0; slot 1 is Reset (0x00850630: OurForce->Reset()), slot 2 the inherited CPhysicsTargetModelGoTo::GetType 0x0084FD10, slot 3 the
//   ??_G (0x00850670). Ego_r.pdb / FableWin.pdb hold ?FrameUpdate@?$CPhysicsLeaf@VCPhysicsTargetModelGoTo@NCameraModes@@VCPhysicsForceModelGotoOutsideSimple@2@VCPhysicsCoordCartesian@2@@NCameraModes@@UAEXXZ. Its byte twin is
//   0x0085CA10 (the <..., CPhysicsForceModelRigid, CPhysicsCoordRigid> instantiation, vtable 0x01276AEC): a different template instantiation, not the same symbol.
// Callees: the force model's and the coordinate model's virtual FrameUpdate (slot 0 of each), std::_List_base<std::pair<C3DVector,
//   float>>::clear() 0x0056CAD0 (STLport 4.6 out-of-line clear; its only callers are these two FrameUpdate rows; the catalog label
//   "CGuiControlTreePane::SortTreeRecursively" is a propagated byte-twin name).
// Shape: CPhysicsState::Reset() (FableWin ?Reset@CPhysicsState@NCameraModes@@QAEXXZ; no out-of-line copy in retail) inlines to the
//   CullPhysics store and the two list clears; GetPhysicsState() = GetOwner().GetGoToPhase() is `mov esi,[esi+4] ; add esi,54h`
//   (retail's out-of-line copy of it is 0x0084FD20). The base CSubComponentBase vtable 0x012758F4 (_purecall, 0x0084FBF0, _purecall,
//   ??_G) fixes the slot order FrameUpdate, Reset, GetType, destructor; CPhysicsLeaf::Reset (slot 1) forwards to OurForce->Reset().
// Layout: Ego_r.pdb CPhysicsLeaf<...> (0x14): CSubComponentBase {vptr, Owner +0x04, CameraManager +0x08}, OurForce +0x0C,
//   OurCoords +0x10 (boost::scoped_ptr); CPhysicsState (retail 0x30: Position, Velocity, Acceleration, then 4-byte STLport lists
//   Attractors +0x24 / IncidentalAttractors +0x28, CullPhysics +0x2C); NCameraModes::CBase GoTo at retail +0x54 (Ego_r +0x68 less the
//   12-byte STLport LookAt lists and SubComponents vector shrink). C3DVector is pack(1) in retail.
extern "C" void __cdecl free(void* p);

#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    void clear();                                          // out of line (0x0056CAD0 for pair<C3DVector, float>)
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x0: the sentinel
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    void clear() { _Base::clear(); }
};
} // namespace std

class CGameCameraManager;

namespace boost {
template <class T> class scoped_ptr {
public:
    T* operator->() const { return ptr; }
private:
    T* ptr;
};
}

namespace NCameraModes {

enum ESubComponentType { };

class CPhysicsState {
public:
    void Reset()                                           // FableWin ?Reset@CPhysicsState@NCameraModes@@QAEXXZ (inline in retail)
    {
        CullPhysics = false;
        Attractors.clear();
        IncidentalAttractors.clear();
    }
    C3DVector                                    Position;               // +0x00
    C3DVector                                    Velocity;               // +0x0C
    C3DVector                                    Acceleration;           // +0x18
    std::list<std::pair<C3DVector, float> >      Attractors;             // +0x24
    std::list<std::pair<C3DVector, float> >      IncidentalAttractors;   // +0x28
    bool                                         CullPhysics;            // +0x2C
};

class CBase {
public:
    CPhysicsState& GetGoToPhase() { return GoTo; }         // FableWin ?GetGoToPhase@CBase@NCameraModes@@QAEAAVCPhysicsState@2@XZ
    unsigned char  _pad00[0x54];
    CPhysicsState  GoTo;                                   // +0x54 (Ego_r +0x68)
};

class CSubComponentBase {
public:
    virtual void FrameUpdate() = 0;                        // slot 0
    virtual void Reset();                                  // slot 1
    virtual ESubComponentType GetType() const = 0;         // slot 2
    virtual ~CSubComponentBase();                          // slot 3
protected:
    CBase& GetOwner() { return *Owner; }                   // FableWin ?GetOwner@CSubComponentBase@NCameraModes@@IAEAAVCBase@2@XZ
    CBase*              Owner;                             // +0x04
    CGameCameraManager* CameraManager;                     // +0x08
};

class CPhysicsTargetModelGoTo : public CSubComponentBase {
public:
    virtual ESubComponentType GetType() const;             // 0x0084FD10
    CPhysicsState& GetPhysicsState() { return GetOwner().GetGoToPhase(); }   // FableWin ?GetPhysicsState@CPhysicsTargetModelGoTo@NCameraModes@@QAEAAVCPhysicsState@2@XZ
};

class CPhysicsForceModelBase {
public:
    virtual void FrameUpdate();                            // slot 0
    virtual void Reset();                                  // slot 1
};
class CPhysicsCoordModelBase {
public:
    virtual void FrameUpdate() = 0;                        // slot 0
};
class CPhysicsForceModelGotoOutsideSimple : public CPhysicsForceModelBase { };
class CPhysicsCoordCartesian : public CPhysicsCoordModelBase { };
class CPhysicsForceModelRigid : public CPhysicsForceModelBase { };
class CPhysicsCoordRigid : public CPhysicsCoordModelBase { };

template <class TTarget, class TForce, class TCoord>
class CPhysicsLeaf : public TTarget {
public:
    virtual void FrameUpdate()
    {
        OurForce->FrameUpdate();
        OurCoords->FrameUpdate();
        this->GetPhysicsState().Reset();
    }
    virtual void Reset();
    boost::scoped_ptr<CPhysicsForceModelBase>  OurForce;   // +0x0C
    boost::scoped_ptr<CPhysicsCoordModelBase>  OurCoords;  // +0x10
};


template void CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelGotoOutsideSimple, CPhysicsCoordCartesian>::FrameUpdate();

} // namespace NCameraModes