// std::fill(CCountedPointer<C3DGroup2>* first, CCountedPointer<C3DGroup2>* last, const CCountedPointer<C3DGroup2>& value) -- assign value to every element
// Leaf: ??$fill@PAV?$CCountedPointer@VC3DGroup2@@@@V1@@std@@YIXPAV?$CCountedPointer@VC3DGroup2@@@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a95b00, 94 bytes, /Gr `__fastcall` (first ecx, last edx spilled, value on the stack), `ret 4`.
// Identity (probable): called by the fill insert 0x00A97F70 (landed under the stand-in N00a97f70::vector<Element>), whose copies go through __uninitialized_copy<CCountedPointer<C3DGroup2>*> 0x00A95820 (bound); reloc_bindings names this address only as the stand-in ??$fill@UElement@N00a97f70@@.
// Callee: operator delete 0x00BFE9BC (a released count block); CCountedPointer's operator= inline (the source's fields read first,
//   release the old block, adopt, add-ref).
class C3DGroup2;
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
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
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(CCountedPointer<C3DGroup2>*, CCountedPointer<C3DGroup2>*, const CCountedPointer<C3DGroup2>&);
}