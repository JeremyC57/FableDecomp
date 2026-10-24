// std::list<T_005932a3>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x00592CCD) then free the sentinel node
// Leaf: ??1list_T_005932a3@std@@
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/59/CActiveFile_OnReadFinished_005932a3.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x00592CCD) ; if (sentinel) free(sentinel)`; the node type T is not recoverable from the bytes (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins); descriptive placeholder (size-optimised TU: pop ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005932A3, 23 bytes.
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace std {
struct list_T_005932a3 {
    void* _M_node;                                   // +0x00 the sentinel node (STLport _List_base)
    void clear();                                    // 0x00592CCD (_List_base::clear, out of line)
    ~list_T_005932a3();
};
}
std::list_T_005932a3::~list_T_005932a3()
{
    clear();
    if (_M_node)
        free(_M_node);
}
