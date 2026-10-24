// std::list<T_00b3a8c0>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x00B3A760) then free the sentinel node
// Leaf: ??1list_T_00b3a8c0@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/b3/CActiveFile_OnReadFinished_00b3a8c0.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x00B3A760) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3A8C0, 25 bytes.

extern "C" void __cdecl free(void*);
namespace std {
struct list_T_00b3a8c0 {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x00B3A760 (_List_base::clear, out of line)
    ~list_T_00b3a8c0();
};
}
std::list_T_00b3a8c0::~list_T_00b3a8c0()
{
    clear();
    if (_M_node)
        free(_M_node);
}
