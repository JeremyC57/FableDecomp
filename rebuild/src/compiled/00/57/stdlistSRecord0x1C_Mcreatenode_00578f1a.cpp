// std::list<SRecord0x1C>::_M_create_node(const SRecord0x1C&) -- STLport list node factory: malloc a 0x24-byte node and copy a 28-byte POD record into it
// Leaf: _M_create_node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00578f1a, 31 bytes (`ret 4`).
// Callee: malloc 0x00BFEA0E (STLport's node allocator), cdecl `pop ecx` cleanup.
// Identity: the catalog label `_Cons_val<allocator<CAppearanceModifierGraphics::CEntry>,..>` names
// only the inlined construct step, and its element type does not fit: no Ego_r.pdb CEntry is 0x1C
// bytes.  The only caller, 0x0057929A, is the list insert(where, value) (`this` left in ecx; it links
// the node before `where`: node+0 = where, node+4 = where->prev), i.e. an STLport _List_node
// {_M_next, _M_prev, _M_data +8} -- the same shape as 0x005BAC52.
// Shape: /Os TU (unaligned address, `pop ecx`, `push 7 ; pop ecx` count): `#pragma optimize("s", on)`;
// the placement new null-tests &node->_M_data, and the POD element copies as one `rep movsd`.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableList_00578f1a {

struct SRecord0x1C {                                       // 28-byte POD element (type not identified)
    long m_words[7];
};

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x00
    _List_node_base* _M_prev;                              // +0x04
};

struct _List_node : _List_node_base {
    SRecord0x1C _M_data;                                   // +0x08
};

struct list {
    _List_node_base* _M_node;
    _List_node* _M_create_node(const SRecord0x1C& x);
};

_List_node* list::_M_create_node(const SRecord0x1C& x)
{
    _List_node* p = (_List_node*)malloc(sizeof(_List_node));
    new (&p->_M_data) SRecord0x1C(x);
    return p;
}

}