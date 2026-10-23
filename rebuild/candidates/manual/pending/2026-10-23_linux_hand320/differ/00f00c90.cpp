// COracleSpectator::Main() -- Oracle quest spectator: register the alive filter, make Me unpushable with the passive brain override and a 0..10 wander around its spot (state group 4); each frame, when the hero talks to it, take scripted control, start a movie, speak the next "TEXT_QST_B03_SPECTATOR_<name>_<n>" line (n cycles 10, 20, 30, 40) while alive, then release control
// Grade: DIFFER(1190v1203) -- flow, VPAD slots, implicit handle dtor inlined only at the for(;;) exit all match; left: VC puts the paused game pointer in ebx (retail ebp, re-zeroed after the movie dtor) so the GetHealth>0 bool spills to [esp+0x13] (retail bl), frame 0x4c vs 0x48
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F00C90, 1216 bytes (+ int3 padding), plain `ret`.
// Callees: the landed CDF_ShineLight::Main 0x00E6EA30 machinery (CIsThingAlive 0x012C32F0 / 0x00E241B0, CScriptThing copy 0x004ABE90
//   / dtor 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, handle 0x0099A380 / 0x0099A430 / 0x00CD23B9
//   / 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0,
//   movie dtor 0x006E7B80), GFIntToCharString 0x0099F830, operator+ 0x0099F690 / 0x0099F600 / 0x0099F570, operator const char*
//   0x0099E4C0; GameCalls slots 7, 8, 70 GetHero, 264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities,
//   589 SetCreatureBrain, 763 SetWanderCentrePoint, 766 SetWanderMinDistance, 769 SetWanderMaxDistance, 784 SetScriptingStateGroup,
//   844 SetIsPushableByHero; CScriptThing slots 3 GetDataString, 6 GetPos, 27 MsgIsTalkedToBy.
// Data: "BRAIN_PASSIVE_OVERRIDE" 0x012D921C, "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E, "_" 0x01244DB4,
//   "TEXT_QST_B03_SPECTATOR_" 0x012F7290, 0.0f 0x0122DEDC.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, line index +0x1c (descriptive).
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    operator const char*() const;                      // 0x0099E4C0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
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
struct CScriptThingImpl;
struct CBaseClass {                                     // vtable 0x01231710
    virtual ~CBaseClass();                              // 0x0099A2E0
    __forceinline CBaseClass() { }
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
struct CScriptThing : CBaseClass {                     // vtable 0x01238C8C, 12 bytes
    VPAD2(ma)                                          // slots 1..2
    virtual CCharString GetDataString() const;         // slot 3 (+0x0c)
    VPAD2(mb)                                          // slots 4..5
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    VPAD16(mc) VPAD4(md)                               // slots 7..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    CCountedPointer<CScriptThingImpl> PImp;            // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    struct InlineCopy { };
    __forceinline CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing();                                   // 0x004AA840
};
struct CIsThingAlive {                                 // vtable 0x012C32F0
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                  // 0x00E241B0
};
struct CBaseIntelligentPointer {                       // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                       // +0x04
    CBaseIntelligentPointer();                         // 0x0099A380
    virtual ~CBaseIntelligentPointer();                // 0x0099A430
};
struct CScriptGameResourceObjectBase : CBaseIntelligentPointer {   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                        // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    CScriptThing GetScriptThing() const;               // 0x007E7490
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
    bool IsPerformingScriptTask() const;               // 0x007E7450
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4, 0x10 bytes
    CCountedPointer<void> PImp;                        // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectMovieBase() { }
    ~CScriptGameResourceObjectMovieBase();             // 0x006E7B80
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CGameScriptInterface {
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD128(ra) VPAD64(rb) VPAD1(rc)                                                 // slots 71..263
    virtual float GetHealth(const CScriptThing& thing) const;                        // slot 264 (+0x420)
    VPAD64(ua) VPAD32(ub) VPAD8(uc) VPAD1(ud)                                        // slots 265..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD8(va)                                                                        // slots 371..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5ec)
    VPAD128(wa) VPAD64(wb) VPAD16(wc) VPAD1(we)                             // slots 380..588
    virtual void SetCreatureBrain(const CScriptThing& thing, const CCharString& brain) const;   // slot 589 (+0x934)
    VPAD128(xa) VPAD32(xb) VPAD8(xc) VPAD4(xd) VPAD1(xe)                             // slots 590..762
    virtual void SetWanderCentrePoint(CScriptThing thing, C3DVector pos);            // slot 763 (+0xbec)
    VPAD2(ya)                                                                        // slots 764..765
    virtual void SetWanderMinDistance(CScriptThing thing, float distance);           // slot 766 (+0xbf8)
    VPAD2(yb)                                                                        // slots 767..768
    virtual void SetWanderMaxDistance(CScriptThing thing, float distance);           // slot 769 (+0xc04)
    VPAD8(yc) VPAD4(yd) VPAD2(ye)                                                    // slots 770..783
    virtual void SetScriptingStateGroup(CScriptThing thing, long group);             // slot 784 (+0xc40)
    VPAD32(za) VPAD16(zb) VPAD8(zc) VPAD2(zd) VPAD1(ze)                              // slots 785..843
    virtual void SetIsPushableByHero(CScriptThing thing, bool pushable);             // slot 844 (+0xd30)
};
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct COracleSpectator : CEntityScriptBase {
    void* ParentClass;                                 // +0x14
    void* PMasterData;                                 // +0x18
    long  LineIndex;                                   // +0x1c
    void Main();
};

void COracleSpectator::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase spectator;
    Game->SetIsPushableByHero(CScriptThing(Me, CScriptThing::InlineCopy()), false);
    Game->SetCreatureBrain(Me, "BRAIN_PASSIVE_OVERRIDE");
    Game->SetWanderCentrePoint(CScriptThing(Me, CScriptThing::InlineCopy()), Me.GetPos());
    Game->SetWanderMinDistance(CScriptThing(Me, CScriptThing::InlineCopy()), 0.0f);
    Game->SetWanderMaxDistance(CScriptThing(Me, CScriptThing::InlineCopy()), 10.0f);
    Game->SetScriptingStateGroup(CScriptThing(Me, CScriptThing::InlineCopy()), 4);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            if (spectator.IsScripting())
                spectator.Release();
            while (!Game->StartScriptingEntity(Me, spectator, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
            }
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(spectator.GetScriptThing()) > g_fZero) {
                spectator.Speak(Game->GetHero(), "TEXT_QST_B03_SPECTATOR_" + Me.GetDataString() + "_" + GFIntToCharString(LineIndex),
                                (ETextGroupSelectionMethod)0, false, true, false);
                while (spectator.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            }
            if (LineIndex == 40) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                LineIndex = 10;
            } else {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                LineIndex += 10;
            }
            if (spectator.IsScripting())
                spectator.Release();
            game->PauseAllNonScriptedEntities(false);
        }
        Game->NewScriptFrame();
        continue;
    }
}
