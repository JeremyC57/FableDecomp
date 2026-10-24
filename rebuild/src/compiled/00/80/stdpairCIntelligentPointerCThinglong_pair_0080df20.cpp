// std::pair<CIntelligentPointer<CThing>,long>::pair(const CIntelligentPointer<CThing>& a, const long& b) -- STLport pair two-argument constructor
// Leaf: ??0?$pair@
// Retail: Fable.exe @ 0x0080DF20, 46 bytes, `ret 8`: __fastcall, this -> esi, returns this.
// No callers (unreferenced COMDAT). The catalog label make_pair<CIntelligentPointer<CThing>,int> is propagated: the
// body is not make_pair (that one, 0x0080E200, takes a hidden result pointer on the stack) but the pair constructor
// make_pair inlines -- `this` in ecx, both arguments by reference, `ret 8`, `mov eax,esi` at the end.
// Byte twin of 0x0080DEF0 (modelled as the pair<CIntelligentPointer<CThing>,int> instantiation); this is the ,long one (Ego_r.pdb has exactly these two
// pair<CIntelligentPointer<CThing>,int/long> constructors); which twin is the int one is not provable from the bytes.
// Shape: first(a) is CIntelligentPointer<CThing>'s copy constructor inlined: CBaseIntelligentPointer() @ 0x00A01B10,
// the derived vtable 0x01238C6C (RTTI .?AV?$CIntelligentPointer@VCThing@@@@), SetPItem(a.GetPItem())
// @ 0x00A01B50/0x00A01B90 (the landed out-of-line copy is 0x0080DEC0); then the 4-byte second.
// Layout: CBaseIntelligentPointer is 8 bytes (vptr + PData, Ego_r.pdb), so `second` sits at +8.
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
    CBaseObject* PData;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                  // this instantiation's vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                            // +0x00
    T2 second;                                           // +0x08
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
}
template struct std::pair<CIntelligentPointer<CThing>, long>;