// CCombatActionBase::GetPCombatActionFromEnum(EActionRegisteredType type, CThingCreatureBase& creature, CThingCreatureBase& target) -- clone the registered prototype for the type as a combat action, give it its creature and target and return it counted; a null counted pointer when the type is not registered, its prototype is null or the clone fails
// Leaf: ?GetPCombatActionFromEnum@CCombatActionBase@@SI?AV?$CCountedPointer@VCCombatActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00857430, length 148 bytes (+12 int3). Static `__fastcall`, `ret 8` (the hidden
//   CCountedPointer result in ecx, the type in edx, creature and target on the stack).
// Callees: the action registry's find(const EActionRegisteredType&) @ 0x00696680 (iterator through a hidden pointer),
//   the prototype's virtual Clone() const (slot 11, +0x2C), CCreatureActionBase::SetCreature @ 0x006928C0,
//   CCombatActionBase::SetTarget(CThing&) @ 0x0085AA90 (Ego_r.pdb names it; the landed body stores the target into the
//   intelligent pointer at +0xA8; manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is a propagated twin),
//   CCountedPointer<CCombatActionBase>::CCountedPointer(CCombatActionBase*) @ 0x0085A840 (landed).
// Data: the registry std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > @ 0x013BA8C8 shared with
//   CCreatureActionBase::GetPActionFromEnum (landed 0x00692E50, the one-argument sibling this body extends): header pointer +0,
//   node value pair +0x10, so the prototype's Data is node +0x14.
// Shape: the end test and the null-prototype test are two separate early returns, which VC merges into one null block on
//   the fall-through of `jne` (one `||` condition moves that block to the end of the function); the clone's null test keeps
//   the success path on the fall-through with a second null return (after `pop edi`) at the end.
// Name: the catalog row already says GetPCombatActionFromEnum; the Ego_r.pdb symbol above has the two creature references.
enum EActionRegisteredType { ACTION_REGISTERED_TYPE_0 = 0 };
class CThing;
class CThingCreatureBase;
class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* item);                               // 0x0085A840 for CCombatActionBase
    T* operator->() const { return Data; }
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};

template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    class _Node {
    public:
        char        _M_color;                               // +0x00
        _Node*      _M_parent;                              // +0x04
        _Node*      _M_left;                                // +0x08
        _Node*      _M_right;                               // +0x0C
        value_type  _M_value_field;                         // +0x10
    };
    class iterator {
    public:
        _Node* _M_node;
        iterator() { }
        iterator(_Node* node) : _M_node(node) { }
        value_type* operator->() const { return &_M_node->_M_value_field; }
        bool operator==(const iterator& other) const { return _M_node == other._M_node; }
    };
    _Node*       _M_header;                                 // +0x00
    unsigned int _M_node_count;                             // +0x04
    iterator end() { return iterator(_M_header); }
    iterator find(const _Key& key);                         // 0x00696680
};
}

class CCreatureActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CCreatureActionBase* Clone() const;             // slot 11 (+0x2C)
protected:
    void SetCreature(CThingCreatureBase& creature);         // 0x006928C0
    static std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > Registry;   // 0x013BA8C8
};

class CCombatActionBase : public CCreatureActionBase {
public:
    void SetTarget(CThing& target);                         // 0x0085AA90
    static CCountedPointer<CCombatActionBase> __fastcall GetPCombatActionFromEnum(EActionRegisteredType type,
                                                                                  CThingCreatureBase& creature,
                                                                                  CThingCreatureBase& target);
};
class CThing { };
class CThingCreatureBase : public CThing { };

CCountedPointer<CCombatActionBase> __fastcall CCombatActionBase::GetPCombatActionFromEnum(EActionRegisteredType type,
                                                                                          CThingCreatureBase& creature,
                                                                                          CThingCreatureBase& target)
{
    std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >::iterator it = Registry.find(type);
    if (it == Registry.end())
        return CCountedPointer<CCombatActionBase>();
    if (!it->second.Data)
        return CCountedPointer<CCombatActionBase>();
    CCombatActionBase* action = (CCombatActionBase*)it->second->Clone();
    if (action) {
        action->SetCreature(creature);
        action->SetTarget(target);
        return CCountedPointer<CCombatActionBase>(action);
    }
    return CCountedPointer<CCombatActionBase>();
}