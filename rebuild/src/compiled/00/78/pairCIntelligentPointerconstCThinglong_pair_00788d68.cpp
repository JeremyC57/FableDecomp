// pair<CIntelligentPointer<const CThing>,long>::pair(const pair<CIntelligentPointer<const CThing>,float>&) -- converting pair constructor: copy the pointer, truncate the float
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788D68, length 32 bytes (catalogued as the propagated label
// "CTCMoneyReceiver::CTCMoneyReceiver"; its only caller @ 0x00788783 builds a {target, time} pair on the frame and
// converts it to {target, frame count} before inserting it -- the CTCMeleeTrap region).
// Leaf: pair
// Callees: CIntelligentPointer<const CThing>::CIntelligentPointer(const CIntelligentPointer&) @ 0x004A9730; __ftol2.
// Shape: the STL member-template converting constructor `first(p.first), second(p.second)`: the source address in
// edi for both members, the float converted through __ftol2 into +8 (CIntelligentPointer is 8 bytes: vptr + item).
struct CThing;

template<class T> struct CIntelligentPointer {
    virtual void v00();
    T* m_pItem;                                     // +0x04
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x004A9730 for <const CThing>
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    template<class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second((T2)p.second) {}
};

template pair<CIntelligentPointer<const CThing>, long>::pair(const pair<CIntelligentPointer<const CThing>, float>&);