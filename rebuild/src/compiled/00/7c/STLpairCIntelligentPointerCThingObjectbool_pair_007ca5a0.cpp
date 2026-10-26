// std::pair<CIntelligentPointer<CThingObject>, bool>::pair(const CIntelligentPointer<CThingObject>& a, const bool& b) -- copy the intelligent pointer into `first` and the flag into `second`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CA5A0, length 46 bytes.
// Leaf: ??0?$pair
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50 (on `a`),
// SetPItem @ 0x00A01B90. Vtable 0x0125A858 = CIntelligentPointer<CThingObject> (landed sources).
// Label correction: the manifest's "CIntelligentPointer<const CThing>::CIntelligentPointer<const CThing>" is
// propagated. The body builds a pointer with the CIntelligentPointer<CThingObject> vtable at +0 and copies a
// byte argument read through a reference to +8 (`ret 8`): the STLport pair(const T1&, const T2&) of
// pair<CIntelligentPointer<CThingObject>, bool> -- the hero item-display queue element that the landed
// CTCHeroReceiveItems::GiveItems (0x007CA9E0) builds with `value_type(CIntelligentPointer<CThingObject>(item),
// false)`. The model is that landed row's (pair and pointer 1-aligned: PDB _Pair_base<..., bool> is 9 bytes).
// Shape: the base constructor, then the derived vtable store between the source load and the GetPItem call (the
// pointer copy constructor `SetPItem(other.GetPItem())`), then the flag copied through the reference.
class CThing;
class CThingObject;
class CBaseObject;

#pragma pack(push, 1)
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0125A858 for CThingObject
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;     // +0x00
    T2 second;    // +0x08
    pair(const T1& a, const T2& b);
};
template <class T1, class T2>
pair<T1, T2>::pair(const T1& a, const T2& b) : first(a), second(b) { }

template struct pair<CIntelligentPointer<CThingObject>, bool>;
}
#pragma pack(pop)