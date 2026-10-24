// CTCHeroExperience::AddAttackOfTypeToAttackHistory(EDamageAttribute type) -- drop the oldest attack record and append {type, current world frame} to the attack history
// Leaf: ?AddAttackOfTypeToAttackHistory@
// Grade: DIFFER(91v89) -- register allocation only: our build loads `type` into edi between GetWorld and GetFrame and keeps
//        the frame in ebx (extra push/pop ebx); retail keeps only the frame in edi and reads `type` from its argument slot
//        at the node copy (`mov edx,[esp+0Ch]`). Tried: std::pair temporary, make_pair, a named `frame` local, a by-value
//        record constructor -- all identical.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D7EC0, length 89 bytes (+ int3 padding), thiscall `ret 4`.
// Ego_r.pdb `?AddAttackOfTypeToAttackHistory@CTCHeroExperience@@AAEXW4EDamageAttribute@@@Z`; single caller 0x006D879A
// (0x006D8790, the CTCHeroExperience strike handler in this chunk). The manifest label "CViewBase_LogMemoryEntry" is
// propagated from GetWorld's wrong "CViewBase::GetDisplayEngine" label.
// Callees: free @ 0x00BFEA14 (node deallocate), CTCBase::GetWorld() @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine"
// is propagated; it reads a global, not `this`), CWorld::GetFrame() @ 0x0049D870 (static; manifest
// "GFGetPreMainMemoryUsed" is propagated), malloc @ 0x00BFEA0E (new node).
// Layout: AttackHistory list<pair<EDamageAttribute, long>> at retail +0x50 (PDB +0x64; see the landed
// OnInitialActivate 0x006D8B00, which sizes it to ten records): the STLport list is its 4-byte sentinel pointer.
// Shape: `AttackHistory.pop_front(); AttackHistory.push_back(pair(type, GetWorld()->GetFrame()));` with STLport's
// pop_front -> erase(begin()) (unlink, free without a null test) and push_back -> insert(end(), x) (malloc, placement
// copy behind its null test, link before the sentinel) inline. GetFrame is static, so `GetWorld()->` only evaluates
// the call for its object (result discarded), as retail does.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};

template <class T> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    _Node* _M_node;                                        // the sentinel (alloc proxy)
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        _Construct(&p->_M_data, x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    iterator erase(iterator position)
    {
        _List_node_base* next_node = position._M_node->_M_next;
        _List_node_base* prev_node = position._M_node->_M_prev;
        _Node* n = (_Node*)position._M_node;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        free(n);
        return iterator(next_node);
    }
    void push_back(const T& x) { insert(end(), x); }
    void pop_front() { erase(begin()); }
};

}

struct CWorld {
    static long __fastcall GetFrame();                     // 0x0049D870
};

struct CTCHeroExperience {
    void*         __vftable;                               // +0x00
    unsigned char _pad_0x04[0x4C];                         // +0x04 CTCBase fields, experience counters, vectors, Pickups
    std::list<std::pair<EDamageAttribute, long> > AttackHistory;   // +0x50 (PDB +0x64)

    CWorld* GetWorld();                                    // 0x00686A70 (CTCBase)
    void AddAttackOfTypeToAttackHistory(EDamageAttribute type);
};

void CTCHeroExperience::AddAttackOfTypeToAttackHistory(EDamageAttribute type)
{
    AttackHistory.pop_front();
    AttackHistory.push_back(std::pair<EDamageAttribute, long>(type, GetWorld()->GetFrame()));
}
