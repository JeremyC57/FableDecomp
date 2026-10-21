// CM_RTFERockTrollTrigger::Main() -- rock-troll trigger marker: alive filter on Me; unless the quest already spawned the troll, wait until the hero comes within the script def's trigger distance, then create the standing rock troll ("RTFE_RockTroll") at the "M_RTFERockTrollSpawnPos" marker, flag it spawned and remove this trigger
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC42F0, length 412 bytes (+ int3 padding).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840; the CIsThingAlive filter (vtable 0x012C32F0,
//   destructor 0x00E241B0) registered @ 0x00F35B10; IsDone @ 0x00F35B30; IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`);
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("M_RTFERockTrollSpawnPos" 0x012F16AC, "RTFE_RockTroll"
//   0x012F1644, "CREATURE_ROCK_TROLL_START_STANDING" 0x012F1688); CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero,
//   72 GetThingWithScriptName, 91 CreateCreature, 108 RemoveThing. Script def @ 0x0143E90C (trigger distance +0x72c); zero vector
//   @ 0x0143E8E0 (the inline CScriptThing::GetPos).
// Layout: the entity-script base {vptr, Game +4, Me +8} with the quest object at +0x14 (spawned flag +0x4e), as the landed
//   CUndeadSoldier::Main 0x00D88F90 and CHTG_AmbushPointCond::Main 0x00D73970.
// Shape: the scoped filter object of the landed entity-script Mains; the creature's two names are implicit conversion temporaries; the in-loop
//   IsDone exit jumps to one shared `done:` return at the end, which moves the destroy-spawnPos exit block below the inline GetPos
//   ternary as in retail (a plain in-loop `return` keeps it inline after the post-loop IsDone test).
//   built before the position argument; the CreateCreature result is a discarded temporary (it reuses the filter's slot).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float x, y, z; };
extern C3DVector g_ZeroVector;                     // 0x0143E8E0
struct CCPPointerInfo;
struct CScriptThing {                              // vtable 0x01238C8C, 12 bytes
    virtual void v00() const; virtual void v01() const; virtual void v02() const; virtual void v03() const;
    virtual void v04() const; virtual void v05() const;
    virtual __forceinline const C3DVector& GetPos() const   // slot 6 (+0x18), 0x004AA980
    {
        if (!PImp_Data)
            return g_ZeroVector;
        return PImp_Data->GetPos();
    }
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CGameScriptInterface {
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const;
    virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const;
    virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const;
    virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const;
    virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const;
    virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const;
    virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const;
    virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    virtual void v071() const;
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const;
    virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const;
    virtual void v089() const; virtual void v090() const;
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& position, const CCharString& scriptName, bool flag) const;   // slot 91 (+0x16c)
    virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const;
    virtual void v100() const; virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const; virtual void v107() const;
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
};
struct CIsThingAlive {                             // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
struct CScriptDef {
    unsigned char _pad[0x72c];
    float         RockTrollTriggerDistance;        // +0x72c (descriptive)
};
extern CScriptDef* g_pScriptDef;                   // 0x0143E90C
struct CQ_RTFEScript {
    unsigned char _pad[0x4e];
    bool          RockTrollSpawned;                // +0x4e (descriptive)
};

class CM_RTFERockTrollTrigger {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CQ_RTFEScript*        ParentClass;             // +0x14
    void RegisterFilter(CIsThingAlive& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CM_RTFERockTrollTrigger::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    if (ParentClass->RockTrollSpawned)
        return;
    if (IsDone())
        return;
    CScriptThing spawnPos = Game->GetThingWithScriptName("M_RTFERockTrollSpawnPos");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, g_pScriptDef->RockTrollTriggerDistance)) {
        Game->NewScriptFrame();
        if (IsDone())
            goto done;
    }
    if (IsDone())
        return;
    Game->CreateCreature("CREATURE_ROCK_TROLL_START_STANDING", spawnPos.GetPos(), "RTFE_RockTroll", false);
    ParentClass->RockTrollSpawned = true;
    Game->RemoveThing(Me, false, true);
    return;
done:
    return;
}