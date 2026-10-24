// std::list<T_0042b755>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0042ABE1) then free the sentinel node
// Leaf: ??1list_T_0042b755@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/42/CActiveFile_OnReadFinished_0042b755.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x0042ABE1) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder (size-optimised TU: pop ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B755, 23 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
struct list_T_0042b755 {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x0042ABE1 (_List_base::clear, out of line)
    ~list_T_0042b755();
};
}
std::list_T_0042b755::~list_T_0042b755()
{
    clear();
    if (_M_node)
        free(_M_node);
}
