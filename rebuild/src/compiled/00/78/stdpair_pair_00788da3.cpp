// std::pair::pair(const CIntelligentPointer<CThing>& __a, const long& __b) -- (<CIntelligentPointer<CThing>, long>) copy both members
// Leaf: ??0?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@QAE@ABV?$CIntelligentPointer@VCThing@@@@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00788DA3, 27 bytes, thiscall, `ret 8`.
// Identity (probable): the only caller is make_pair 0x00788EF5 (dead COMDAT) in the CTCMeleeTrap/CTCTrapBase module, whose element type pair<CIntelligentPointer<CThing>, long> is proven by the landed copy constructor 0x00788D88 (CTCTrapBase::HitThings, Ego_r.pdb vector<pair<CIntelligentPointer<CThing>, long> >); the first member's copy constructor 0x004A9730 is CIntelligentPointer<CThing>'s (RTTI of vtable 0x01238C6C).
// Callees: CIntelligentPointer<CThing> copy constructor 0x004A9730.
// Shape (family M068, representative 0x004851B6): STLport `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}`
//   in a size-optimised module (unaligned entry, `push [esp+8]` straight from the argument slot): `optimize("s")`.
#pragma optimize("s", on)
class CThing;
class CBaseObject;
class CBaseIntelligentPointer {                           // 8 bytes: vptr, item
public:
    virtual void v00();
    CBaseObject* PItem;                                   // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x004A9730 for CThing (RTTI vtable 0x01238C6C)
};
namespace std {
template <class _T1, class _T2> struct pair {
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template struct pair<CIntelligentPointer<CThing>, long >;
}