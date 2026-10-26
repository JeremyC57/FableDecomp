// _global::UnguardedLinearInsert_PairAnimComponent_005e07f0(pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*, pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >, CKeyPairCompareLess) -- SGI __unguarded_linear_insert over the anim-component pairs, ordered by key
// Leaf: UnguardedLinearInsert_PairAnimComponent_005e07f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E07F0, length 141 bytes.
// Identity: the manifest's `_Copy_backward<NGameText::CDataBankTextEntry*>` label is propagated and wrong -- this is the
// SGI `__unguarded_linear_insert(last, T val, Compare comp)` (`ret 0Ch` = the 8-byte value + the 4-byte empty
// comparator). Its TU neighbours are the std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > sort helpers
// with CKeyPairCompareLess (0x005E0940 _Push_heap, 0x005E1210 _Adjust_heap, 0x005E0C00 _Dest_val of that pair), so the
// element is that pair: a signed enum key (`cmp ; jge/jl`) and a one-word intrusive counted pointer whose pointee keeps
// its reference count at +4 and is freed through virtual slot 1 (CIVCountedPointeeBase, modelled as the landed
// 0x0083CE60 / 0x00BBC310).
// Shape: /Gr TU (__fastcall, last in ecx). The pair's implicit operator= stores the key, then the inlined
// CIVCountedPointer::operator= (same-object skip; release the old pointee and clear the slot; adopt and add a
// reference). After `*last = val` the by-value `val` is destroyed by this callee (the pointer's release with the
// pointee as `this`: `mov ecx,edi ; call [eax+4]`).
// Callees: none direct (virtual slot 1 of the pointee).
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

struct CKeyPairCompareLess {                 // CKeyPairCompareLess<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >
    bool operator()(const PairAnimComponent& a, const PairAnimComponent& b) const { return a.first < b.first; }
};

void __fastcall UnguardedLinearInsert_PairAnimComponent_005e07f0(PairAnimComponent* last, PairAnimComponent val, CKeyPairCompareLess comp)
{
    PairAnimComponent* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}