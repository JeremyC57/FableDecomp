// CCreatureActionBase::GetPActionFromEnum(EActionRegisteredType type, CThingCreatureBase& creature) -- clone the registered prototype action for the type, give it the creature and return it counted; a null counted pointer when the type is not registered
// Leaf: ?GetPActionFromEnum@CCreatureActionBase@@SI?AV?$CCountedPointer@VCCreatureActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@@Z
//
// Retail 0x00692E50, 105 bytes (+ int3 padding to 112), static __fastcall `ret 4` (the hidden CCountedPointer result
//   in ecx, the type in edx, the creature on the stack).
// Callees: the registry map's find(const EActionRegisteredType&) @ 0x00696680 (landed; iterator through a hidden
//   pointer, `ret 8`), the prototype's virtual Clone() const (slot 11, +0x2C), CCreatureActionBase::SetCreature @
//   0x006928C0 (landed as the intelligent-pointer assign to PCreature +0x08), CCountedPointer<CCreatureActionBase>::
//   CCountedPointer(CCreatureActionBase*) @ 0x00695880 (landed).
// Data: the registry std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > @ 0x013BA8C8 (header
//   pointer +0, node count +4; its static initialiser 0x0121E070 and atexit destructor 0x012292E0 are landed); a
//   node's value pair sits at +0x10, so the prototype's Data is node +0x14.
// Caller: 0x0066770E.
// Shape: `it = Registry.find(type); if (it == Registry.end()) return CCountedPointer<CCreatureActionBase>();
//   CCreatureActionBase* action = it->second->Clone(); action->SetCreature(creature); return
//   CCountedPointer<CCreatureActionBase>(action);` -- the type is homed in a frame slot for find's reference, the
//   empty pointer is the inline default ctor (two zero stores), the full one constructed in the result slot.
// Name: Ego_r.pdb `?GetPActionFromEnum@CCreatureActionBase@@SI?AV?$CCountedPointer@VCCreatureActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@@Z`
//   (exact symbol); the catalog's "GetPCombatActionFromEnum" is a near-miss label.  The registry's own name is not in
//   the PDB name sets (descriptive "Registry").
enum EActionRegisteredType { ACTION_REGISTERED_TYPE_0 = 0 };
class CThingCreatureBase;
class CCPPointerInfo;

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* item);                               // 0x00695880
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
    static CCountedPointer<CCreatureActionBase> __fastcall GetPActionFromEnum(EActionRegisteredType type,
                                                                              CThingCreatureBase& creature);
protected:
    void SetCreature(CThingCreatureBase& creature);         // 0x006928C0
    static std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > Registry;   // 0x013BA8C8
};

CCountedPointer<CCreatureActionBase> __fastcall CCreatureActionBase::GetPActionFromEnum(EActionRegisteredType type,
                                                                                        CThingCreatureBase& creature)
{
    std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >::iterator it = Registry.find(type);
    if (it == Registry.end())
        return CCountedPointer<CCreatureActionBase>();
    CCreatureActionBase* action = it->second->Clone();
    action->SetCreature(creature);
    return CCountedPointer<CCreatureActionBase>(action);
}