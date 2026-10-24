// std::list<T_004302cd>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0043027A) then free the sentinel node
// Leaf: ??1list_T_004302cd@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/43/CActiveFile_OnReadFinished_004302cd.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x0043027A) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder (size-optimised TU: pop ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004302CD, 23 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
struct list_T_004302cd {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x0043027A (_List_base::clear, out of line)
    ~list_T_004302cd();
};
}
std::list_T_004302cd::~list_T_004302cd()
{
    clear();
    if (_M_node)
        free(_M_node);
}
