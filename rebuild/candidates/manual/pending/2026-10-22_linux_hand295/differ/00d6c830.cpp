// CQ_HangingTreeEvilScript::CheckBanditFollowStatus() -- watch the bandits' exit point: once the hero comes within 20 units the bandits follow him until the region switch is over, then re-fetch the exit point in the new region
// Grade: DIFFER(607v607) -- tail-merge lane gap: retail shares one terminating-return block between the inner and outer waits (branch targets/scheduling only, same instructions)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6C830, length 607 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::operator= 0x008AB980,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   ~CScriptThing 0x004AA840 (out of line), CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface
//   (+0x40): NewScriptFrame (7), GetHero (70), GetThingWithScriptName (72).
// Strings: "HTE_BanditExitPoint" 0x012D2228.
// Layout (retail = Ego_r.pdb - 0x14): SwitchingRegions +0x51, ShouldFollowHero +0x58.
// Shape: the CScriptThing model of the landed CV_ChapelOfEvilScript::WatchForWeaponUnsheathed 0x00E6A340.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CBaseClass {
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing() {}                                      // out of line 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);     // 0x008AB980
};
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) GetHero
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CQ_HangingTreeEvilScript : CQuestScriptBase {
    bool IntroCutscenePlayed;                               // +0x48
    bool BanditsStarted;                                    // +0x49
    bool BanditKilled;                                      // +0x4a
    bool PartySuccessfullySetUp;                            // +0x4b
    bool MissionSucceeded;                                  // +0x4c
    bool MissionFailed;                                     // +0x4d
    bool BanditsLeftRegion;                                 // +0x4e
    bool SpawnHere;                                         // +0x4f
    bool AtWayPoint;                                        // +0x50
    bool SwitchingRegions;                                  // +0x51
    bool RemoveBandits;                                     // +0x52
    bool AllEnemiesDead;                                    // +0x53
    bool QuestStartScreenShown;                             // +0x54
    bool BanditYelledRecently;                              // +0x55
    bool ExecutionerInterrupted;                            // +0x56
    bool AudienceLeggit;                                    // +0x57
    bool ShouldFollowHero;                                  // +0x58
    void CheckBanditFollowStatus();
};

void CQ_HangingTreeEvilScript::CheckBanditFollowStatus()
{
    CScriptThing exitPoint = Game->GetThingWithScriptName("HTE_BanditExitPoint");
    if (!IsActiveThreadTerminating()) {
        do {
            while (!ShouldFollowHero) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                if (IsDistanceBetweenThingsUnder(Game->GetHero(), exitPoint, 20.0f)) {
                    if (IsActiveThreadTerminating())
                        return;
                    ShouldFollowHero = true;
                    while (!SwitchingRegions) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    ShouldFollowHero = false;
                    while (SwitchingRegions) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                    exitPoint = Game->GetThingWithScriptName("HTE_BanditExitPoint");
                }
            }
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}
