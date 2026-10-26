// CNOVI_Barrel::Main() -- tutorial barrel entity script: until the parent's game-info flag is set, wait for the hero within 2m, show the Xbox or PC game-info text, wait for it to be clicked past and set the flag; then idle until the thread terminates
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB7E10, 0x1DC bytes (padded to 480), `ret`.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (predicate vtable 0x012C2FE8),
//   CEntityScriptBase::SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0. Entity interface (+4) slots 6 IsXbox,
//   7 NewScriptFrame, 40 MsgIsGameInfoClickedPast, 70 GetHero, 115 DisplayGameInfo (cgsi_slots_aligned.tsv).
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 whose game-info-shown bool is at +0x82.
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
};
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual bool IsXbox() const;   // slot 6 (+0x18)
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual bool MsgIsGameInfoClickedPast() const;   // slot 40 (+0xa0)
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void DisplayGameInfo(const CCharString& text) const;   // slot 115 (+0x1cc)
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CIsThingPredicate {                         // vtable 0x012C2FE8: a predicate holding a script thing
    virtual void v00();
    CScriptThing Thing;                            // +0x04
    CIsThingPredicate(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingPredicate() { }
};
struct CNOVIParent {
    unsigned char _pad[0x82];
    bool          GameInfoShown;                   // +0x82
};
class CNOVI_Barrel {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CNOVIParent*          ParentClass;             // +0x14
    void SetPredicate(const CIsThingPredicate& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating() const;        // 0x00F35B30
    void Main();
};
void CNOVI_Barrel::Main()
{
    {
        CIsThingPredicate predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->GameInfoShown) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsXbox()) {
                if (IsActiveThreadTerminating())
                    return;
                {
                    CCharString text("XBOX_TEXT");
                    Game->DisplayGameInfo(text);
                }
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                {
                    CCharString text("PC_TEXT");
                    Game->DisplayGameInfo(text);
                }
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GameInfoShown = true;
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}