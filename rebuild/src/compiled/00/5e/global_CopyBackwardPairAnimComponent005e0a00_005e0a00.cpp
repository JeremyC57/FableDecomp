// _global::CopyBackward_PairAnimComponent_005e0a00(pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*, pair<..>*, pair<..>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over the anim-component pairs
// Leaf: CopyBackward_PairAnimComponent_005e0a00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0A00, length 94 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// >> 3, both pointers step back one element, the moved-back result is returned.
// Identity: the manifest's `_Copy_backward<NGameText::CDataBankTextEntry*>` label is propagated; the 8-byte element is
// std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > of the surrounding sort TU (see 0x005E07F0, the
// __unguarded_linear_insert of the same pair two functions up): the implicit pair operator= stores the key first, then
// the inlined intrusive CIVCountedPointer::operator= (same-object skip, release through virtual slot 1 at refcount 0 and
// clear, adopt + add a reference).
enum EAnimComponent { ANIM_COMPONENT_NONE = 0 };

class CAnimComponentBase { public:                  // CIVCountedPointeeBase-derived
    virtual void Destroy(int flags);         // slot 0
    virtual void Delete();                   // slot 1 (+0x04)
    unsigned long IVRefCount;                // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

struct CIVCountedPointer {                   // CIVCountedPointer<CAnimComponentBase>, 4 bytes
    CAnimComponentBase* Object;              // +0x00
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CAnimComponentBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct PairAnimComponent {                   // std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >, 8 bytes
    EAnimComponent    first;                 // +0x00
    CIVCountedPointer second;                // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairAnimComponent* __fastcall CopyBackward_PairAnimComponent_005e0a00(PairAnimComponent* first, PairAnimComponent* last, PairAnimComponent* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}