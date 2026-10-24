// std::list<T_0042ab1c>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0042A141) then free the sentinel node
// Leaf: ??1list_T_0042ab1c@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/42/CActiveFile_OnReadFinished_0042ab1c.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x0042A141) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder (size-optimised TU: pop ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AB1C, 23 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
struct list_T_0042ab1c {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x0042A141 (_List_base::clear, out of line)
    ~list_T_0042ab1c();
};
}
std::list_T_0042ab1c::~list_T_0042ab1c()
{
    clear();
    if (_M_node)
        free(_M_node);
}
