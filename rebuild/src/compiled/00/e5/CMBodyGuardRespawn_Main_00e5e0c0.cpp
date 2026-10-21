// CM_BodyGuardRespawn::Main() -- bodyguard respawn marker: find which of the four bodyguards this marker stands for (from its data string); if that bodyguard is dead (status 1) and its respawn timer has run out, respawn the matching CREATURE_BODY_GUARD* creature here as "BodyGuard", tag it with its _EVIL1/_EVIL2/_GOOD1/_GOOD2 suffix and mark it alive
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5E0C0, length 360 bytes + a 4-entry jump table (0x00E5E228).
// Callees: CScriptThing slots 3 GetDataString, 4 SetDataString (0x004AA930, inlined on the local), 6 GetPos;
//   CV_BodyGuardScript's index-from-data-string helper @ 0x00E5E240 (manifest "GetAlignmentFromCode"), CCharString()
//   @ 0x0099E4B0, operator=(const char*) @ 0x0099EFE0, CCharString(const char*, long) @ 0x0099EBF0, ~CCharString
//   @ 0x0099EAE0, ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30; the game script interface global @ 0x0143E8F8 slot 90
//   GetTimer and Game's slot 91 CreateCreature (CScriptThing by value). Strings: "CREATURE_BODY_GUARD_WEAK" @ 0x012578E0,
//   "CREATURE_BODY_GUARD" @ 0x012578FC, "CREATURE_BODY_GUARD_GOOD_WEAK" @ 0x012578A4, "CREATURE_BODY_GUARD_GOOD"
//   @ 0x012578C4, "_EVIL1" @ 0x012E706C, "_EVIL2" @ 0x012E7064, "_GOOD1" @ 0x012E705C, "_GOOD2" @ 0x012E7054,
//   "BodyGuard" @ 0x012CB3F0.
// Layout: Ego_r.pdb CM_BodyGuardRespawn (28 bytes): entity-script base {vptr, Game +4, CScriptThing Me +8}, ParentClass
//   (CV_BodyGuardScript*) +0x14. Retail CV_BodyGuardScript sits 0x18 bytes below the PDB here: HenchmanDeadRespawnTimer
//   (CTimer[4], one timer index each) at +0x74 and HenchmanGlobalStatus (long[4]) at +0x84 (PDB +0x8C/+0x9C), so the
//   quest is modelled locally.
// Shape: the two strings are default-constructed and assigned per case (the suffix assignment is tail-merged across the
//   cases); the switch's default exits through the strings' destructors.
struct CCharString {
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct C3DVector { float x, y, z; };
struct CCPPointerInfo;
struct CScriptThing {                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CCharString GetDataString() const;     // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& text)   // slot 4 (+0x10), 0x004AA930
    {
        if (PImp_Data)
            PImp_Data->SetDataString(text);
    }
    virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    ~CScriptThing();                               // 0x004AA840
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089();
    virtual long GetTimer(long index) const;                             // slot 90 (+0x168)
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& position, const CCharString& scriptName, bool flag) const; // slot 91 (+0x16c)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
struct CV_BodyGuardScript {
    unsigned char _pad[0x74];
    CTimer        HenchmanDeadRespawnTimer[4];     // +0x74
    long          HenchmanGlobalStatus[4];         // +0x84
    long GetHenchmanIndex(const CCharString& code);    // 0x00E5E240
};
struct CQ_SunnyvaleMasterData;

class CM_BodyGuardRespawn {
public:
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    CV_BodyGuardScript*     ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CM_BodyGuardRespawn::Main()
{
    long index = ParentClass->GetHenchmanIndex(Me.GetDataString());
    if (ParentClass->HenchmanGlobalStatus[index] != 1)
        return;
    if (ParentClass->HenchmanDeadRespawnTimer[index].GetTime() != 0)
        return;
    if (IsDone())
        return;
    CCharString def;
    CCharString suffix;
    switch (index) {
    case 0:
        def = "CREATURE_BODY_GUARD_WEAK";
        suffix = "_EVIL1";
        break;
    case 1:
        def = "CREATURE_BODY_GUARD";
        suffix = "_EVIL2";
        break;
    case 2:
        def = "CREATURE_BODY_GUARD_GOOD_WEAK";
        suffix = "_GOOD1";
        break;
    case 3:
        def = "CREATURE_BODY_GUARD_GOOD";
        suffix = "_GOOD2";
        break;
    default:
        return;
    }
    CScriptThing bodyguard = Game->CreateCreature(def, Me.GetPos(), "BodyGuard", false);
    bodyguard.SetDataString(suffix);
    ParentClass->HenchmanGlobalStatus[index] = 0;
}