// Grade: DIFFER(1986v1906) -- region-2 respawn temporaries: retail calls ~CScriptThing 0x004AA840 out of line (per-function inline budget) while every other inline still expands; plus the inline angle's fstp is scheduled after the next null test in retail
// CMagicBarrier::Main() -- the Darkwood barrier: find the "BarrierFX" marker, spawn the two idle red forcefield halves on it;
//   while the parent quest's flag is clear respawn both whenever both are gone, then look up "DarkwoodRockTroll" and keep
//   respawning while the flag stays set; finally remove the halves, play the two "_OFF" effects at the marker and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E03F70, 1906 bytes; entity-script Main (no EH frame).
// Callees: CCharString 0x0099EBF0 / 0x0099EAE0, CBaseClass dtor 0x0099A2E0, ~CScriptThing 0x004AA840, CScriptThing::GetAngleXY
//   0x004AAA40, IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC; GameCalls slots 7 NewScriptFrame,
//   72 GetThingWithScriptName, 100 CreateEffect, 108 RemoveThing; thing impl slots 6 GetPos, 10 GetAngleXY, 75 IsAlive.
// Data: "BarrierFX" 0x012DF704, "DarkwoodRockTroll" 0x012DF378, "NEW_RED_FORCEFIELD_IDLE_01/02" 0x012C354C / 0x012C3530,
//   "..._OFF" 0x012C3470 / 0x012C3450, "" 0x0122D70E, zero vector 0x0143E8E0.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (barrier flag +0x55, descriptive).
// Shape: sibling of CMagicBarrier::Main 0x00DA4E30 (same file model). The loop spawns inline the handle angle with a 0.0f
//   empty-handle value (AngleOrZero, as the landed CTollBanditScript 0x00E4E0E0 models it) while the "_OFF" lines call the
//   out-of-line GetAngleXY 0x004AAA40 (-1.0f when empty); the second loop and tail sit under `inline_depth(0)`, so the
//   respawn temporaries there call ~CScriptThing out of line.
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
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThingImp {
    virtual void v00() const; virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetAngleXY() const;                    // slot 10 (+0x28)
    VPAD64(i11_0)                                        // slots 11..74
    virtual bool IsAlive() const;                        // slot 75 (+0x12c)
};
extern const C3DVector g_ZeroPos;                        // 0x0143E8E0
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
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
struct CBaseClass {
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing() { }                    // out of line 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const { return PImp.Data == 0 ? g_ZeroPos : PImp.Data->GetPos(); }   // slot 6
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetAngleXY() const;                                                                    // slot 10, 0x004AAA40
    float AngleOrZero() const { return PImp.Data == 0 ? 0.0f : PImp.Data->GetAngleXY(); }
    VPAD64(t11_0)                                                                                        // slots 11..74
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
struct CGameScriptInterface {                              // vtable 0x01260F0C
    VPAD4(p0_0) VPAD2(p0_1) VPAD1(p0_2)                                          // slots 0..6
    virtual void NewScriptFrame() const;                                         // slot 7 (+0x1c)
    VPAD64(p8_0)                                                                 // slots 8..71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;  // slot 72 (+0x120)
    VPAD16(p73_0) VPAD8(p73_1) VPAD2(p73_2) VPAD1(p73_3)                         // slots 73..99
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& pos, const CCharString& bone, float angle, bool b1, bool b2) const;   // slot 100 (+0x190)
    VPAD4(p101_0) VPAD2(p101_1) VPAD1(p101_2)                                    // slots 101..107
    virtual void RemoveThing(const CScriptThing& thing, bool unused, bool fade) const;   // slot 108 (+0x1b0)
    VPAD128(p109_0) VPAD32(p109_1) VPAD4(p109_2) VPAD1(p109_3)                   // slots 109..273
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& sound, bool important, bool blocking) const;   // slot 274 (+0x448)
    VPAD128(p275_0) VPAD64(p275_1) VPAD4(p275_2) VPAD2(p275_3) VPAD1(p275_4)     // slots 275..473
    virtual void EntitySetFacingAngle(const CScriptThing& entity, float angle, bool b) const;   // slot 474 (+0x768)
};
inline bool BothGone(const CScriptThing& a, const CScriptThing& b) { return !a.IsAlive() && !b.IsAlive(); }
struct CBarrierQuest {
    unsigned char _pad[0x55];
    bool          BarrierFlag;                         // +0x55 (descriptive)
};
struct CMagicBarrier {
    void*                 __vftable;                   // +0x00
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    CBarrierQuest*        ParentClass;                 // +0x14
    void*                 PMasterData;                 // +0x18
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CMagicBarrier::Main()
{
    CScriptThing fx = Game->GetThingWithScriptName("BarrierFX");
    CScriptThing barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", fx.GetPos(), "", fx.AngleOrZero(), false, false);
    CScriptThing barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", fx.GetPos(), "", fx.AngleOrZero(), false, false);
    while (!ParentClass->BarrierFlag) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (BothGone(barrier1, barrier2)) {
            if (IsActiveThreadTerminating()) return;
            barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", fx.GetPos(), "", fx.AngleOrZero(), false, false);
            barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", fx.GetPos(), "", fx.AngleOrZero(), false, false);
        }
    }
    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    CScriptThing troll = Game->GetThingWithScriptName("DarkwoodRockTroll");
    while (ParentClass->BarrierFlag) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (BothGone(barrier1, barrier2)) {
            if (IsActiveThreadTerminating()) return;
            barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", fx.GetPos(), "", fx.AngleOrZero(), false, false);
            barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", fx.GetPos(), "", fx.AngleOrZero(), false, false);
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(barrier1, false, true);
    Game->RemoveThing(barrier2, false, true);
#pragma inline_depth(0)
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01_OFF", fx.GetPos(), "", fx.GetAngleXY(), false, false);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02_OFF", fx.GetPos(), "", fx.GetAngleXY(), false, false);
    Game->RemoveThing(Me, false, true);
}
#pragma inline_depth()
