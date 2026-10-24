// std::list<T_005f900a>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x005F8B7D) then free the sentinel node
// Leaf: ??1list_T_005f900a@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/5f/CActiveFile_OnReadFinished_005f900a.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x005F8B7D) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder (size-optimised TU: pop ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F900A, 23 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
struct list_T_005f900a {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x005F8B7D (_List_base::clear, out of line)
    ~list_T_005f900a();
};
}
std::list_T_005f900a::~list_T_005f900a()
{
    clear();
    if (_M_node)
        free(_M_node);
}
