// std::fill(NGameText::CDataBankTextEntry* first, NGameText::CDataBankTextEntry* last, const NGameText::CDataBankTextEntry& val) -- assign val to every text-bank entry of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x00A43890, 80 bytes, `ret 4`: __fastcall (first ecx -> esi, last edx -> ebx, the value reference
// on the stack -> edi).
// Types (Ego_r.pdb): NGameText::CDataBankTextEntry (8 bytes) = PText CIVCountedPointer<NGameText::CTextEntry> +0,
//   NarratorIndex +4.
// Callees: the text entry's virtual destroy (vtable slot 1) when its reference count (+4) drops to zero.
// Shape: the entry's implicit copy assignment = the intrusive pointer's operator= then the index word. operator= tests
// the held object against the source first (`cmp ecx,[edi] ; je`), releases the old one through the local copy
// (`dec [ecx+4] ; jne ; call [eax+4]`, the member zeroed), then stores the new pointer and bumps its count when set.
// Local base model, not engine/CIVCountedPointeeBase.h: the generated header flattens the vptr into a plain
// `void* __vftable`, which cannot make the slot-1 virtual destroy call the release runs.
class CIVCountedPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template <class T>
class CIVCountedPointer { public:
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

namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { public: };
class CDataBankTextEntry { public:                     // 8 bytes
    CIVCountedPointer<CTextEntry> PText;        // +0x00
    long                          NarratorIndex; // +0x04
};
}
using NGameText::CDataBankTextEntry;

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CDataBankTextEntry*, CDataBankTextEntry*, const CDataBankTextEntry&);
}