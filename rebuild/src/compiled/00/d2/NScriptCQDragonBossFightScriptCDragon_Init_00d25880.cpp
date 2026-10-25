// NScript::CQ_DragonBossFightScript::CDragon::Init() -- put the boss fight in its high-health state, then read the dragon's three health thresholds from the script def
// Leaf: ?Init@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D25880, 59 bytes, thiscall, plain `ret`: `ParentClass (+0x14)->SetDragonState(0)`
//   (call 0x00D258C0), then three dword copies from the script def 0x0143E90C +0xBB4/+0xBB8/+0xBBC to +0x1C/+0x20/+0x24,
//   the global re-read before each copy (the stores through `this` may alias it).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 452 callee "0xD25880 CDragon::Init"; hand452 README "CBS_Teacher::Init" ->
//   CDragon::Init): its only reference is slot 2 of vtable 0x012CB770 {0x00D259E0, 0x00D25A00, 0x00D25880, 0x00D25870},
//   whose slot 1 is the landed CDragon::Main 0x00D25A00; Ego_r.pdb ?Init@CDragon@CQ_DragonBossFightScript@NScript@@EAEXXZ
//   (private virtual) and CDragon (40 bytes) = ParentClass +0x14 (CQ_DragonBossFightScript*), PMasterData +0x18,
//   MediumHealth / LowHealth / VeryLowHealth +0x1C/+0x20/+0x24 (floats: VC7.1 copies them through integer registers);
//   Ego_r CScriptDef DBDragonMediumHealth / Low / VeryLow are 3164/3168/3172, retail +0xBB4.. (= PDB - 0xA8, the same shift
//   the landed CDragon::Main uses for DBNumMinions* at +0xBC4); the callee is the landed
//   CQ_DragonBossFightScript::SetDragonState 0x00D258C0 (FableWin ?SetDragonState@CQ_DragonBossFightScript@NScript@@QAEXW4EDragonState@12@@Z).
//   The landed CBS_Teacher::Init model (KidIntro CScriptThing copied memberwise) was the wrong class: CBS_Teacher's Init is
//   Ego_r 0x00EA9AB0 in CV_BookCollectingScript, and a CScriptThing copy goes through its handle operator=.
// Model: no engine header for these nested script classes; the base is reduced to its 0x14 bytes.
class CScriptDef {
public:
    unsigned char _pad[0xbb4];
    float DBDragonMediumHealth;                      // +0xbb4
    float DBDragonLowHealth;                         // +0xbb8
    float DBDragonVeryLowHealth;                     // +0xbbc
};
extern CScriptDef* pGlobalScriptDef;                     // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
class CQ_SunnyvaleMasterData;
class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase();
    virtual void Main();
    virtual void Init();
    unsigned char _base[0x10];                       // +0x04..+0x13
};
class CQ_DragonBossFightScript {
public:
    enum EDragonState {
        DRAGON_HIGH_HEALTH,
        DRAGON_MEDIUM_HEALTH,
        DRAGON_LOW_HEALTH,
        DRAGON_VERY_LOW_HEALTH
    };
    void SetDragonState(EDragonState state);         // 0x00D258C0
    class CDragon : public CEntityScriptBase {
        CQ_DragonBossFightScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData*   PMasterData;       // +0x18
        float MediumHealth;                          // +0x1c
        float LowHealth;                             // +0x20
        float VeryLowHealth;                         // +0x24
        virtual void Main();
        virtual void Init();
    };
};
}

void NScript::CQ_DragonBossFightScript::CDragon::Init()
{
    ParentClass->SetDragonState(DRAGON_HIGH_HEALTH);
    MediumHealth = pGlobalScriptDef->DBDragonMediumHealth;
    LowHealth = pGlobalScriptDef->DBDragonLowHealth;
    VeryLowHealth = pGlobalScriptDef->DBDragonVeryLowHealth;
}
