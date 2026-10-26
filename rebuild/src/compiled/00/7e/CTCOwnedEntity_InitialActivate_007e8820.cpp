// CTCOwnedEntity::InitialActivate() -- base activation, then re-binds POwner from the saved OwnerUID when one is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8820, length 46 bytes.
// Callees: CTCBase::InitialActivate() @ 0x00686860 (empty, called directly), CTCBase::GetGame() @ 0x00686AE0 (returns the
// game global @ 0x013B8A1C; manifest label "GFGetPreMainMemoryUsed" is propagated), the game's GetThingFromUID(unsigned
// __int64) @ 0x0051ED80 (a map lookup at +0x58, `ret 8`; label "CStopWatch::GetTicks" is propagated),
// CTCOwnedEntity::SetPOwner(CThing*) @ 0x007E8590.
// Shape: the UID's two words are loaded once, tested with `or`, and pushed as the argument before GetGame runs.
#pragma pack(push, 1)
class CThing;

struct CGame_007e8820 {
    CThing* GetThingFromUID(unsigned __int64 uid);  // 0x0051ED80
};

struct CTCBase_007e8820 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
    void InitialActivate();                         // 0x00686860
    CGame_007e8820* GetGame();                      // 0x00686AE0
};

class CTCOwnedEntity : public CTCBase_007e8820 { public:
    bool             SwitchableNavigationTCAdded;   // +0x0c
    unsigned char    POwner[8];                     // +0x0d CIntelligentPointer<CThing>
    unsigned char    _pad15[3];
    unsigned __int64 OwnerUID;                      // +0x18
    void SetPOwner(CThing* owner);                  // 0x007E8590
    void InitialActivate();
};
#pragma pack(pop)

void CTCOwnedEntity::InitialActivate()
{
    CTCBase_007e8820::InitialActivate();
    if (OwnerUID != 0)
        SetPOwner(GetGame()->GetThingFromUID(OwnerUID));
}