// CCreatureAction_UnsheatheToShowToSheatheTrophy::FinishAction() -- base (unsheathe) FinishAction, then hand the creature a temporary CCreatureAction_ShowToSheatheTrophy(creature, Trophy, ShowAnimName) as its current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DD8F0, length 132 bytes.
// Callees: CCreatureAction_UnsheatheItemFromInventory::FinishAction @ 0x0069FC40 (direct base call), CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50 (PCreature at +8 twice, Trophy at +0xbc), CCreatureAction_ShowTrophy::CCreatureAction_ShowTrophy(
// CThingCreatureBase*, CThing*, const CCharString&, long) @ 0x006DF730, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (twice), CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0
// (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated).
// Shape: the creature is fetched into a named local BEFORE the trophy (VC would otherwise evaluate the right-hand argument
// first); the 0xc0-byte temporary's inline constructor adds the ShowToSheatheTrophy vtable 0x0125F9F4 after the
// out-of-line ShowTrophy constructor (action code 1001); its implicit destructor destroys PEmitter (+0xb5, packed after the
// PostedDeed byte) and PTrophyBeingShown (+0xa8) through the out-of-line CBaseIntelligentPointer destructor, then the base.
// Retail layouts are 0xc below Ego_r.pdb (CCreatureActionBase 0xa8): ShowAnimName +0xb8, Trophy +0xbc; modelled locally.
class CBaseObject;
class CThing;
class CCreatureActionBase;

class CCharString { public:
    void* m_pData;
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
};

class CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0 (CThingCreatureBase)
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    virtual ~CCreatureActionBase();                 // 0x00693EF0
};

#pragma pack(push, 1)
class CCreatureAction_ShowTrophy : public CCreatureActionBase { public:   // retail sizeof 0xc0
    CIntelligentPointer PTrophyBeingShown;  // +0xa8
    unsigned long ShowingTrophyDeedHandle;  // +0xb0
    bool PostedDeed;                        // +0xb4
    CIntelligentPointer PEmitter;           // +0xb5 (unaligned in retail and in the PDB)
    unsigned char _pad_bd[3];
    CCreatureAction_ShowTrophy(CThing* creature, CThing* trophy, const CCharString& animName, long actionCode);   // 0x006DF730
};
#pragma pack(pop)

class CCreatureAction_ShowToSheatheTrophy : public CCreatureAction_ShowTrophy { public:   // vtable 0x0125F9F4
    virtual void v00();
    CCreatureAction_ShowToSheatheTrophy(CThing* creature, CThing* trophy, const CCharString& animName)
        : CCreatureAction_ShowTrophy(creature, trophy, animName, 1001)
    {
    }
};

class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:
    unsigned char _pad_a8[0xb8 - 0xa8];     // PUnsheatheItem +0xa8, PFollowUpAction +0xb0
    void FinishAction();                    // 0x0069FC40
};

class CCreatureAction_UnsheatheToShowToSheatheTrophy : public CCreatureAction_UnsheatheItemFromInventory { public:
    CCharString ShowAnimName;               // +0xb8
    CIntelligentPointer Trophy;             // +0xbc
    void FinishAction();
};

void CCreatureAction_UnsheatheToShowToSheatheTrophy::FinishAction()
{
    CCreatureAction_UnsheatheItemFromInventory::FinishAction();
    CThing* creature = PCreature.GetPItem();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_ShowToSheatheTrophy(creature, Trophy.GetPItem(), ShowAnimName));
}