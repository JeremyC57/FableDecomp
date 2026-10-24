// std::list<T_00440650>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0043FE40) then free the sentinel node
// Leaf: ??1list_T_00440650@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/44/CActiveFile_OnReadFinished_00440650.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x0043FE40) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x00440650, 25 bytes.

extern "C" void __cdecl free(void*);
namespace std {
struct list_T_00440650 {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x0043FE40 (_List_base::clear, out of line)
    ~list_T_00440650();
};
}
std::list_T_00440650::~list_T_00440650()
{
    clear();
    if (_M_node)
        free(_M_node);
}
