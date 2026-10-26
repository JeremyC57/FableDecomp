// std::pair<CIntelligentPointer<CThing>, int>::pair(const CIntelligentPointer<CThing>& a, const int& b) -- STLport: copy-construct the intelligent pointer (register with the pointed-to thing) and the int
// Leaf: ??0?$pair@V?$CIntelligentPointer@VCThing@@@@H@std@@QAE@ABV?$CIntelligentPointer@VCThing@@@@ABH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CBB80, length 48 bytes (46 + int3 padding), `ret 8` (thiscall: a, b), returns this.
// Identity: a thiscall constructor taking two references (no hidden result pointer, `this` returned), so not make_pair
// itself but the STLport `pair(const _T1&, const _T2&)` that make_pair<CIntelligentPointer<CThing>, int> builds; no direct
// callers (an emitted COMDAT). The catalog "make_pair<CIntelligentPointer<CThing>,int>" names its user.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem() const @ 0x00A01B50,
// SetPItem(CBaseObject*) @ 0x00A01B90 (landed names); CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: `first(a), second(b)`; the intelligent pointer's copy ctor is the landed `CBaseIntelligentPointer() {
// SetPItem(other.GetPItem()); }` (the argument is loaded before the vtable store, as retail).
class CThing;

class CBaseObject;
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C (T = CThing)
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                             // +0x00
    T2 second;                                            // +0x08
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};

template struct pair<CIntelligentPointer<CThing>, int>;
}