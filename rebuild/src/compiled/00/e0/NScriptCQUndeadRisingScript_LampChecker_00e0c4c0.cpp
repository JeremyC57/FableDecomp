// CQ_UndeadRisingScript::LampChecker() -- each time the graveyard is (re)loaded, recreate the light effect at every activated lamp and note the lamps whose light did not survive; while any lamp is out and the graveyard stays loaded, retry the missing lights once a second until all are back
// Leaf: LampChecker
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0C4C0, 1197 bytes (int3 padded to 1200); /GS cookie (bool LampOut[8] array).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, ~CScriptThing 0x004AA840,
//   CScriptThing::IsAlive 0x004AB130 (called at the first site, inlined at the second), __security_check_cookie 0x00BFE9F9;
//   Game slots 7, 11, 100 CreateEffect, 376 Pause.
// Layout (retail = Ego_r.pdb - 0x14): LightsActivated[8] +0x54, activation_points +0x60 (STLport vector<CScriptThing>).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImp {
    virtual void i00(); virtual void i01(); virtual void i02(); virtual void i03(); virtual void i04(); virtual void i05(); virtual void i06(); virtual void i07(); virtual void i08(); virtual void i09();
    virtual void i10(); virtual void i11(); virtual void i12(); virtual void i13(); virtual void i14(); virtual void i15(); virtual void i16(); virtual void i17(); virtual void i18(); virtual void i19();
    virtual void i20(); virtual void i21(); virtual void i22(); virtual void i23(); virtual void i24(); virtual void i25(); virtual void i26(); virtual void i27(); virtual void i28(); virtual void i29();
    virtual void i30(); virtual void i31(); virtual void i32(); virtual void i33(); virtual void i34(); virtual void i35(); virtual void i36(); virtual void i37(); virtual void i38(); virtual void i39();
    virtual void i40(); virtual void i41(); virtual void i42(); virtual void i43(); virtual void i44(); virtual void i45(); virtual void i46(); virtual void i47(); virtual void i48(); virtual void i49();
    virtual void i50(); virtual void i51(); virtual void i52(); virtual void i53(); virtual void i54(); virtual void i55(); virtual void i56(); virtual void i57(); virtual void i58(); virtual void i59();
    virtual void i60(); virtual void i61(); virtual void i62(); virtual void i63(); virtual void i64(); virtual void i65(); virtual void i66(); virtual void i67(); virtual void i68(); virtual void i69();
    virtual void i70(); virtual void i71(); virtual void i72(); virtual void i73(); virtual void i74();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010();
    virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030();
    virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050();
    virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060();
    virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070();
    virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const                        // slot 75 (+0x12c) 0x004AB130
    {
        if (!PImpData)
            return false;
        return PImpData->IsAlive();
    }
    CScriptThingImp* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                    // 0x004AA840
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_UndeadRisingScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned long number_of_zombie_tokens_in_right_area;    // +0x48
    unsigned long number_of_boss_kills_in_right_area;   // +0x4c
    bool HeroReachedCentre;                             // +0x50
    bool BossCutscenePlayed;                            // +0x51
    bool MissionSucceeded;                              // +0x52
    bool MissionFailed;                                 // +0x53
    bool LightsActivated[8];                            // +0x54
    long quest_status;                                  // +0x5c
    std::vector<CScriptThing> activation_points;        // +0x60
    void LampChecker();
};
} // namespace NScript
using NScript::CQ_UndeadRisingScript;

void NScript::CQ_UndeadRisingScript::LampChecker()
{
    bool lampOut[8];
    while (!Game->IsLevelLoaded("GraveYard_2")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    for (long i = 0; i < 8; ++i) {
        if (IsActiveThreadTerminating())
            return;
        lampOut[i] = false;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (Game->IsLevelLoaded("GraveYard_2") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("GraveYard_2")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        bool anyOut = false;
        for (long i = 0; i < 8; ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (LightsActivated[i]) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing light = Game->CreateEffect("GRAVEYARD_CIRCLE_LIGHT_SMALL", activation_points[i].GetPos(), "", 0.0f, false, false);
#pragma inline_depth(0)
                if (!light.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        return;
                    lampOut[i] = true;
                    anyOut = true;
                }
            }
        }
        if (IsActiveThreadTerminating())
            return;
        while (anyOut && Game->IsLevelLoaded("GraveYard_2")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(1.0f);
            anyOut = false;
            for (long i = 0; i < 8; ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (lampOut[i]) {
                    if (IsActiveThreadTerminating())
                        return;
                    CScriptThing light = Game->CreateEffect("GRAVEYARD_CIRCLE_LIGHT_SMALL", activation_points[i].GetPos(), "", 0.0f, false, false);
                    if (!light.IsAlive()) {
                        if (IsActiveThreadTerminating())
                            return;
                        anyOut = true;
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        lampOut[i] = false;
                    }
                }
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}