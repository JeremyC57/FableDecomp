// std::pair<CIntelligentPointer<CThing>, CUnprovenDword_004cbbb0>::pair(const pair& other) -- STLport pair copy constructor: copy the intelligent pointer (re-registering the item), then the 4-byte second
// Leaf: ??0?$pair@V?$CIntelligentPointer@VCThing@@@@VCUnprovenDword_004cbbb0@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CBBB0, 47 bytes, thiscall `ret 4`, returns this.
// Identity (descriptive second): `first` is CIntelligentPointer<CThing> -- the inline copy stores vtable 0x01238C6C, RTTI
//   .?AV?$CIntelligentPointer@VCThing@@@@ -- but no call, jump or reference reaches this COMDAT, so the 4-byte `second`
//   type is unproven (next to make_pair<CIntelligentPointer<CThing>, int> 0x004CBB80 (catalog), so int is the likeliest second).
// Callees: CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::GetPItem 0x00A01B50 (on other.first), SetPItem 0x00A01B90.
// Shape (family K115, representative 0x004CBBB0): STLport `pair(const pair& __o) : first(__o.first), second(__o.second) {}` with
//   the K108 __forceinline intelligent-pointer copy constructor; emitted by an explicit instantiation of the pair.
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
class CUnprovenDword_004cbbb0 {                                            // descriptive: the pair's 4-byte second
public:
    unsigned long Value;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x08
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template struct pair<CIntelligentPointer<CThing>, CUnprovenDword_004cbbb0>;
}