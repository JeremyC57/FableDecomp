// Grade: DIFFER(280v274) -- retail enters the final `while (!IsDone()) NewScriptFrame();` through `jmp` to its bottom test; the lane tail-duplicates the IsDone test after the StartScriptingEntity loop (while/for/nested/goto forms all duplicate)
// CPretendGuard::Main() -- entity script: register an alive filter over Me, remove Me once the oracle quest's opening necromancer cutscene has played, then start scripting Me (priority 4) and run frames until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFCCA0, length 274 bytes, plain `ret` (entity-script vtable slot at
// 0x012C7CB4).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (the filter's member; the filter's
// inline destructor re-stores its vtable 0x012C32F0 first), CEntityScriptBase::SetPredicate 0x00F35B10 and
// IsActiveThreadTerminating 0x00F35B30 (on `this`), CBaseObject::CBaseObject 0x0099A380, the scripted-thing handle's
// IsScripting 0x00CD23B9 / Release 0x00CD2770 / ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 (vtable
// 0x0127094C); through Game (+4): NewScriptFrame (slot 7, +0x1c), RemoveThing(const CScriptThing&, bool, bool) (slot
// 108, +0x1b0), StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, long) (slot 8,
// +0x20, 0x0089B5B0).
// Layout = Ego_r.pdb CPretendGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
// (CQ_AwakeningTheOracleScript*: PlayedOpeningNecroCS at retail +0x65 = PDB 0x79 - 0x14), PMasterData +0x18.
// Shape: the landed CEndGameScarletRobe::Main (0x00F01C50) / CFishermanWasp::Main (0x00E7BB30) handle model; the
// final `while (!IsDone()) NewScriptFrame();` is entered through a jump to its bottom test.
struct CScriptThing {                                  // 12 bytes, vtable 0x01238C8C
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};

struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};

struct CBaseObject {                                   // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;                       // +0x04
    CBaseObject();                                     // 0x0099A380
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
    ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();                                       // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, long priority); // slot 8 (+0x20)
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
    virtual void RemoveThing(const CScriptThing& thing, bool b1, bool b2); // slot 108 (+0x1b0)
};

struct CQ_AwakeningTheOracleScript {
    unsigned char _pad0[0x65];
    bool PlayedOpeningNecroCS;                         // +0x65
};

class CPretendGuard {
public:
    void*                        __vftable;            // +0x00
    CGameScriptInterface*        Game;                 // +0x04
    CScriptThing                 Me;                   // +0x08
    CQ_AwakeningTheOracleScript* ParentClass;          // +0x14
    void*                        PMasterData;          // +0x18
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CPretendGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (ParentClass->PlayedOpeningNecroCS) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
    }
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, 4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
