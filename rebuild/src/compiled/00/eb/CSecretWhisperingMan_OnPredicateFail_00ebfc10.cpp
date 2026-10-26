// CSecretWhisperingMan::OnPredicateFail() -- when the whispering man reports he was killed, move the Paranoid Whispers quest to state 5, show the digging spot again and point the Mayor's Invitation quest card at its fifth objective in Oakbay
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBFC10, 210 bytes (+ int3 padding), `ret`; entity-script vtable slot 3
//   (vtable entry 0x012F1024; the factory 0x00EBFB00 "SecretWhisperingMan" allocates 0x28 = Ego_r.pdb sizeof 40).
// Leaf: OnPredicateFail
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literals "" @ 0x0122D70E, "OakBay" @ 0x012D31E4,
//   "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_05" @ 0x012F102C, "V_MayorsInvitation" @ 0x012C5008), ~CCharString @ 0x0099EAE0,
//   CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48, 0x004AAAF0), CGameScriptInterface slot 296
//   SetQuestCardObjective(4 x const CCharString&) (+0x4A0).
// Shape: the "" conversion temporary lives only for the test (the bool rides in bl across its destructor); the four
//   SetQuestCardObjective literals are implicit conversion temporaries built right to left into the 16-byte frame
//   before the vtable fetch, then destroyed in construction-reverse order. Same TU family as the landed
//   CDarkwoodRockTroll::OnPredicateFail 0x00E03B00 and CTraderToRescue::OnPredicateFail 0x00DFB100.
// Layout = Ego_r.pdb CSecretWhisperingMan (40 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
//   ParentClass +0x14 (CV_ParanoidWhispersScript: ParanoidState +0x48, HideDiggingSpot +0x4C = PDB - 0x14),
//   PMasterData +0x18, StartPos +0x1C.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#define SCRIPT_SLOT(n) virtual void slot##n()
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0); SCRIPT_SLOT(d##1); SCRIPT_SLOT(d##2); SCRIPT_SLOT(d##3); SCRIPT_SLOT(d##4); SCRIPT_SLOT(d##5); SCRIPT_SLOT(d##6); SCRIPT_SLOT(d##7); SCRIPT_SLOT(d##8); SCRIPT_SLOT(d##9)
class CGameScriptInterface { public:                          // vtable 0x01260F0C (only the slot used is typed)
    SCRIPT_SLOTS10(0); SCRIPT_SLOTS10(1); SCRIPT_SLOTS10(2); SCRIPT_SLOTS10(3); SCRIPT_SLOTS10(4);
    SCRIPT_SLOTS10(5); SCRIPT_SLOTS10(6); SCRIPT_SLOTS10(7); SCRIPT_SLOTS10(8); SCRIPT_SLOTS10(9);
    SCRIPT_SLOTS10(10); SCRIPT_SLOTS10(11); SCRIPT_SLOTS10(12); SCRIPT_SLOTS10(13); SCRIPT_SLOTS10(14);
    SCRIPT_SLOTS10(15); SCRIPT_SLOTS10(16); SCRIPT_SLOTS10(17); SCRIPT_SLOTS10(18); SCRIPT_SLOTS10(19);
    SCRIPT_SLOTS10(20); SCRIPT_SLOTS10(21); SCRIPT_SLOTS10(22); SCRIPT_SLOTS10(23); SCRIPT_SLOTS10(24);
    SCRIPT_SLOTS10(25); SCRIPT_SLOTS10(26); SCRIPT_SLOTS10(27); SCRIPT_SLOTS10(28);
    SCRIPT_SLOT(290); SCRIPT_SLOT(291); SCRIPT_SLOT(292); SCRIPT_SLOT(293); SCRIPT_SLOT(294); SCRIPT_SLOT(295);
    virtual void SetQuestCardObjective(const CCharString& quest, const CCharString& text, const CCharString& region, const CCharString& regionAlt) const;   // slot 296 (+0x4a0)
};
class CScriptThing { public:                                  // vtable 0x01238C8C (only the slot used is typed)
    SCRIPT_SLOTS10(0); SCRIPT_SLOT(10); SCRIPT_SLOT(11); SCRIPT_SLOT(12); SCRIPT_SLOT(13); SCRIPT_SLOT(14);
    SCRIPT_SLOT(15); SCRIPT_SLOT(16); SCRIPT_SLOT(17);
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48, 0x004AAAF0)
    long m_data[2];
};
struct CV_ParanoidWhispersScript {
    unsigned char _pad[0x48];
    long          ParanoidState;                       // +0x48 (PDB +0x5c)
    bool          HideDiggingSpot;                     // +0x4c (PDB +0x60)
};
struct CQ_SunnyvaleMasterData;
class C3DVector { public: float X, Y, Z; };
class CSecretWhisperingMan {
public:
    void*                      __vftable;              // +0x00
    CGameScriptInterface*      Game;                   // +0x04
    CScriptThing               Me;                     // +0x08
    CV_ParanoidWhispersScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;            // +0x18
    C3DVector                  StartPos;               // +0x1c
    void OnPredicateFail();
};
void CSecretWhisperingMan::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->ParanoidState = 5;
        ParentClass->HideDiggingSpot = false;
        Game->SetQuestCardObjective("V_MayorsInvitation", "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_05", "OakBay", "");
    }
}