// std::pair<CIntelligentPointer<CThing>, long>::operator=(const pair&) -- copy assignment: the first member through its out-of-line operator=, then the 4-byte second; returns *this
// Leaf: ??4?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@QAEAAU01@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788DF0, 27 bytes, thiscall `ret 4`.
// Identity (proven): std::pair<CIntelligentPointer<CThing>, long>::operator=(const pair&); evidence: its only caller is the element copy loop 0x00788F76 (0x00788F8F, 12-byte stride) behind the HitThings vector's erase 0x00789085 (CTCTrapBase::FrameUpdate 0x007888F9) and CTCDExplosion's vector assignment 0x0080E4E0: assignment, not construction (the callee 0x004A9760 assigns the pointer; the twin copy constructor is 0x00788D88).
// Label: the manifest's "CComboMultiplierData" is propagated (family H044: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one record type).
// Callee: 0x004A9760 (the first member's out-of-line copy).
// Shape: `first(o.first), second(o.second)` (or the matching assignments): edi holds the source, esi `this`, the 4-byte
//   second member is one eax move after the call.
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
    pair& operator=(const pair& __o) { first = __o.first; second = __o.second; return *this; }   // the implicit assignment, spelled out
};
template pair<CIntelligentPointer<CThing>, long>& pair<CIntelligentPointer<CThing>, long>::operator=(const pair<CIntelligentPointer<CThing>, long>&);
} // namespace std
