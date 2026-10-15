// std::vector<void*>::erase(void**) -- STLport single-element erase for a vector of pointers: shifts the tail down over the erased slot (trivial memmove copy) unless it was the last one, drops the finish pointer by one element and returns the position
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485468, length 35 bytes. Callers: 0x0047E31D, 0x0053B3DC,
// 0x0053B507, 0x00577A48, 0x0057849A (the first erases an owned polymorphic pointer after deleting it).
// Callees: the trivial copy std::__copy_trivial @ 0x00405BA0 (`__fastcall` first/last in ecx/edx, the
// destination on the stack; memmove + returns the end; manifest "Std_Move_Backward").
// Label: the catalog name "CThreadedFile::FinishAndGetFileHandle" is propagated; the body is the STLport
// `vector<T*>::erase(iterator)` (_M_start +0, _M_finish +4, _M_end_of_storage +8).
// Shape: `pos + 1 != end()` compares the incremented position against the finish pointer read first, the
// copy reuses both registers as its fastcall arguments, `--_M_finish` is an `add [esi+4],-4` and the
// position comes back in eax.
void* __fastcall __copy_trivial(const void* first, const void* last, void* result);   // 0x00405BA0

namespace std {

template <class T>
struct vector {
    T* _M_start;                                   // +0x00
    T* _M_finish;                                  // +0x04
    T* _M_end_of_storage;                          // +0x08
    T* end() { return _M_finish; }
    T* erase(T* pos);
};

template <class T>
T* vector<T>::erase(T* pos)
{
    if (pos + 1 != end())
        __copy_trivial(pos + 1, _M_finish, pos);
    --_M_finish;
    return pos;
}

template struct vector<void*>;

}