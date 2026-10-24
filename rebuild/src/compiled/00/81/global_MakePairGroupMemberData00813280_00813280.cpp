// _global::MakePair_GroupMemberData_00813280(const unsigned __int64&, const CGroupMemberData&) -- std::make_pair: build a pair<unsigned __int64, CGroupMemberData> by value from a key and the member data
// Leaf: MakePair_GroupMemberData_00813280
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813280, length 80 bytes, __cdecl, returns the hidden result pointer.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90; CIntelligentPointer vtable 0x01264BAC.
// Shape: `return pair<T1,T2>(a, b);` -- the class result is built straight in the caller's hidden slot ([esp+4] -> edi),
// the inline pair(const T1&, const T2&) constructor copies the two key words, then CGroupMemberData's implicit copy
// constructor through re-based pointers (esi = &result.second, ebx = b): the CIntelligentPointer copy constructor, the
// flag byte and two words. Same element as the 0x00812840 / 0x008132D0 pair constructors.
// Label correction: the catalog's `_Cons_val<allocator<pair<unsigned __int64 const,CGroupMemberData>>>` is propagated
// (three arguments, plain `ret`, the result pointer returned).
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
struct SGroupMemberPairMutable_00813280 {   // std::pair<unsigned __int64, CGroupMemberData>
    unsigned __int64 first;                 // +0x00
    CGroupMemberData second;                // +0x08
    SGroupMemberPairMutable_00813280(const unsigned __int64& a, const CGroupMemberData& b) : first(a), second(b) { }
};

SGroupMemberPairMutable_00813280 MakePair_GroupMemberData_00813280(const unsigned __int64& a, const CGroupMemberData& b)
{
    return SGroupMemberPairMutable_00813280(a, b);
}