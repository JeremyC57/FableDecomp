// CV_ChickenKickingScript::LookAfterOrganiser() -- each time the hero leaves Oak Bay and comes back, re-create the chicken master if he ran off: at his original marker once the hero knows the ghost has gone, else at the scared-master marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E631F0, length 701 bytes. Identity: spawned by CV_ChickenKickingScript::Main
//   0x00E62A00 as "LookAfterOrganiser" 0x012E7378 (CSpawnedFunc vtable 0x012E732C at 0x00E62CFB); the strings name the chicken-kicking quest (manifest
//   "CQ_GuildTrainingScript::KeepTabsOnWhisper" is propagated).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptThing::GetPos (virtual slot 6, +0x18); through the
//   interface (+0x40): NewScriptFrame (7), IsRegionLoaded (12), GetThingWithScriptName (72), CreateCreature (91).
// Strings: "OakBay" 0x012D31E4, "ChickenMaster" 0x012E73D0, "MK_CK_ORG" 0x012E7470, "ScaredMasterMarker" 0x012E745C,
//   "CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED" 0x012E7424.
// Layout (retail = Ego_r.pdb CV_ChickenKickingScript - 0x14): KnowGhostHasGone +0x54, RanOff +0x56.
// Shape: the "while loaded" wait is written `!= false` (test/setne, see RunThemeOverriding 0x00D35ED0); the marker
//   handle and the created creature are by-value CScriptThing temporaries destroyed out of line.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CScriptThing {                                   // 12 bytes: vtable 0x01238C8C + counted pointer
    virtual ~CScriptThing();                            // 0x004AA840 (slot 0)
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    void* PImpData;
    void* PImpInfo;
};
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30) IsRegionLoaded
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& pos, const CCharString& scriptName, bool flag) const;   // slot 91 (+0x16c) CreateCreature
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CV_ChickenKickingScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool ChickenLanded;                                 // +0x48
    long FinalSector;                                   // +0x4c
    long DistanceBand;                                  // +0x50
    bool KnowGhostHasGone;                              // +0x54
    bool SpectatorsCreated;                             // +0x55
    bool RanOff;                                        // +0x56
    void LookAfterOrganiser();
};

void CV_ChickenKickingScript::LookAfterOrganiser()
{
    if (IsActiveThreadTerminating())
        return;
    do {
        while (Game->IsRegionLoaded("OakBay") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("OakBay")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (RanOff) {
            if (IsActiveThreadTerminating())
                return;
            if (KnowGhostHasGone) {
                if (IsActiveThreadTerminating())
                    return;
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("MK_CK_ORG").GetPos(), "ChickenMaster", false);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("ScaredMasterMarker").GetPos(), "ChickenMaster", false);
            }
            RanOff = false;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
