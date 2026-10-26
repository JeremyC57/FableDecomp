// std::pair<CIntelligentPointer<CThing>, long>::pair(const pair&) -- copy constructor: the first member through its out-of-line copy constructor, then the 4-byte second; returns this
// Leaf: ??0?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788D88, 27 bytes, thiscall `ret 4`.
// Identity (proven): std::pair<CIntelligentPointer<CThing>, long>::pair(const pair&); evidence: its callers are the out-of-line push_back 0x00789515 (0x0078952A), which CTCMeleeTrap::ProcessPossibleStrike calls on this+0xC = CTCTrapBase::HitThings (Ego_r.pdb vector<pair<CIntelligentPointer<CThing>, long> > at +0xC; 0x0078878C `lea ecx,[esi+0Ch]`) after the converting pair constructor 0x00788D68, and the vector's uninitialized_copy 0x00788F17 / fill_n 0x00788F42 / fill insert 0x007893AE; the first member's copy constructor is CIntelligentPointer<CThing>'s 0x004A9730 (RTTI of its vtable).
// Label: the manifest's "CComboMultiplierData" is propagated (family H044: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one record type).
// Callee: 0x004A9730 (the first member's out-of-line copy).
// Shape: `first(o.first), second(o.second)` (or the matching assignments): edi holds the source, esi `this`, the 4-byte
//   second member is one eax move after the call.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
class CThing;
class CBaseObject;
class CBaseIntelligentPointer {                           // 8 bytes: vptr, item
public:
    virtual void v00();
    CBaseObject* PItem;                                    // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other);              // 0x004A9730 for CThing (RTTI vtable 0x01238C6C)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760 for CThing
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template pair<CIntelligentPointer<CThing>, long>::pair(const pair<CIntelligentPointer<CThing>, long>&);
} // namespace std
