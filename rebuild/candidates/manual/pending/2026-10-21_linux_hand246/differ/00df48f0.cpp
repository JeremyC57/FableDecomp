// Grade: DIFFER(348v348) -- register swap only: Me's address (lea [esi+8]) is held in ebx where retail uses edi (push ebx/push esi vs push esi/push edi)
// CM_Experience::Main() -- Kraken boss-fight experience marker: register a conscious filter on Me, wait for the master data's kraken-defeated flag, then wait while "TheKraken" is still alive and finally drop an experience orb (amount from the script def) 5 units above this marker
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF48F0, length 348 bytes.
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the CIsThingConscious filter,
//   vtable 0x012C2FE8, destroyed inline here), the filter registration @ 0x00F35B10, IsDone @ 0x00F35B30,
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("TheKraken" @ 0x012DE55C); CScriptThing slots
//   6 GetPos and 75 IsAlive (0x004AB130, inlined and devirtualised on the local); CGameScriptInterface (vtable
//   0x01260F0C) slots 7 NewScriptFrame, 72 GetThingWithScriptName, 103 CreateExperienceOrb (CScriptThing by value,
//   discarded). Data: 5.0f @ 0x012C26D8; the script def @ 0x0143E90C (orb amount at +0xDE0).
// Layout: Ego_r.pdb CM_Experience (28 bytes): entity-script base {vptr, Game +4, CScriptThing Me +8}, ParentClass
//   +0x14, PMasterData +0x18 (its byte at +0x84 is the wait flag); CScriptThing as engine/CScriptThing.h, modelled
//   locally for its virtuals; filter as the landed CUndeadRisingZombieAttacker::Main 0x00E0CFF0.
// Shape: the filter declares an empty inline virtual destructor (vtable re-store before the member's destructor); the
//   orb position is `Me.GetPos() + C3DVector(0, 0, 5.0f)` (VC7.1 folds the `+ 0` terms: x copied as an integer, y and z
//   through the x87).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector {
    float x, y, z;
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(x + v.x, y + v.y, z + v.z); }
};
struct CCPPointerInfo;
struct CScriptThing {                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const                   // slot 75 (+0x12c), 0x004AB130
    {
        if (!PImp_Data)
            return false;
        return PImp_Data->IsAlive();
    }
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
};
struct CIsThingConscious {                         // vtable 0x012C2FE8: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingConscious() { }
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                 // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const; // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual CScriptThing CreateExperienceOrb(const C3DVector& position, long amount) const; // slot 103 (+0x19c)
};
struct CScriptDef {
    unsigned char _pad[0xde0];
    long          ExperienceOrbAmount;             // +0xDE0
};
extern CScriptDef* g_pScriptDef;                   // 0x0143E90C
struct CQ_TentacleKrakenBossFightScript;
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x84];
    bool          KrakenDefeated;                  // +0x84
};

class CM_Experience {
public:
    void*                             __vftable;   // +0x00
    CGameScriptInterface*             Game;        // +0x04
    CScriptThing                      Me;          // +0x08
    CQ_TentacleKrakenBossFightScript* ParentClass; // +0x14
    CQ_SunnyvaleMasterData*           PMasterData; // +0x18
    void RegisterFilter(CIsThingConscious& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CM_Experience::Main()
{
    {
        CIsThingConscious filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!PMasterData->KrakenDefeated) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    CScriptThing kraken = Game->GetThingWithScriptName("TheKraken");
    while (kraken.IsAlive()) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->CreateExperienceOrb(Me.GetPos() + C3DVector(0.0f, 0.0f, 5.0f), g_pScriptDef->ExperienceOrbAmount);
}
