// CCreatureAction_ShowTrophy::CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy&) -- copy constructor: base CCreatureActionBase(other), own vtable, the trophy pointer, the state word and flag byte, the second intelligent pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DFB90, length 129 bytes (+ int3 padding).
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (twice each: the inline
// CIntelligentPointer copy constructor of the landed 0x0084C070 shape, vtable 0x01238C6C). Own vtable 0x0125F7B4.
// Shape: memberwise copy in declaration order; the second pointer sits at the unaligned +0xb5 (the class is packed to 1, see the
// constructor 0x006DF730), the flag byte is copied through cl. Each pointer member's address rides in ebx. `ret 4`.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

#pragma pack(push, 1)
struct CCreatureAction_ShowTrophy : CCreatureActionBase {      // vtable 0x0125F7B4; retail sizeof 0xbd
    CIntelligentPointer PTrophy;            // +0xa8 CIntelligentPointer<CThing>
    long TrophyState;                       // +0xb0
    unsigned char Shown;                    // +0xb4
    CIntelligentPointer PTrophyObject;      // +0xb5 CIntelligentPointer<CThing> (unaligned)
    virtual void v00();
    CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy& other);
};
#pragma pack(pop)

CCreatureAction_ShowTrophy::CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy& other)
    : CCreatureActionBase(other)
    , PTrophy(other.PTrophy)
    , TrophyState(other.TrophyState)
    , Shown(other.Shown)
    , PTrophyObject(other.PTrophyObject)
{
}