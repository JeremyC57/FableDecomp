// Grade: DIFFER(1144v1144) -- same length; only the AXE branch (twice) picks edx/eax differently for the Game vtable loads around SetCreatureCreationDelayFrames + CreateCreature
// CM_UndeadHere::Main() -- haunted-house undead spawner: wait for the parent's start flag; then until the parent's stop flag,
//   while the parent's spawn count is under its maximum, spawn (at most 10 times while the hero is over 3 units away, else after
//   a 1-second pause re-checking the parent's limits) an undead sword or axe soldier "HHUndead" at Me, and wait for it to die
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95770, 1144 bytes, plain `ret`, no EH frame.
// Callees: CBase ctor 0x0099A2D0 / dtor 0x0099A2E0, ~CScriptThing 0x004AA840, CScriptThing::operator= 0x008AB980,
//   IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall), rand 0x00BFEB16, CCharString
//   0x0099EBF0 / 0x0099EAE0, operator delete 0x00BFE9BC; GameCalls slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature,
//   95 SetCreatureCreationDelayFrames, 96 ResetCreatureCreationDelayFrames, 376 Pause; CScriptThing slots 6 GetPos, 75 IsAlive.
// Data: "HHUndead" 0x012ECC08, "CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH" 0x012ECCFC, "CREATURE_OK_UNDEAD_SOLDIER_AXE_HH" 0x012ECCD8.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (stop flag +0x50, start flag +0x53, spawned +0x54,
//   maximum +0x58; descriptive), PMasterData +0x18, own spawn counter +0x1c.
// Shape: no inline_depth pragmas: the per-function inline budget alone puts the temporaries' ~CScriptThing out of line and keeps the wait-loop
//   return's expanded; Me / the undead are reached through reference helpers, so GetPos / IsAlive stay
//   virtual calls.
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC
extern "C" int __cdecl rand();                          // 0x00BFEB16
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    __forceinline ~CCountedPointer()
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
struct CScriptThingImp;
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
struct CScriptThing : CBase {                           // vtable 0x01238C8C, 12 bytes
    VPAD4(t1) VPAD1(t2)                                                  // slots 1..5
    virtual const C3DVector& GetPos() const;                             // slot 6 (+0x18)
    VPAD64(t7)                                                           // slots 7..70
    VPAD4(t8)                                                            // slots 71..74
    virtual bool IsAlive() const;                                        // slot 75 (+0x12c)
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    CScriptThing() { }
    ~CScriptThing() { }                                 // out of line 0x004AA840
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};
inline const C3DVector& PositionOf(const CScriptThing& thing) { return thing.GetPos(); }
inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
struct CGameScriptInterface {
    VPAD4(p0) VPAD2(p1) VPAD1(p2)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(q0) VPAD16(q1) VPAD8(q2) VPAD4(q3) VPAD2(q4)                              // slots 8..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD16(r0) VPAD4(r1)                                                             // slots 71..90
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& pos, const CCharString& scriptName, bool b) const;   // slot 91 (+0x16c)
    VPAD2(r2) VPAD1(r3)                                                              // slots 92..94
    virtual void SetCreatureCreationDelayFrames(long frames);                        // slot 95 (+0x17c)
    virtual void ResetCreatureCreationDelayFrames();                                 // slot 96 (+0x180)
    VPAD256(s0) VPAD16(s1) VPAD4(s2) VPAD2(s3) VPAD1(s4)                             // slots 97..375
    virtual void Pause(float seconds) const;                                         // slot 376 (+0x5e0)
};
struct CHauntedHouseScript {
    unsigned char _pad[0x50];
    bool          StopSpawning;                         // +0x50
    unsigned char _pad51[2];
    bool          StartSpawning;                        // +0x53
    long          UndeadSpawned;                        // +0x54
    long          MaxUndead;                            // +0x58
};
struct CM_UndeadHere {
    void*                 __vftable;                    // +0x00
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    CHauntedHouseScript*  ParentClass;                  // +0x14
    void*                 PMasterData;                  // +0x18
    long                  SpawnCount;                   // +0x1c
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CM_UndeadHere::Main()
{
    while (!ParentClass->StartSpawning) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing undead;
    while (!ParentClass->StopSpawning) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->UndeadSpawned < ParentClass->MaxUndead) {
            if (IsActiveThreadTerminating()) return;
            if (SpawnCount < 10 && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 3.0f)) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->UndeadSpawned++;
                if (rand() % 2 == 0) {
                    if (IsActiveThreadTerminating()) return;
                    Game->SetCreatureCreationDelayFrames(1);
                    undead = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH", PositionOf(Me), "HHUndead", true);
                } else {
                    if (IsActiveThreadTerminating()) return;
                    Game->SetCreatureCreationDelayFrames(1);
                    undead = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_AXE_HH", PositionOf(Me), "HHUndead", true);
                }
                Game->ResetCreatureCreationDelayFrames();
                SpawnCount++;
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->Pause(1.0f);
                if (ParentClass->UndeadSpawned < ParentClass->MaxUndead && !ParentClass->StopSpawning
                    && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 3.0f)) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->UndeadSpawned++;
                    if (rand() % 2 == 0) {
                        if (IsActiveThreadTerminating()) return;
                        Game->SetCreatureCreationDelayFrames(1);
                        undead = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH", PositionOf(Me), "HHUndead", true);
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->SetCreatureCreationDelayFrames(1);
                        undead = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_AXE_HH", PositionOf(Me), "HHUndead", true);
                    }
                    Game->ResetCreatureCreationDelayFrames();
                    SpawnCount++;
                }
            }
            while (IsThingAlive(undead)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (IsActiveThreadTerminating()) return;
}
