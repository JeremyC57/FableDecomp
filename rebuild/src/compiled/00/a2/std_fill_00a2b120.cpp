// std::fill(CCountedPointer<CMeshDataBankEntry>* first, CCountedPointer<CMeshDataBankEntry>* last, const CCountedPointer<CMeshDataBankEntry>& value) -- assign value to every element
// Leaf: ??$fill@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@V1@@std@@YIXPAV?$CCountedPointer@VCMeshDataBankEntry@@@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a2b120, 94 bytes, /Gr `__fastcall` (first ecx, last edx spilled, value on the stack), `ret 4`.
// Identity (proven): called by vector<CCountedPointer<CMeshDataBankEntry> >::_M_fill_insert 0x00A2D960 (bound in reloc_bindings).
// Callee: operator delete 0x00BFE9BC (a released count block); CCountedPointer's operator= inline (the source's fields read first,
//   release the old block, adopt, add-ref).
class CMeshDataBankEntry;
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
template void __fastcall fill(CCountedPointer<CMeshDataBankEntry>*, CCountedPointer<CMeshDataBankEntry>*, const CCountedPointer<CMeshDataBankEntry>&);
}