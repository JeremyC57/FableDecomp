// Behaviour test for 00633f10: the model below is a copy of src/00633f10.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
// test-side stand-in for engine/CPlayer.h (only the members these bodies touch; host layout)
class CGamePlayerInterface; class CWorld;
class CPlayer { public:
    void* __vftable;
    CGamePlayerInterface* PlayerInterface;
    CWorld* World;
    void* PlayerCharacter[2];
    void* InputProcessBoastUI_ptr; void* InputProcessControlCreature_ptr; void* InputProcessRightStickLookAround_ptr;
    void* InputProcessCycleSpecialCameraModes_ptr; void* InputProcessCreatureMovement_ptr;
    void* InputProcessWatchForControlAngleChange_ptr; void* InputProcessControlCreatureActivateZTarget_ptr;
    void* InputProcessFirstPersonTargeting_ptr; void* InputProcessUseRangedWeapon_ptr; void* InputProcessLeftStickLookAround_ptr;
};

typedef long EPlayerMode;
typedef long EControlledMovementType;
class CProcessedInput;
class CBaseObject;

class CAInputProcess { public:
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66();
    virtual void SetFacingAngleYZ(float angle);        // slot 67 (+0x10C)
};

class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60 (PDB +0x70)
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_stateFlags;                       // +0x91 (bit 0: dead)
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
class CThingPlayerCreature : public CThing { public:
    unsigned char           _pad94[0x17c - 0x94];
    EControlledMovementType ControlledMovementType;     // +0x17c (PDB +0x1a4)
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CViewBase { };
namespace NDisplayView {
struct CView3DThirdPerson : CViewBase {                 // 0x9c bytes, vtable 0x0125C7F8
    unsigned char _body[0x9c];
    CView3DThirdPerson();                               // 0x0069E940
};
}
class CDisplayViewManager { public:
    void SetView(CViewBase* view);                      // 0x00640050
};
class CDisplayEngine { public:
    unsigned char        _pad00[0xe0];
    CDisplayViewManager* ViewManager;                   // +0xe0
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* DisplayEngine;                      // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct CPlayer_Methods : CPlayer {
    CIntelligentPointer<CThingPlayerCreature>& PlayerCharacterPointer() { return *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter); }
    CThing* GetHeroThing();                             // 0x00487DC0
    CThing* GetControlledCreature();                    // 0x00487DD0
    void InitModeBoastUI(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeBoastUI(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessBoastUI_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->AddToInterface(PlayerInterface);
    if (PlayerCharacterPointer().GetPItem()) {
        g_pMainGameComponent->DisplayEngine->ViewManager->SetView(new NDisplayView::CView3DThirdPerson);
        PlayerCharacterPointer().GetPItem()->ControlledMovementType = 1;
        if (IsThingAlive(GetControlledCreature()))
            GetHeroThing()->GetPhysicsTC()->SetFacingAngleYZ(0.0f);
    }
}

// ---- test doubles and driver ----
static int g_log[16]; static int g_n = 0;
static void Log(int e) { if (g_n < 16) g_log[g_n++] = e; }
static CGamePlayerInterface* g_seenInterface = 0; static CViewBase* g_view = 0; static int g_views = 0;
static float g_tilt = -1.0f; static int g_tilts = 0;
static CThing* g_controlled = 0;
struct CProc : CAInputProcess { int id; CProc(int i) : id(i) {} bool AddToInterface(CGamePlayerInterface* gpi) { g_seenInterface = gpi; Log(id); return true; } };
void CAInputProcess::VFunc00() {}
bool CAInputProcess::AddToInterface(CGamePlayerInterface*) { return false; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::v00() {}
NDisplayView::CView3DThirdPerson::CView3DThirdPerson() { Log(10); }
void CDisplayViewManager::SetView(CViewBase* v) { g_view = v; ++g_views; Log(11); }
CThing* CPlayer_Methods::GetHeroThing() { return PlayerCharacterPointer().GetPItem(); }
CThing* CPlayer_Methods::GetControlledCreature() { return g_controlled; }
struct CPhys : CTCPhysicsBase { void SetFacingAngleYZ(float a) { g_tilt = a; ++g_tilts; } };
#define V(n) void CTCPhysicsBase::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
V(60) V(61) V(62) V(63) V(64) V(65) V(66)
void CTCPhysicsBase::SetFacingAngleYZ(float) {}
static CDisplayViewManager g_vm; static CDisplayEngine g_de; static CMainGameComponent g_mgc;
CMainGameComponent* g_pMainGameComponent = &g_mgc;
int main()
{
    g_de.ViewManager = &g_vm; g_mgc.DisplayEngine = &g_de;
    static CProc p1(1), p2(2), p3(3), p4(4), p5(5), p6(6);
    static CPhys phys; static CThingPlayerCreature hero; hero.PhysicsTC = &phys;
    static CPlayer_Methods p;
    p.PlayerInterface = (CGamePlayerInterface*)&p;     // any distinct pointer
    p.InputProcessBoastUI_ptr = &p1; p.InputProcessControlCreature_ptr = &p2; p.InputProcessRightStickLookAround_ptr = &p3;
    p.InputProcessCycleSpecialCameraModes_ptr = &p4; p.InputProcessCreatureMovement_ptr = &p5;
    p.InputProcessWatchForControlAngleChange_ptr = &p6;
    bool ok = true;
    p.PlayerCharacterPointer().PItem = 0;        // no character: processes only
    p.InitModeBoastUI(0, 0);
    static const int order1[] = { 1, 2, 3, 4, 5, 6 };
    if (g_n != 6 || std::memcmp(g_log, order1, sizeof(order1)) || g_seenInterface != p.PlayerInterface || g_views) ok = false;
    g_n = 0; p.PlayerCharacterPointer().PItem = (CBaseObject*)&hero; g_controlled = &hero;
    p.InitModeBoastUI(0, 0);
    static const int order2[] = { 1, 2, 3, 4, 5, 6, 10, 11 };
    if (g_n != 8 || std::memcmp(g_log, order2, sizeof(order2)) || g_views != 1 || g_view == 0) ok = false;
    if (hero.ControlledMovementType != 1 || g_tilts != 1 || g_tilt != 0.0f) ok = false;
    hero.ControlledMovementType = 0; hero.m_stateFlags = 1;   // dead controlled creature: no tilt reset
    p.InitModeBoastUI(0, 0);
    if (hero.ControlledMovementType != 1 || g_tilts != 1 || g_views != 2) ok = false;
    hero.m_stateFlags = 0; g_controlled = 0;
    p.InitModeBoastUI(0, 0);
    if (g_tilts != 1) ok = false;
    if (!ok) { std::printf("00633f10_TEST FAIL\n"); return 1; }
    std::printf("00633f10_TEST PASS\n");
    return 0;
}