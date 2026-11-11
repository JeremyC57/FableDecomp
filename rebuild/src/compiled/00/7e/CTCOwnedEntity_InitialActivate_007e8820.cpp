// CTCOwnedEntity::InitialActivate() -- base activation, then re-binds POwner from the saved OwnerUID when one is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8820, length 46 bytes.
// Callees: CTCBase::InitialActivate() @ 0x00686860 (empty, called directly), CTCBase::GetThingManager() @ 0x00686AE0 (Ego_r.pdb; returns
// *CThing::PCurrentThingManager @ 0x013B8A1C; manifest label "GFGetPreMainMemoryUsed" is propagated), CThingManager::GetPThingWithUID(unsigned
// __int64) @ 0x0051ED80 (Ego_r.pdb) (a map lookup at +0x58, `ret 8`; label "CStopWatch::GetTicks" is propagated),
// CTCOwnedEntity::SetPOwner(CThing*) @ 0x007E8590.
// Shape: the UID's two words are loaded once, tested with `or`, and pushed as the argument before GetGame runs.
#pragma pack(push, 1)
class CThing;

class CThingManager { public:                       // (was the stand-in CGame_007e8820)
    CThing* GetPThingWithUID(unsigned __int64 uid); // 0x0051ED80 Ego_r.pdb ?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z
};

class CTCBase {                                     // (was the stand-in CTCBase_007e8820)
public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
    virtual void InitialActivate();                 // 0x00686860 (Ego_r.pdb UAE)
protected:
    CThingManager& GetThingManager();               // 0x00686AE0 Ego_r.pdb ?GetThingManager@CTCBase@@IAEAAVCThingManager@@XZ (was GetGame)
};

class CTCOwnedEntity : public CTCBase { public:
    bool             SwitchableNavigationTCAdded;   // +0x0c
    unsigned char    POwner[8];                     // +0x0d CIntelligentPointer<CThing>
    unsigned char    _pad15[3];
    unsigned __int64 OwnerUID;                      // +0x18
    void SetPOwner(CThing* owner);                  // 0x007E8590
    virtual void InitialActivate();                 // Ego_r.pdb UAE
};
#pragma pack(pop)

void CTCOwnedEntity::InitialActivate()
{
    CTCBase::InitialActivate();
    if (OwnerUID != 0)
        SetPOwner(GetThingManager().GetPThingWithUID(OwnerUID));
}