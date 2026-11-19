// std::pair<CIntelligentPointer<CThing>,long>::pair(const std::pair<CIntelligentPointer<CThing>,float>&) -- converting pair constructor: copy the pointer, truncate the float
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788D68, length 32 bytes (catalogued as the propagated label
// "CTCMoneyReceiver::CTCMoneyReceiver"; its only caller @ 0x00788783 builds a {target, time} pair on the frame and
// converts it to {target, frame count} before inserting it -- the CTCMeleeTrap region).
// Leaf: ??$?0V?$CIntelligentPointer@VCThing@@@@M@?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@QAE@ABU?$pair@V?$CIntelligentPointer@VCThing@@@@M@1@@Z
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(const CIntelligentPointer&) @ 0x004A9730; __ftol2.
// NF95 2026-11-19 (q11276): the element is CIntelligentPointer<CThing> (0x004A9730 stores its vtable 0x01238C6C; Ego_r.pdb CTCTrapBase::HitThings
//   is vector<pair<CIntelligentPointer<CThing>, long> >; the landed caller 0x00788617 binds this std::pair spelling); was `<const CThing>`
//   on a global-namespace pair.
// Shape: the STL member-template converting constructor `first(p.first), second(p.second)`: the source address in
// edi for both members, the float converted through __ftol2 into +8 (CIntelligentPointer is 8 bytes: vptr + item).
class CThing;

template<class T> class CIntelligentPointer { public:
    virtual void v00();
    T* m_pItem;                                     // +0x04
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x004A9730 for <CThing>
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    template<class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second((T2)p.second) {}
};

}

template std::pair<CIntelligentPointer<CThing>, long>::pair(const std::pair<CIntelligentPointer<CThing>, float>&);