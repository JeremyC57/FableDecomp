// std::fill(CIVCountedPointer<CLocalDetailPlacementGrid>* first, CIVCountedPointer<CLocalDetailPlacementGrid>* last, const CIVCountedPointer<CLocalDetailPlacementGrid>& val) -- assign val to every intrusive pointer of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B000, 66 bytes, `ret 4`: __fastcall (first ecx -> esi, last edx -> ebx,
//   the value reference on the stack -> edi).
// Caller: 0x00A6BDA8 (the local-detail placement-grid vector fill). Callees: the grid's virtual destroy (vtable slot 1)
//   when its intrusive reference count (+4) drops to zero.
// Label: the manifest's `_Fill<CIVCountedPointer<CLocalDetailPlacementGrid>*, ...>` is the Dinkumware spelling; STLport 4.6
//   `fill`. Same body as the landed std::fill<CDataBankTextEntry> @ 0x00A43890 (CIVCountedPointer::operator= inline).
class CIVCountedPointeeBase {
public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04 IVRefCount
};

template <class T>
class CIVCountedPointer {
public:
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

class CLocalDetailPlacementGrid : public CIVCountedPointeeBase {
};

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(CIVCountedPointer<CLocalDetailPlacementGrid>*, CIVCountedPointer<CLocalDetailPlacementGrid>*,
                              const CIVCountedPointer<CLocalDetailPlacementGrid>&);
}   // namespace std