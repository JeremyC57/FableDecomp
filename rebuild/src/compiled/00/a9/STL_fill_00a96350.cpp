// std::fill(CCountedPointer<C3DGroup2>* first, CCountedPointer<C3DGroup2>* last, const CCountedPointer<C3DGroup2>& value) -- assign one counted pointer to every slot of [first, last), releasing each slot's old reference
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96350, 94 bytes, `__fastcall` (first ecx, last edx, value on the stack),
//   `ret 4`.
// Identity: the only call site 0x00A97F21 is the overlap arm of vector<CCountedPointer<..> >::_M_fill_insert 0x00A97E40
//   (`fill(pos, pos + n, x_copy)` after the x_copy counted copy and the uninitialized_copy/copy_backward calls). The
//   shape is fill, not the catalog's propagated `_Copy_backward<CCountedPointer<C3DGroup2>*>`; the pointee type is
//   kept from that label (the bytes cannot name it).
// Callees: the count block's destroy hook (`mov ecx,[info+8] ; call [info+4]`, a __fastcall DeleteData), operator delete
//   0x00BFE9BC.
// Shape: SGI `for (; first != last; ++first) *first = value;` (`last` spilled to the `push ecx` frame word, reloaded after
//   the release call) with CCountedPointer::operator= inline exactly as in the landed counted-pointer models: read
//   other.Info/other.Data first, skip when Info is unchanged, otherwise release the old block (decrement, re-read,
//   destroy + delete at zero), store both words and bump the new count.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

struct CCPPointerInfo {
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void*         Data;                                 // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
};

class C3DGroup2;

namespace std {

template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill<CCountedPointer<C3DGroup2>*, CCountedPointer<C3DGroup2> >(
    CCountedPointer<C3DGroup2>* __first, CCountedPointer<C3DGroup2>* __last, const CCountedPointer<C3DGroup2>& __val);
}