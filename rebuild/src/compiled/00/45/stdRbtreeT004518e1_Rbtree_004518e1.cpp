// std::_Rb_tree<T_004518e1>::~_Rb_tree() -- STLport red-black tree (map/set) destructor: clear the nodes (out-of-line
// _Rb_tree::clear @ 0x00451046) then free the header node
// Leaf: ??1_Rb_tree_T_004518e1@std@@
// Renamed (rename1, 2026-10-24) from std::list<T_004518e1>::~list (fistp pass; originally CFileInstaller::CActiveFile::
//   OnReadFinished) @ 00/45/std_list_T_004518e1_dtor_004518e1.cpp: the out-of-line clear it calls, 0x00451046 (landed
//   _global::RbTree_Clear_00451046), is STLport _Rb_tree::clear -- `if (_M_node_count (+4) != 0) { _M_erase(root =
//   header->_M_parent (+4)); leftmost (+8) = header; root = 0; rightmost (+0xc) = header; count = 0; }` -- not a list
//   clear, so this is the tree's destructor (`clear(); deallocate(header)`); its caller 0x0045C8D3
//   (CSpecialEffectsDef_TransferMap) uses it as a map destructor (FINDINGS chunk 368). Key/value types not recoverable
//   from the bytes: descriptive placeholder.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004518E1, 23 bytes: `mov esi,ecx ; call clear (0x00451046) ; if (header) free(header)`
//   (size-optimised TU: pop ecx).
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
struct _Rb_tree_T_004518e1 {
    void*         _M_header;                         // +0x00 the header node (STLport _Rb_tree_base)
    unsigned long _M_node_count;                     // +0x04
    void clear();                                    // 0x00451046 (_Rb_tree::clear, out of line)
    ~_Rb_tree_T_004518e1();
};
}
std::_Rb_tree_T_004518e1::~_Rb_tree_T_004518e1()
{
    clear();
    if (_M_header)
        free(_M_header);
}
