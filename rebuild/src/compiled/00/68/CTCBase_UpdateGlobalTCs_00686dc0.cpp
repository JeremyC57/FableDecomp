// CTCBase::UpdateGlobalTCs(bool paused) -- run GlobalUpdate(paused) on every registered global-update TC that is not marked for removal, stepping the iterator before each call
// Leaf: ?UpdateGlobalTCs@CTCBase@@SIX_N@Z
//
// Retail 0x00686DC0, 46 bytes (+ int3 padding to 48), plain `ret` (static __fastcall: paused in cl, kept whole in edi).
// Callee: the TC's virtual GlobalUpdate(bool) (slot 26, `call [eax+68h]`; CTCInventoryExperience::GlobalUpdate
//   0x0056FC5D sits in slot 26 of its vtable 0x0124F11C).
// Data: the global-update TC list @ 0x013BA8AC (one-word STLport list head; its `??__E` initializer is the landed
//   0x0121E010) -- filled by CTCBase::AddToGlobalTCs / RemoveFromGlobalTCs (Ego_r name set); the static's own name is not
//   in the PDB name set, so it is descriptive.
// Identity: Ego_r.pdb ?UpdateGlobalTCs@CTCBase@@SIX_N@Z (static, __fastcall, bool); one caller 0x004A5CC0.
// Layout: Ego_r.pdb CTCBase (12 bytes): vptr, ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9.  engine/CTCBase.h
//   is data-only (its vptr a `void*`), so the class is modelled locally with the virtual slot.
// Shape: `for (it = list.begin(); it != list.end(); ) { CTCBase* tc = *it; ++it; if (!tc->ToBeRemoved)
//   tc->GlobalUpdate(paused); }` -- end() re-read from the global after each call.
class CThing;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                                            // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _Tp> class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;                               // the sentinel
    iterator begin() { return _M_node->_M_next; }
    iterator end() { return _M_node; }
};
}

class CTCBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25();
    virtual void GlobalUpdate(bool paused);                 // slot 26 (+0x68)

    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09

    static std::list<CTCBase*> GlobalUpdateTCs;             // 0x013BA8AC
    static void __fastcall UpdateGlobalTCs(bool paused);
};

void __fastcall CTCBase::UpdateGlobalTCs(bool paused)
{
    for (std::list<CTCBase*>::iterator it = GlobalUpdateTCs.begin(); it != GlobalUpdateTCs.end(); ) {
        CTCBase* tc = *it;
        ++it;
        if (!tc->ToBeRemoved)
            tc->GlobalUpdate(paused);
    }
}