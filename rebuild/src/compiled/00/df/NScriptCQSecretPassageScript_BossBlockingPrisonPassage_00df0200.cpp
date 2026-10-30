// CQ_SecretPassageScript::BossBlockingPrisonPassage() -- each time the hero reaches the passage entrance with the exterior loaded: start the door switcher once, make the entrance unusable, spawn an undead lieutenant at every boss spawn point (each targeting the hero), block the doors until all of them are dead, then unblock and make the entrance usable again
// Leaf: BossBlockingPrisonPassage
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF0200, 1174 bytes (int3 padded to 1184).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A,
//   CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012DDC54), AddSpawnedFunction 0x00CB7E50, SwitchDoorsOnAndOff 0x00DEFB10,
//   CBaseClass 0x0099A2D0, ~CScriptThing 0x004AA840, CScriptThing::operator= 0x008AB980, ~vector<CScriptThing> 0x008AC970,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, AreAllThingsInVectorDead 0x00CBED00; Game slots 7, 11, 70, 72, 75, 91, 447, 451.
// Layout (retail = Ego_r.pdb - 0x14): DoorsBlocked +0x4c, DoorsBeingDone +0x4d.
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
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"

struct __declspec(novtable) CEntityScriptBindingBase {
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    __forceinline CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    void* PImpData;
    void* PImpInfo;
    CScriptThing() : PImpData(0), PImpInfo(0) {}
    virtual ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                          // 0x008AC970
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);                         // 0x00CBED00
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SecretPassageScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    bool MotherFound;                                   // +0x4b
    bool DoorsBlocked;                                  // +0x4c
    bool DoorsBeingDone;                                // +0x4d
    void BossBlockingPrisonPassage();
    void SwitchDoorsOnAndOff();                         // 0x00DEFB10
};
} // namespace NScript
using NScript::CQ_SecretPassageScript;

void NScript::CQ_SecretPassageScript::BossBlockingPrisonPassage()
{
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        while (!Game->IsLevelLoaded("PrisonSecretPassageExterior")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!DoorsBeingDone) {
            if (IsActiveThreadTerminating())
                return;
            AddSpawnedFunction(new CSpawnedFunc<CQ_SecretPassageScript>("SwitchDoorsOnAndOff", 0, this, &NScript::CQ_SecretPassageScript::SwitchDoorsOnAndOff), CCharString(""));
            DoorsBeingDone = true;
        }
        CScriptThing entrance = Game->GetThingWithScriptName("EntranceToPrisonSecretPassage");
        CScriptThing boss;
        CScriptThing unused;
        std::vector<CScriptThing> spawnPoints;
        Game->GetAllThingsWithScriptName("SP_UndeadBossSpawnPoint", spawnPoints);
        Game->SetThingAsUsable(entrance, false);
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), entrance, 3.0f) && Game->IsLevelLoaded("PrisonSecretPassageExterior")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (Game->IsLevelLoaded("PrisonSecretPassageExterior")) {
            if (IsActiveThreadTerminating())
                return;
            DoorsBlocked = true;
            for (unsigned int i = 0; i < spawnPoints.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                boss = Game->CreateCreature("CREATURE_OK_UNDEAD_LIEUTENANT_AXE", spawnPoints[i].GetPos(), "UndeadBoss", true);
                Game->GiveThingBestEnemyTarget(boss, Game->GetHero());
                spawnPoints[i] = boss;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!AreAllThingsInVectorDead(spawnPoints)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            DoorsBlocked = false;
            Game->SetThingAsUsable(entrance, true);
            return;
        }
    }
}