// Grade: DIFFER(1117v1117) -- same length; in the two attacker wait loops retail hoists lea ecx,[attacker] above the terminate jne (the return jumps one instruction into the shared cleanup)
// CLostTraderBanditSpawn::Main() -- lost-trader bandit spawner: spawn a random bandit at Me and wait for it to die; wait
//   until Me is off camera and the hero over 15 units away, spawn a second bandit set on the hero and wait for it to die;
//   wait for the parent's go flag and the same off-camera condition, spawn another bandit on the hero and wait for it to
//   die; once the hero is within 30 units spawn a last bandit and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA5230, 1117 bytes, plain `ret`, no EH frame.
// Callees: CV_LostTraderScript::GetRandomBanditName 0x00EA5690, CScriptThing::operator= 0x008AB980 / ~CScriptThing 0x004AA840,
//   CBase dtor 0x0099A2E0, IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall),
//   CCharString 0x0099EBF0 / 0x0099EAE0, operator delete 0x00BFE9BC; GameCalls slots 7 NewScriptFrame, 70 GetHero,
//   91 CreateCreature, 108 RemoveThing, 421 IsCameraPosOnScreen, 451 GiveThingBestEnemyTarget; CScriptThing slots 6 GetPos,
//   75 IsAlive (the first wait tests the impl directly).
// Data: "" 0x0122D70E.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_LostTraderScript; go flag +0x4c, descriptive).
// Shape: the CM_UndeadHere::Main 0x00E95770 model (reference helpers keep GetPos / IsAlive virtual).
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
struct CScriptThingImp {
    VPAD64(i0) VPAD8(i1) VPAD2(i2) VPAD1(i3)                             // slots 0..74
    virtual bool IsAlive() const;                                        // slot 75 (+0x12c)
};
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
inline bool IsThingImpAlive(const CScriptThing& thing) { return thing.PImp.Data && thing.PImp.Data->IsAlive(); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CGameScriptInterface {
    VPAD4(p0) VPAD2(p1) VPAD1(p2)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    VPAD32(q0) VPAD16(q1) VPAD8(q2) VPAD4(q3) VPAD2(q4)                              // slots 8..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD16(r0) VPAD4(r1)                                                             // slots 71..90
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& pos, const CCharString& scriptName, bool b) const;   // slot 91 (+0x16c)
    VPAD16(r2)                                                                       // slots 92..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;       // slot 108 (+0x1b0)
    VPAD256(s0) VPAD32(s1) VPAD16(s2) VPAD8(s3)                                      // slots 109..420
    virtual bool IsCameraPosOnScreen(const C3DVector& pos) const;                    // slot 421 (+0x694)
    VPAD16(s5) VPAD8(s6) VPAD4(s7) VPAD1(s8)                                         // slots 422..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
};
struct CV_LostTraderScript {
    unsigned char _pad[0x4c];
    bool          SpawnNextBandit;                      // +0x4c (descriptive)
    CCharString GetRandomBanditName();                  // 0x00EA5690
};
struct CLostTraderBanditSpawn {
    void*                 __vftable;                    // +0x00
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    CV_LostTraderScript*  ParentClass;                  // +0x14
    void*                 PMasterData;                  // +0x18
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CLostTraderBanditSpawn::Main()
{
    CScriptThing bandit = Game->CreateCreature(ParentClass->GetRandomBanditName(), PositionOf(Me), "", false);
    while (IsThingImpAlive(bandit)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (Game->IsCameraPosOnScreen(PositionOf(Me)) || IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing attacker = Game->CreateCreature(ParentClass->GetRandomBanditName(), PositionOf(Me), "", false);
    Game->GiveThingBestEnemyTarget(attacker, Game->GetHero());
    while (IsThingAlive(attacker)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass->SpawnNextBandit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (Game->IsCameraPosOnScreen(PositionOf(Me)) || IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    attacker = Game->CreateCreature(ParentClass->GetRandomBanditName(), PositionOf(Me), "", false);
    Game->GiveThingBestEnemyTarget(attacker, Game->GetHero());
    while (IsThingAlive(attacker)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 30.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    attacker = Game->CreateCreature(ParentClass->GetRandomBanditName(), PositionOf(Me), "", false);
    Game->RemoveThing(Me, false, true);
}
