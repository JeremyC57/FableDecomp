// CTCScriptedControl::OnKill() -- clean up every scripted action (the inlined CleanupAllActions), then the empty CTCBase::OnKill
// Leaf: ?OnKill@CTCScriptedControl@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00713700, 51 bytes, thiscall, tail `jmp` to CTCBase::OnKill.
// Identity (proven): the only reference is vtable 0x012658E4 (RTTI .?AVCTCScriptedControl@@) slot 14, which is OnKill in the CTCBase
//   virtual order (dtor, OnSerialise, OnRemove, ConstructFromParams, OnCreate, InitialActivate, OnInitialActivate, RenderUpdate,
//   HasFrameUpdate, FrameUpdate, BulletTimeFrameUpdate, DrawPreparePrimitivesForRendering, Copy, OnDie 13, OnKill 14; the landed
//   CTCAvoidRegionExit::OnKill 0x007E9410 sits in slot 14 and qualifies the same base 0x006868F0); Ego_r.pdb + FableWin.pdb
//   ?OnKill@CTCScriptedControl@@UAEXXZ, donor order right after CleanupAllActions (retail 0x00713630, whose body this one expands).
// Callees: CActionBase virtual slot 5 OnCleanup (+0x14), STLport std::_List_base<CCountedPointer<CTCScriptedControl::CActionBase>,
//   allocator<..> >::clear() 0x00719280, CTCBase::OnKill 0x006868F0 (empty base, qualified call).
// Layout: as CleanupAllActions 0x00713630 (PActionList +0x14; CActionBase Activated +0x08).
// Shape: CleanupAllActions is a plain member defined above OnKill in the same file; VC7.1 expands it on its own (GOTCHAS: a callee
//   retail inlines in one place and calls in others is a plain non-inline member defined earlier in the file).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
protected:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
public:
    void clear();                                          // _list.c, out of line
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    _Tp& front() { return ((_List_node<_Tp>*)this->_M_node._M_data->_M_next)->_M_data; }
    void clear() { _Base::clear(); }
};
}
class CThing;
class CCharString;
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
class CTCBase : public CBaseClassNonCopyable {               // engine/CTCBase.h flattens the vptr: local polymorphic model
protected:
    virtual void OnSerialise(CPersistContext&, bool);      // slot 1
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();
    virtual void InitialActivate();
    virtual void OnInitialActivate();
    virtual void RenderUpdate(float);
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual void BulletTimeFrameUpdate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);
    virtual void OnDie();                                  // slot 13
    virtual void OnKill();                                 // slot 14: 0x006868F0 (empty)
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CScriptGameResourceObjectScriptedThing;
class CTCScriptedControl : public CTCBase {
public:
    class CActionBase : public CBaseClassNonCopyable {     // vtable 0x01265504
    public:
        virtual void Update() = 0;                         // slot 1 (pure; CActionWait::Update 0x00712240 = FableWin ?Update@CActionWait@..)
        virtual void OnActivate();                         // slot 2
        virtual void OnAdd();                              // slot 3
        virtual CCharString GetActionName() const = 0;     // slot 4 (pure; CActionWait 0x00903640 = ?GetActionName@CActionWait@..@@UBE?AVCCharString@@XZ)
        virtual void OnCleanup();                          // slot 5 (+0x14)
        bool IsActive() const { return Activated; }
        CTCScriptedControl* PTCScript;                     // +0x04
        bool Activated;                                    // +0x08
        bool Terminated;                                   // +0x09
    };
    CIntelligentPointer<CScriptGameResourceObjectScriptedThing> PScriptHandleImp;   // +0x0C
    std::list<CCountedPointer<CActionBase> > PActionList;                         // +0x14
    bool Locked;                                                                    // +0x18
    void CleanupAllActions();
    virtual void OnKill();
};

void CTCScriptedControl::CleanupAllActions()
{
    if (!PActionList.empty()) {
        CActionBase* action = PActionList.front().Get();
        if (action && action->IsActive())
            action->OnCleanup();
    }
    PActionList.clear();
}

void CTCScriptedControl::OnKill()
{
    CleanupAllActions();
    CTCBase::OnKill();
}