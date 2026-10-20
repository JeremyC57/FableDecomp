// CSP_NymphTrigger::Main() -- Secret Passage nymph trigger: wait (frame by frame) until the hero comes within 10 units of the trigger, then spawn the succubus nymph at the trigger with the hero-seeking script
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DED380, length 208 bytes (`ret`).
// Callees: IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`),
// CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// ~CCharString @ 0x0099EAE0, ~CScriptThing @ 0x004AA840; CScriptThing (vtable 0x01238C8C) slot 6 GetPos;
// CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature(const CCharString&, const C3DVector&,
// const CCharString&, bool) (CScriptThing by value).
// Layout = Ego_r.pdb CSP_NymphTrigger (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14 (CQ_SecretPassageScript*), PMasterData +0x18.
// Shape: the top-tested wait loop of the landed CHTG_AmbushPoint::Main 0x00D73610 (distance test repeated at the
// bottom, the 10.0f pushed before GetHero); the two literal names are implicit CCharString conversions built before
// every other argument (right to left), the created creature is a named local destroyed after them.
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CScriptThing {                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    ~CScriptThing();                               // 0x004AA840
    unsigned char _body[8];
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CGameScriptInterface {                     // vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv names)
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
    virtual CScriptThing& GetHero();                                         // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, bool b) const;   // slot 91 (+0x16c)
};
struct CQ_SecretPassageScript;
struct CQ_SunnyvaleMasterData;

class CSP_NymphTrigger {
public:
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    CQ_SecretPassageScript* ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    bool IsActiveThreadTerminating() const;        // 0x00F35B30 CEntityScriptBase
    void Main();
};

void CSP_NymphTrigger::Main()
{
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing nymph = Game->CreateCreature("CREATURE_SUCCUBUS_NYMPH_01", Me.GetPos(), "EnemySeekingHero", true);
}
