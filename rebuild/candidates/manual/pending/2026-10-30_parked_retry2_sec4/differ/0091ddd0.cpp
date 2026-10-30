// CAIStateGroup_HoverAllAroundTest::Update() -- test state: while the player creature lives, hover to a random point within ten units of the start position (the away-from-player direction rotated by a random angle), clamped above the ground; terminate when a move fails
// Grade: DIFFER(ours v retail) -- 515v626, nd 135->124 (retry wave 2 sec4): GFROR13 respelled long __fastcall
//   GFROR13(unsigned long) (Ego_r/FableWin ?GFROR13@@YIJK@Z), IsDead accessor, the rotated offset built as a C3DVector
//   before the distance scale. Left: retail's 8-aligned frame with a memory double angle temp and its fcos/fsin x87
//   schedule
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091DDD0, length 626 bytes (+ int3 padding).
// Callees: GetPWorld @ 0x008FCFC0, CWorld::GetPlayerManager @ 0x0049D9D0, CPlayerManager::GetMainPlayerCreature @ 0x00487DC0,
// CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10,
// CThing::GetPosition @ 0x004C73D0, C3DVector::Normalise @ 0x00A14440, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890,
// _CIfmod @ 0x00BFEB28, GFFloatRandom @ 0x004978A0, PeekWorldMap @ 0x008FCFF0, CWorldMap::GetGroundSizeZAt @ 0x0063A300,
// AIAction_MoveToPos @ 0x0097EC00, Terminate @ 0x008FCF40.
#include <math.h>
#pragma intrinsic(cos, sin, fmod)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                           // 0x00A14440
};
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    void* vftable;
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
extern void* const CIntelligentPointer_CThing_vftable;   // 0x01238C6C
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item) { *(void* const**)this = &CIntelligentPointer_CThing_vftable; SetPItem(item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CPlayerManager {
    CThing* GetMainPlayerCreature();            // 0x00487DC0
};
struct CWorld {
    CPlayerManager* GetPlayerManager();         // 0x0049D9D0
};
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector& pos);   // 0x0063A300
};
long __fastcall GFROR13(unsigned long value);        // 0x00497890
struct CRandomSeed {
    unsigned long Seed;
    float GFFloatRandom(float range);           // 0x004978A0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    CRandomSeed* GetPWorldSeed();               // 0x008FCFD0
    CWorldMap* PeekWorldMap();                  // 0x008FCFF0
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool& moving, long anim, bool lookAround, bool b6,
                            const CThing* ignoreThing, bool b8, float speedScale);   // 0x0097EC00
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_HoverAllAroundTest : CAIStateGroupBase {
    void Update();
};

void CAIStateGroup_HoverAllAroundTest::Update()
{
    CIntelligentPointerThing player(GetPWorld()->GetPlayerManager()->GetMainPlayerCreature());
    C3DVector start = GetCreature()->GetPosition();
    while (IS_THING_ALIVE(player)) {
        const C3DVector& me = GetCreature()->GetPosition();
        const C3DVector& them = player.GetPItem()->GetPosition();
        C3DVector dir(them.X - me.X, them.Y - me.Y, 0.0f);
        dir.Normalise();
        float x = -dir.X;
        float y = dir.Y;
        CRandomSeed* seed = GetPWorldSeed();
        seed->Seed = seed->Seed * 0x24A1 + 0x24DF;
        seed->Seed = GFROR13(seed->Seed);
        float fraction = (float)(fmod((double)seed->Seed, 65536.0) * (1.0 / 65536.0));
        float distance = GetPWorldSeed()->GFFloatRandom(10.0f);
        double angle = fraction * 6.2831854820251465;
        C3DVector offset(x * (float)cos(angle) - y * (float)sin(angle), x * (float)sin(angle) + y * (float)cos(angle), 0.0f);
        C3DVector target(start.X + offset.X * distance, start.Y + offset.Y * distance, start.Z + offset.Z * distance);
        float ground = PeekWorldMap()->GetGroundSizeZAt(target);
        if (target.Z < ground)
            target.Z = ground;
        bool moving;
        if (AIAction_MoveToPos(1, target, moving, 3, true, true, 0, true, 0.0f) != 2) {
            Terminate();
            break;
        }
    }
}
