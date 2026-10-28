// NUISystem::CHoverable::OnHovered() -- remember the requested state as the state before hovering, change to the definition's hovered state, then run the on-hovered actions
// Leaf: ?OnHovered@CHoverable@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055BAE0, 96 bytes, virtual thiscall, plain `ret`.
// Identity (proven): slot 143 (+0x23C) of CHoverable's vtable 0x0124BFB4, inherited by CDraggableInto 0x0124B2D4;
//   it sits between CHoverable::IsHovered (slot 142, 0x0055B8F0) and CHoverable::OnUnhovered (slot 144, 0x0055B9A0), and
//   Ego_r.pdb CHoverable declares OnHovered / OnUnhovered with StateBeforeHovered and PActionsOnHovered / PActionsOnUnhovered.
// Callees (virtual): ChangeState (slot 48, +0xC0; CChangingStateComponent 0x0052CF40), LoadAndReturnDef (slot 108, +0x1B0,
//   CDefPointer<const CUIDef> by value, released inline: --RefCount, slot 1 at zero), ExecuteActions (slot 131, +0x20C).
// Layout: retail = Ego_r.pdb minus 0x30: CChangingStateComponent::RequestedState +0x14C, CHoverable::StateBeforeHovered
//   +0x15C, PActionsOnHovered +0x164; CUIDef::HoveredState +0x204.
namespace std {
template <class T> class list;
}
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
};
namespace NUISystem {
class CAction;
class CUIDef { public:
    virtual void _d000();
    virtual void DestroySelf() const;                           // slot 1 (+4)
    mutable long RefCount;                                      // +0x04
    unsigned char _pad08[0x204 - 8];
    long HoveredState;                                          // +0x204
    void DecRef() const { --RefCount; if (RefCount == 0) DestroySelf(); }
};
}
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
    ~CDefPointer() { if (Object) Object->DecRef(); }
};
namespace NUISystem {
class CUIInterface { public:
};
class CComponent : public CUIInterface { public:
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007();
    virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023();
    virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031();
    virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void ChangeState(unsigned long state);                          // slot 48 (+0xC0)
    virtual void _v049(); virtual void _v050(); virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055(); virtual void _v056();
    virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063(); virtual void _v064();
    virtual void _v065(); virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071(); virtual void _v072();
    virtual void _v073(); virtual void _v074(); virtual void _v075(); virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079(); virtual void _v080();
    virtual void _v081(); virtual void _v082(); virtual void _v083(); virtual void _v084(); virtual void _v085(); virtual void _v086(); virtual void _v087(); virtual void _v088();
    virtual void _v089(); virtual void _v090(); virtual void _v091(); virtual void _v092(); virtual void _v093(); virtual void _v094(); virtual void _v095(); virtual void _v096();
    virtual void _v097(); virtual void _v098(); virtual void _v099(); virtual void _v100(); virtual void _v101(); virtual void _v102(); virtual void _v103(); virtual void _v104();
    virtual void _v105(); virtual void _v106(); virtual void _v107();
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;            // slot 108 (+0x1B0), 0x00530EC0
    virtual void _v109(); virtual void _v110(); virtual void _v111(); virtual void _v112(); virtual void _v113(); virtual void _v114(); virtual void _v115(); virtual void _v116();
    virtual void _v117(); virtual void _v118(); virtual void _v119(); virtual void _v120(); virtual void _v121(); virtual void _v122(); virtual void _v123(); virtual void _v124();
    virtual void _v125(); virtual void _v126(); virtual void _v127(); virtual void _v128(); virtual void _v129(); virtual void _v130();
    virtual void ExecuteActions(std::list<CCountedPointer<CAction> >* actions);   // slot 131 (+0x20C), 0x005309B0
};
class CChangingStateComponent : public CComponent { public:
    unsigned char _pad04[0x14C - 4];
    unsigned long RequestedState;                               // +0x14C
    unsigned char _pad150[0xC];
};
class CHoverable : public CChangingStateComponent { public:
    unsigned long StateBeforeHovered;                           // +0x15C
    bool WasHovered;                                            // +0x160
    std::list<CCountedPointer<CAction> >* PActionsOnHovered;    // +0x164
    std::list<CCountedPointer<CAction> >* PActionsOnUnhovered;  // +0x168
    virtual void OnHovered();
};
}

void NUISystem::CHoverable::OnHovered()
{
    StateBeforeHovered = RequestedState;
    ChangeState(LoadAndReturnDef()->HoveredState);
    ExecuteActions(PActionsOnHovered);
}