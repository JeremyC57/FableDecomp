// CFH_BriarRose::Main() -- once the scythe cutscene has played, remove Briar Rose (unless the script is ending); otherwise release any old scripting handle and keep trying, a script frame at a time, to start scripting her through the quest's handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D37330, 156 bytes (+ int3 padding).
// Callees: IsDone @ 0x00F35B30 (on `this`: the script thread's IsActiveThreadTerminating; manifest
//   "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated, as in the landed CHTE_Prisoner::Main 0x00D69F50), the game
//   script interface (+4) slots 7 NewScriptFrame, 8 StartScriptingEntity(const CScriptThing&,
//   CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority), 108 RemoveThing(const CScriptThing&, bool, bool); the
//   handle's IsScripting @ 0x00CD23B9 and Release @ 0x00CD2770 (manifest labels "C3DMeshInfo::HasPhysicsMesh" /
//   "CMemoryDataOutputStream::Clear" are propagated).
// Layout = Ego_r.pdb CFH_BriarRose (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass
//   (CQ_FireHeartScript*) +0x14, PMasterData +0x18. In the retail CQ_FireHeartScript (0x64 below the PDB) the
//   HasScytheCutscenePlayed flag is +0x1a1 and the seh_BriarRose handle +0x154.
// Shape: the early arm ends in `call ; pop ; ret`; the release test goes through a named handle pointer (edi; a member
//   expression re-reads ParentClass for the Release call); `while (!StartScriptingEntity(...)) { NewScriptFrame(); if (IsDone())
//   return; }` is rotated (test first, body + bottom test), the handle address re-derived from ParentClass for each call and
//   &Me kept in edi; the final `IsDone();` is a tail jump.
class CThing;
class CScriptThing { public: unsigned char _body[12]; };

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

class CScriptGameResourceObjectScriptedThingBase { public:
    unsigned char _body[0x10];
    bool IsScripting();                          // 0x00CD23B9
    void Release();                              // 0x00CD2770
};

struct CGameScriptInterface_Calls {               // the game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
};

struct CQ_FireHeartScript {                      // retail offsets (PDB less 0x64)
    unsigned char _pad0[0x154];
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;   // +0x154
    unsigned char _pad164[0x1a1 - 0x164];
    bool          HasScytheCutscenePlayed;       // +0x1a1
};
struct CQ_SunnyvaleMasterData;

struct CFH_BriarRose {
    void*                       __vftable;       // +0x00
    CGameScriptInterface_Calls* Game;            // +0x04
    CScriptThing                Me;              // +0x08
    CQ_FireHeartScript*         ParentClass;     // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;     // +0x18
    bool IsDone();                               // 0x00F35B30
    void Main();
};

void CFH_BriarRose::Main()
{
    if (ParentClass->HasScytheCutscenePlayed) {
        if (IsDone())
            return;
        Game->RemoveThing(Me, false, true);
        return;
    }
    CScriptGameResourceObjectScriptedThingBase* handle = &ParentClass->seh_BriarRose;
    if (handle->IsScripting())
        handle->Release();
    while (!Game->StartScriptingEntity(Me, ParentClass->seh_BriarRose, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    IsDone();
}