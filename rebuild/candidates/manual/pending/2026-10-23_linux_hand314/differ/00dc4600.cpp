// CCeremonialEffect::Main() -- graveyard secret-passage ceremonial effect: until the hero carries this effect's hero object (index from Me's data string), once skeleton_visited is set put a mini-map marker on Me and create the GRAVEYARD_SPARKLES_GRAVE effect at Me (re-creating it whenever it dies); then give the hero experience, remove the marker and the effect, and if the doors are open spawn an undead lieutenant at SecretPassageUndeadSpawn set on the hero; idle until terminated
// Grade: DIFFER(1188v1173) -- register allocation: ours keeps 0 in ebx and &Me in ebp (retail: &Me in edi, ebx for the Game vtable), frame 0x50 vs 0x4c; retail also mixes an inline CScriptThing default ctor (effect) with the out-of-line 0x006E7B40 (an unused local) and builds the creature as a named local before GetHero; tried named creature + unused local (worse, 1219)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC4600, length 1184 bytes, plain `ret`.
// Callees: CBase ctor 0x0099A2D0 / dtor 0x0099A2E0 (the CScriptThing base), CScriptThing() 0x006E7B40 / dtor 0x004AA840
//   (the inline default ctor, assignment and dtor are expanded where retail expands them), GFCharStringToInt 0x0099E7F0,
//   CCharString 0x0099EBF0 / 0x0099EAE0, operator delete 0x00BFE9BC, IsActiveThreadTerminating 0x00F35B30.
//   CGameScriptInterface slots 7, 70, 77, 91 CreateCreature, 100 CreateEffect, 108 RemoveThing, 128 GiveHeroExperience,
//   184 IsObjectInThingsPossession, 350 MiniMapAddMarker, 352 MiniMapRemoveMarker, 451; CScriptThing slots 3, 6, 75.
// Literals: "HUD_ORB_GREEN_SMALL" 0x012C7F50, "GRAVEYARD_SPARKLES_GRAVE" 0x012C7F34, "SecretPassageUndeadSpawn"
//   0x012DA63C, "CREATURE_OK_UNDEAD_LIEUTENANT_AXE_LOW_DETAIL" 0x012DA60C, "" 0x0122D70E. g_pScriptDef 0x0143E90C +0x3c.
// Layout: Ego_r.pdb CCeremonialEffect (28 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_OpeningGraveyardSecretPassageScript, retail = PDB - 0x14: skeleton_visited +0x4b, doors_open +0x4c,
//   CCharString hero_object[4] +0x58), PMasterData +0x18.
struct C3DVector { float X, Y, Z; };
extern C3DVector g_ZeroVector;                         // 0x0143E8E0
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
void operator delete(void* p);                         // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThing;
struct CCountedPointer {
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = other.Data;
            Info = other.Info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBase {
    CBase();                                           // 0x0099A2D0
    virtual ~CBase();                                  // 0x0099A2E0
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CScriptThing : CBase {                          // 12 bytes, vtable 0x01238C8C
    VPAD1(p0)                                          // slot 1 (slot 0 is the dtor)
    virtual void p1() const;                           // slot 2
    virtual CCharString GetDataString() const;         // slot 3 (+0xc)
    VPAD2(p2)                                          // slots 4..5
    __forceinline virtual const C3DVector& GetPos() const { if (!PImp.Data) return g_ZeroVector; return PImp.Data->GetPos(); }   // slot 6 (+0x18)
    VPAD64(p3) VPAD4(p4)                               // slots 7..74
    __forceinline virtual bool IsAlive() const { if (PImp.Data && PImp.Data->IsAlive()) return true; return false; }   // slot 75 (+0x12c)
    CCountedPointer PImp;                              // +0x04
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing() { }
};
struct CGameScriptInterface {
    VPAD4(p0x0) VPAD2(p0x1) VPAD1(p0x2)   // slots 0..6
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    VPAD32(p1x0) VPAD16(p1x1) VPAD8(p1x2) VPAD4(p1x3) VPAD2(p1x4)   // slots 8..69
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    VPAD4(p2x0) VPAD2(p2x1)   // slots 71..76
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const;   // slot 77 (+0x134)
    VPAD8(p3x0) VPAD4(p3x1) VPAD1(p3x2)   // slots 78..90
    virtual CScriptThing CreateCreature(const CCharString& name, const C3DVector& pos, const CCharString& bone, bool a) const;   // slot 91 (+0x16c)
    VPAD8(p4x0)   // slots 92..99
    virtual CScriptThing CreateEffect(const CCharString& effect, const C3DVector& position, const CCharString& bone, float angle, bool b, bool c) const;   // slot 100 (+0x190)
    VPAD4(p5x0) VPAD2(p5x1) VPAD1(p5x2)   // slots 101..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    VPAD16(p6x0) VPAD2(p6x1) VPAD1(p6x2)   // slots 109..127
    virtual void GiveHeroExperience(long amount) const;   // slot 128 (+0x200)
    VPAD32(p7x0) VPAD16(p7x1) VPAD4(p7x2) VPAD2(p7x3) VPAD1(p7x4)   // slots 129..183
    virtual bool IsObjectInThingsPossession(const CCharString& def, const CScriptThing& thing) const;   // slot 184 (+0x2e0)
    VPAD128(p8x0) VPAD32(p8x1) VPAD4(p8x2) VPAD1(p8x3)   // slots 185..349
    virtual void MiniMapAddMarker(const CScriptThing& thing, const CCharString& marker) const;   // slot 350 (+0x578)
    VPAD1(p9x0)   // slots 351..351
    virtual void MiniMapRemoveMarker(const CScriptThing& thing) const;   // slot 352 (+0x580)
    VPAD64(p10x0) VPAD32(p10x1) VPAD2(p10x2)   // slots 353..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
};

struct CQ_OpeningGraveyardSecretPassageScript {
    unsigned char _pad[0x4b];
    bool        skeleton_visited;                      // +0x4b
    bool        doors_open;                            // +0x4c
    unsigned char _pad4d[0x58 - 0x4d];
    CCharString hero_object[4];                        // +0x58
};
struct CScriptDef {
    unsigned char _pad[0x3c];
    long CeremonialEffectExperience;                   // +0x3c
};
extern CScriptDef* g_pScriptDef;                       // 0x0143E90C
struct CQ_SunnyvaleMasterData;
class CCeremonialEffect {
public:
    void*                                   __vftable; // +0x00
    CGameScriptInterface*                   Game;      // +0x04
    CScriptThing                            Me;        // +0x08
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*                 PMasterData;   // +0x18
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CCeremonialEffect::Main()
{
    bool started = false;
    CScriptThing effect;
    long index = GFCharStringToInt(Me.GetDataString()) - 1;
    while (!Game->IsObjectInThingsPossession(ParentClass->hero_object[index], Game->GetHero())) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!started) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->skeleton_visited) {
                if (IsActiveThreadTerminating())
                    return;
                Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
                effect = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
                started = true;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (!effect.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                effect = Game->CreateEffect("GRAVEYARD_SPARKLES_GRAVE", Me.GetPos(), "", 0.0f, false, true);
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveHeroExperience(g_pScriptDef->CeremonialEffectExperience);
    Game->MiniMapRemoveMarker(Me);
    if (effect.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(effect, false, true);
    }
    if (ParentClass->doors_open) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing spawn = Game->GetNearestWithScriptName(Me, "SecretPassageUndeadSpawn");
        CScriptThing creature;
        Game->GiveThingBestEnemyTarget(Game->CreateCreature("CREATURE_OK_UNDEAD_LIEUTENANT_AXE_LOW_DETAIL", spawn.GetPos(), "", true), Game->GetHero());
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
