// NCameraModes::CGetAllThingsOfType::operator()(const CThing* thing) -- collect a thing into the functor's list when its def name equals the wanted one
// Leaf: ??RCGetAllThingsOfType@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00877110, length 80 bytes (`__fastcall`, `ret 4`).
// Identity: Ego_r.pdb `??RCGetAllThingsOfType@NCameraModes@@QAEXPBVCThing@@@Z` (non-const, void, const CThing*) and its
//   PDB layout CGetAllThingsOfType { char* DefName +0; list<const CThing*>* ListToChange +4 } (8 bytes) fit the body
//   exactly. No direct callers or data references (an unreferenced out-of-line copy; the camera-mode CTools
//   for_each users inline it). The catalog label "CThingList_AddThingByDefName" is an auto label.
// Callees: CThing::GetDefName() const @ 0x004C7CC0 (landed; CDefString by hidden pointer), CDefString::operator==(const
//   char*) const @ 0x005B3440 (landed), malloc @ 0x00BFEA0E (the 12-byte STLport list node).
// Shape: DefName is loaded and pushed before the GetDefName call, whose CDefString result is homed in the dead `thing`
//   argument slot (no destructor: CDefString's is trivial); STLport list::push_back(x) = insert(end(), x) as in the
//   landed NCameraModes::CBase::AddIncidentalAttractorLookAt 0x0084E2C0: sentinel read before the malloc, null-checked
//   placement copy, then the link with `prev` read after the node's value store.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }   // placement new (the null test on the node)

struct CDefString {
    long TablePos;                                      // +0
    CDefString(const CDefString& other);                // user copy ctor: returned through a hidden pointer
    bool operator==(const char* text) const;            // 0x005B3440
};

class CThing {
public:
    CDefString GetDefName() const;                      // 0x004C7CC0
};

namespace _STL {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <class T> struct _List_iterator {
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_node_base* _M_node;
};
template <class T> class list {                         // STLport list: one pointer to the sentinel node
public:
    typedef _List_iterator<T> iterator;
    iterator end() { return iterator(_M_node); }
    void insert(iterator position, const T& x)
    {
        _List_node<T>* node = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        ::new (&node->_M_data) T(x);
        _List_node_base* prev = position._M_node->_M_prev;
        node->_M_next = position._M_node;
        node->_M_prev = prev;
        prev->_M_next = node;
        position._M_node->_M_prev = node;
    }
    void push_back(const T& x) { insert(end(), x); }
    _List_node_base* _M_node;
};
}

namespace NCameraModes {
class CGetAllThingsOfType {                             // Ego_r.pdb, 8 bytes
public:
    char*                         DefName;              // +0x00
    _STL::list<const CThing*>*    ListToChange;         // +0x04
    void operator()(const CThing* thing);
};

void CGetAllThingsOfType::operator()(const CThing* thing)
{
    if (thing->GetDefName() == DefName)
        ListToChange->push_back(thing);
}
}