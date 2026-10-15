// CVtable_012DB4B8::Main() -- Opening Graveyard entity script: register an alive-filter over Me, let the interface update once, and if the script is not done and the parent quest has started the undead cut-scene, remove Me (fading) through the interface
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCF4C0, length 119 bytes (padded to 128).
// Label correction: the catalog's "CQ_OpeningGraveyardSecretPassageScript::FadeObjectUp" is not a quest-script member:
// this body is slot 1 (Main, entity-script contract [dtor, Main, Init, ...]) of vtable 0x012DB4B8, stored by the factory
// 0x00DCF3D0 (`new` of 0x1c bytes: Game +4, Me +8 copied from the argument, ParentClass +0x14, PMasterData +0x18).
// No RTTI locator and no registered name in the factory, so the class is named by its vtable (the landed convention).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member at +4; filter vtable
// 0x012C32F0), the filter registration @ 0x00F35B10 (on `this`), CScriptThing::~CScriptThing @ 0x004AA840, IsDone
// @ 0x00F35B30 (x2), the game interface's virtual slots 7 (+0x1c) and 108 (+0x1b0, RemoveThing(thing, true, true)).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14 (the parent's byte at +0x50 is
// CQ_OpeningGraveyardSecretPassageScript::UndeadCutsceneStarted in the retail layout), PMasterData +0x18.
// Shape (landed CHTE_Prisoner::Main 0x00D69F50): the filter is a scoped 0x10-byte frame object, the address of Me stays
// in edi; the three-term condition is one `&&` chain re-testing IsDone after the parent flag.
struct CScriptThing {                              // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void Update();   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool fade, bool removeOnFade);   // slot 108 (+0x1b0)
};
struct CIsThingAlive {                             // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct CQ_OpeningGraveyardSecretPassageScript {
    unsigned char _pad[0x50];
    bool          UndeadCutsceneStarted;           // +0x50
};
class CVtable_012DB4B8 {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    void*                 PMasterData;             // +0x18
    void RegisterFilter(CIsThingAlive& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};
void CVtable_012DB4B8::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->Update();
    if (!IsDone() && ParentClass->UndeadCutsceneStarted && !IsDone())
        Game->RemoveThing(Me, true, true);
}