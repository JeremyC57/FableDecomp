// _global::CopyBackward_PairLongThing_006a2eb0(pair<long,CIntelligentPointer<CThing> >*, pair<..>*, pair<..>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 12-byte (long, intelligent pointer) pairs
// Leaf: CopyBackward_PairLongThing_006a2eb0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A2EB0, length 80 bytes.
// SGI-STL random-access `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the moved-back result is returned.
// Element: std::pair<long, CIntelligentPointer<CThing> > (the manifest label; 12 bytes = the key + the 8-byte
// vtable/item pointer). The implicit operator= copies the key, then PThing = other as SetPItem(other.GetPItem())
// (CBaseIntelligentPointer @ 0x00A01B50 / 0x00A01B90, modelled as the landed 0x0080AA60).
class CBaseObject;

struct CBaseIntelligentPointer {             // 8 bytes: vtable + item
    virtual void v00();
    void* PData;                             // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing;

struct PairLongThing {                       // std::pair<long, CIntelligentPointer<CThing> >
    long                        first;       // +0x00
    CIntelligentPointer<CThing> second;      // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairLongThing* __fastcall CopyBackward_PairLongThing_006a2eb0(PairLongThing* first, PairLongThing* last, PairLongThing* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}