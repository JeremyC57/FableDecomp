// CHHUndead::Main() -- haunted-house undead script: register an alive-filter over Me, start a script frame; unless done, when the parent script's flag (+0x50) is set remove Me (unless done by then), then run script frames until done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E95E80, length 137 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, the filter registration @ 0x00F35B10,
// the filter's out-of-line destructor @ 0x00E241B0, IsDone @ 0x00F35B30; CGameScriptInterface slots 7 NewScriptFrame and
// 108 RemoveThing(const CScriptThing&, bool, bool) (called with true, true).
// Layout = Ego_r.pdb CHHUndead (32 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14
// (CV_HauntedHouseScript*, the tested byte at +0x50 is unnamed in the PDB), PMasterData +0x18, RaiseUndeadBarAmount +0x1c.
// Shape: as the landed CHTE_Prisoner::Main 0x00D69F50; the early exits are plain returns (the second IsDone test
// jumps straight to the epilogue), the wait loop a `do/while`.
struct CScriptThing {                              // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
};
struct CGameScriptInterface {                     // vtable 0x01260F0C (rtti-vtables.tsv slot names)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                     // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual const CScriptThing& GetHero() const;                              // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
};
struct CIsThingAlive {                             // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
struct CV_HauntedHouseScript {
    unsigned char _pad[0x50];
    bool          m_raiseUndead;                   // +0x50
};
struct CQ_SunnyvaleMasterData;

class CHHUndead {
public:
    void*                  __vftable;              // +0x00
    CGameScriptInterface*  Game;                   // +0x04
    CScriptThing           Me;                     // +0x08
    CV_HauntedHouseScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    float                  RaiseUndeadBarAmount;   // +0x1c
    void RegisterFilter(CIsThingAlive& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CHHUndead::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    if (ParentClass->m_raiseUndead) {
        if (IsDone())
            return;
        Game->RemoveThing(Me, true, true);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}