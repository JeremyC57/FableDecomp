// CTCKraken::KillTentacles() -- kill every tentacle thing the kraken spawned, popping each off the back of its Tentacles list, then park the attacking-tentacle cursor at end()
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the last tentacle is read straight off the head node (`((_List_node<T>*)Tentacles._M_node->_M_prev)->_M_data.GetPItem()`), which loads the node into ecx and adds the data offset in place as retail does (`mov ecx,[eax+4] ; add ecx,8`); the iterator-based back() forms all give `lea ecx,[eax+8]`
// Leaf: ?KillTentacles@CTCKraken@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FB1C0, 106 bytes (+ int3 padding). Ego_r.pdb / FableWin.pdb
//   `?KillTentacles@CTCKraken@@AAEXXZ` (private thiscall); called by CTCKraken::OnDie 0x007FB230, SetMode 0x007FC730 and
//   EndAttackBeam 0x007FC800 (tail jump). Catalog label `LinkedList_KillAllThings` was descriptive.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, the list element's virtual
//   destructor (vtable slot 0, flag 0 = STLport _Destroy), free @ 0x00BFEA14 (the STLport node deallocate).
// Layout (retail; STLport list = one head-node word, so CTCKraken sits 8 below Ego_r.pdb from +0x30, as the landed
//   SetPhase 0x007FC820 / RaiseSunkenTentacles 0x007FB610): Tentacles +0x44, AttackingTentacleIterator +0x48.
// Shape: `while (Tentacles.size() > 0)` (STLport's walking size, unsigned `> 0` = `test ; jbe`), the last tentacle read straight off the head node's
//   _M_prev (an iterator-based back(), kept below for reference, gives `lea ecx,[eax+8]` instead of retail's `add ecx,8`),
//   pop_back = `erase(--end())` with the STLport erase body (prev/next relink, _Destroy, deallocate).
extern "C" void __cdecl free(void* p);                          // 0x00BFEA14

class CBaseObject { };
class CThing : public CBaseObject {
public:
    void Kill(bool immediately);                                // 0x004C9B80
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                         // slot 0
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                              // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
typedef CIntelligentPointer<CThing> CIntelligentPointer_CThing;

struct _List_node_base {                                       // STLport: the links live in a non-template base
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <class T> struct CListIterator {
    _List_node_base* _M_node;
    CListIterator() { }
    CListIterator(_List_node_base* n) : _M_node(n) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    CListIterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> struct CRetailList {                         // STLport list (4 bytes: the head node)
    _List_node<T>* _M_node;
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    T& back()
    {
        CListIterator<T> tmp = end();
        return *(--tmp);
    }
    CListIterator<T> erase(CListIterator<T> position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node<T>* n = (_List_node<T>*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        _Destroy(&n->_M_data);
        free(n);
        return CListIterator<T>(next);
    }
    void pop_back()
    {
        CListIterator<T> tmp = end();
        erase(--tmp);
    }
};

class CTCKraken {
public:
    void*                                    __vfptr;           // +0x00
    CThing*                                  ParentThing;       // +0x04 (CTCBase)
    unsigned char                            _pad08[0x44 - 0x08];
    CRetailList<CIntelligentPointer_CThing>  Tentacles;         // +0x44
    CListIterator<CIntelligentPointer_CThing> AttackingTentacleIterator;   // +0x48
private:
    void KillTentacles();
public:
    void OnDie() { KillTentacles(); }
};

void CTCKraken::KillTentacles()
{
    while (Tentacles.size() > 0) {
        CThing* tentacle = ((_List_node<CIntelligentPointer_CThing>*)Tentacles._M_node->_M_prev)->_M_data.GetPItem();
        if (tentacle)
            tentacle->Kill(true);
        Tentacles.pop_back();
    }
    AttackingTentacleIterator = Tentacles.end();
}