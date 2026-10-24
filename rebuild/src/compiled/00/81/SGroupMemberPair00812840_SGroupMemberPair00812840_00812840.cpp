// SGroupMemberPair_00812840::SGroupMemberPair_00812840(const SGroupMemberPairMutable_00812840&) -- pair<const unsigned __int64, CGroupMemberData> converting constructor from pair<unsigned __int64, CGroupMemberData>: copy the 64-bit key and the member data
// Leaf: ??0SGroupMemberPair_00812840
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812840, length 79 bytes (+ int3 padding), thiscall `ret 4`, returns this.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90; CIntelligentPointer vtable 0x01264BAC.
// Shape: STLport `template <class U1, class U2> pair(const pair<U1,U2>& p) : first(p.first), second(p.second) {}`
// instantiated for the map value_type (the catalog's argument is `pair<unsigned __int64,CGroupMemberData>`). The key
// moves as two words, then CGroupMemberData's implicit copy constructor inlines through re-based pointers (esi =
// &this->second, ebx = &p.second): the CIntelligentPointer copy constructor (base ctor, vtable store,
// SetPItem(other.GetPItem())), the flag byte (+8) and two words (+0xC, +0x10).
// The sibling 0x008132D0 is a byte-identical instantiation. CGroupMemberData member names are descriptive.
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x01264BAC
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CGroupMemberData {                   // 0x14 bytes
    CIntelligentPointer m_member;           // +0x00
    bool m_isLeader;                        // +0x08
    long m_role;                            // +0x0c
    long m_joinTime;                        // +0x10
};
struct SGroupMemberPairMutable_00812840 {   // std::pair<unsigned __int64, CGroupMemberData>
    unsigned __int64 first;                 // +0x00
    CGroupMemberData second;                // +0x08
};
struct SGroupMemberPair_00812840 {          // std::pair<const unsigned __int64, CGroupMemberData>
    const unsigned __int64 first;           // +0x00
    CGroupMemberData second;                // +0x08
    SGroupMemberPair_00812840(const SGroupMemberPairMutable_00812840& p);
};

SGroupMemberPair_00812840::SGroupMemberPair_00812840(const SGroupMemberPairMutable_00812840& p)
    : first(p.first), second(p.second)
{
}