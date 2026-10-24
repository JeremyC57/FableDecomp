// std::list<T_004dd2b1>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x004DB91F) then free the sentinel node
// Leaf: ??1list_T_004dd2b1@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/4d/CActiveFile_OnReadFinished_004dd2b1.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x004DB91F) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder (size-optimised TU: pop ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD2B1, 23 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
struct list_T_004dd2b1 {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x004DB91F (_List_base::clear, out of line)
    ~list_T_004dd2b1();
};
}
std::list_T_004dd2b1::~list_T_004dd2b1()
{
    clear();
    if (_M_node)
        free(_M_node);
}
