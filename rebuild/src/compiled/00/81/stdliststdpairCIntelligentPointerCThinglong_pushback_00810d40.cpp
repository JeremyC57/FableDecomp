// std::list<std::pair<CIntelligentPointer<CThing>,long> >::push_back(const std::pair<CIntelligentPointer<CThing>,long>& x) -- STLport list append: insert x before the sentinel
// Leaf: ?push_back@?$list@
// Retail: Fable.exe @ 0x00810D40, 25 bytes, `ret 4`: __fastcall (this passed through in ecx), the value reference on the
//   stack.
// Caller: 0x00810855. Callee: list::insert(iterator pos, const T& x) 0x00810C20 (thiscall, hidden iterator result;
//   it mallocs a 0x14-byte node and copy-constructs the pair at +8).
// Identity: Ego_r.pdb has `?push_back@?$list@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@...` (the node is 0x14 bytes:
//   next, prev, the 12-byte pair). Label: catalogued `std_vector_PushBackIntelligent` (a list, not a vector).
// Shape: STLport `void push_back(const T& x) { insert(end(), x); }` -- end() is copied into its argument slot by the
//   iterator's user copy constructor (`push ecx ; mov eax,esp ; mov [eax],edx`), the discarded result iterator lands in
//   the dead x argument slot (`lea eax,[esp+0Ch]`).
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                      // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                                // +0x00
    T2 second;                                               // +0x08
};
template <class T> struct _List_node {
    _List_node<T>* _M_next;                                  // +0x00
    _List_node<T>* _M_prev;                                  // +0x04
    T _M_data;                                               // +0x08
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) { }
    _List_iterator(const _List_iterator<T>& o) : _M_node(o._M_node) { }
};
template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;                                  // +0x00 sentinel (retail lists are 4 bytes)
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator position, const T& x);          // 0x00810C20
    void push_back(const T& x) { insert(end(), x); }
};
template void list<pair<CIntelligentPointer<CThing>, long> >::push_back(const pair<CIntelligentPointer<CThing>, long>&);
}