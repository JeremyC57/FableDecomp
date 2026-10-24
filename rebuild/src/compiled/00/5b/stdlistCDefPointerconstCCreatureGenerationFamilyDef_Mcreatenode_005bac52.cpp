// std::list<CDefPointer<const CCreatureGenerationFamilyDef> >::_M_create_node(const CDefPointer<const CCreatureGenerationFamilyDef>&) -- STLport list node factory: malloc a 12-byte node and copy-construct the def pointer into it (bumping the def's reference count)
// Leaf: _M_create_node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005bac52, 33 bytes (`ret 4`).
// Callee: malloc 0x00BFEA0E (STLport's node allocator), cdecl `pop ecx` cleanup.
// Identity: the catalog label `_Cons_val<allocator<CDefPointer<..>>,..>` names only the inlined
// construct step.  The only caller, 0x005BB0A7, is the list insert(where, value): it calls this with
// `this` still in ecx, then links the result before `where` (node+0 = where, node+4 = where->prev),
// i.e. an STLport _List_node {_M_next +0, _M_prev +4, _M_data +8}.  `this` itself is unused here.
// Shape: the unaligned address and `push 0Ch ; call ; ... pop ecx` mark a /Os TU
// (`#pragma optimize("s", on)`); `lea edx,[eax+8] ; test edx,edx` is the placement new's null test on
// &node->_M_data; the CDefPointer copy constructor is `Object = other.Object; if (Object)
// ++Object->m_refCount` (+4), as in the landed CThingSwitch::GetSwitchDef 0x0083C1E0.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableList_005bac52 {

struct CCreatureGenerationFamilyDef {
    void* __vftable;                                       // +0x00
    long m_refCount;                                       // +0x04
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};

typedef CDefPointer<CCreatureGenerationFamilyDef> CFamilyDefPointer;

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x00
    _List_node_base* _M_prev;                              // +0x04
};

struct _List_node : _List_node_base {
    CFamilyDefPointer _M_data;                             // +0x08
};

struct list {
    _List_node_base* _M_node;
    _List_node* _M_create_node(const CFamilyDefPointer& x);
};

_List_node* list::_M_create_node(const CFamilyDefPointer& x)
{
    _List_node* p = (_List_node*)malloc(sizeof(_List_node));
    new (&p->_M_data) CFamilyDefPointer(x);
    return p;
}

}