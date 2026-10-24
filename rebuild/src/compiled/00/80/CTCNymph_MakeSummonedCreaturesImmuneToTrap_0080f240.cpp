// CTCNymph::MakeSummonedCreaturesImmuneToTrap(CTCTrapBase* trap) -- make every live summoned creature immune to the given trap
// Leaf: ?MakeSummonedCreaturesImmuneToTrap@CTCNymph@@SIXPAVCTCTrapBase@@@Z
// Retail: Fable.exe @ 0x0080F240, 46 bytes (+2 int3), static `__fastcall` (trap in ecx -> ebx), plain `ret`.
// Name: FableWin.pdb / Ego_r.pdb `?MakeSummonedCreaturesImmuneToTrap@CTCNymph@@SIXPAVCTCTrapBase@@@Z`; caller 0x007BDCD2.
// Callees: CTCAISummon::GetAllSummonedCreatures() 0x007FDEB0 (static __fastcall, FableWin.pdb: returns
//   list<CIntelligentPointer<CThing> >&), CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCTrapBase::SetThingAsImmune(
//   CThing&) 0x0078839A.
// Shape: STLport list walk (the 4-byte list holds the sentinel node pointer; node: next +0, prev +4, value +8) -- the end
//   node is re-read from the list every pass (`cmp esi,[edi]`), the item passed by reference through `*GetPItem()`.
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    CBaseObject* PData;                                      // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
namespace std {
template <class T> struct _List_node {
    _List_node<T>* _M_next;                                  // +0x00
    _List_node<T>* _M_prev;                                  // +0x04
    T _M_data;                                               // +0x08
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) { }
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator<T>& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator<T>& o) const { return _M_node != o._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;                                  // +0x00 sentinel (retail lists are 4 bytes)
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
}
class CTCAISummon {
public:
    static std::list<CIntelligentPointer<CThing> >& __fastcall GetAllSummonedCreatures();   // 0x007FDEB0
};
class CTCTrapBase {
public:
    void SetThingAsImmune(CThing& thing);                    // 0x0078839A
};
class CTCNymph {
public:
    static void __fastcall MakeSummonedCreaturesImmuneToTrap(CTCTrapBase* trap);
};
void __fastcall CTCNymph::MakeSummonedCreaturesImmuneToTrap(CTCTrapBase* trap)
{
    std::list<CIntelligentPointer<CThing> >& creatures = CTCAISummon::GetAllSummonedCreatures();
    for (std::list<CIntelligentPointer<CThing> >::iterator it = creatures.begin(); it != creatures.end(); ++it)
        trap->SetThingAsImmune(*it->GetPItem());
}