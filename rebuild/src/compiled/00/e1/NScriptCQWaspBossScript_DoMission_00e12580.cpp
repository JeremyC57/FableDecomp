// CQ_WaspBossScript::DoMission() -- register the post-wasp-boss gossip (rumour, picnic-area faction), spawn five hornet drones at the queen's deposit markers, gather the drones, chasers and attackers, run the wasp intro and wait until they are all dead; pause, create the queen hornet at the starting marker, start the guildmaster-help thread and, unless the mission failed, run the wasp-menace phase
// Leaf: ?DoMission@CQ_WaspBossScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E12580, 1446 bytes (int3 padded to 1456).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CDefString::operator CCharString 0x00415D70, IntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, IsActiveThreadTerminating 0x00CB7940, CBaseClass 0x0099A2D0 / ~
//   0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (and inline), AreAllThingsInVectorDead 0x00CBED00,
//   operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012E0104, GuildmasterHelp 0x00E12B30),
//   AddSpawnedFunction 0x00CB7E50, WaspIntro 0x00E12F20, WaspMenace 0x00E13310, free 0x00BFEA14, ~vector<CScriptThing>
//   0x008AC970 (and inline); pGlobalScriptDef 0x0143E90C (CDefStrings +0xE3C hornet drone, +0xE40 queen hornet);
//   Game slots 7, 72, 75, 91, 376, 717, 718, 722.
// Layout (retail = Ego_r.pdb - 0x14): MissionFailed +0x49, QueenHornetAttacks +0x4b.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define VEC_DTOR
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
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
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
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl {
    virtual void i000();
    virtual void i001();
    virtual void i002();
    virtual void i003();
    virtual void i004();
    virtual void i005();
    virtual const C3DVector& GetPos() const;            // imp slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015(); virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069();
    virtual void SetToKillOnLevelUnload(bool kill);     // imp slot 70 (+0x118)
};
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const             // slot 6 (+0x18) 0x004AA980
    {
        if (!PImp.Data)
            return g_ZeroVector;
        return PImp.Data->GetPos();
    }
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void SetToKillOnLevelUnload(bool kill)      // slot 70 (+0x118) 0x004AB0C0
    {
        if (PImp.Data)
            PImp.Data->SetToKillOnLevelUnload(kill);
    }
    virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04 Data, +0x08 Info
#ifdef THING_INLINE_COPY
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
#else
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
#endif
    CScriptThing() {}
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    ~CScriptThing() {}
};
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
#ifndef VEC_DTOR
#define VEC_DTOR __forceinline
#endif
    VEC_DTOR ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
CCharString __fastcall GFIntToCharString(long);                              // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
class CDefString { public:
    long Index;
    operator CCharString() const;                       // 0x00415D70
};
class CScriptDef { public:
    unsigned char _pad[0xe3c];
    CDefString CREATURE_HORNET_DRONE;                   // +0xe3c
    CDefString CREATURE_QUEEN_HORNET;                   // +0xe40
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
bool __fastcall AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);   // 0x00CBED00
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBindingBase;
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const CCharString& regionName = CCharString("")) { AddSpawnedFunction(func, regionName); }
};
namespace NScript {
class CQ_WaspBossScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool DronesCreated;                                 // +0x4a
    bool QueenHornetAttacks;                            // +0x4b
    void DoMission();
    void GuildmasterHelp();                             // 0x00E12B30
    void WaspIntro();                                   // 0x00E12F20
    void WaspMenace();                                  // 0x00E13310
};
} // namespace NScript
using NScript::CQ_WaspBossScript;

void NScript::CQ_WaspBossScript::DoMission()
{
    Game->AddRumourCategory("Post waspboss killed");
    Game->AddNewRumourToCategory("Post waspboss killed", "TEXT_AI_GOSSIP_WASPBOSS_KILLED");
    Game->AddGossipFactionToCategory("Post waspboss killed", "FACTION_PICNIC_AREA");
    CScriptThing hornet;
    for (long i = 1; i < 6; ++i) {
        if (IsActiveThreadTerminating())
            return;
        hornet = Game->CreateCreature(pGlobalScriptDef->CREATURE_HORNET_DRONE, Game->GetThingWithScriptName("QueenDepositPos" + GFIntToCharString(i)).GetPos(), "HornetDrone", false);
    }
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> wasps;
    Game->GetAllThingsWithScriptName("HornetDrone", wasps);
    Game->GetAllThingsWithScriptName("WaspChaser", wasps);
    Game->GetAllThingsWithScriptName("WaspAttacker", wasps);
    WaspIntro();
    while (!AreAllThingsInVectorDead(wasps)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(4.0f);
    Game->CreateCreature(pGlobalScriptDef->CREATURE_QUEEN_HORNET, Game->GetThingWithScriptName("MK_WQ_STARTING").GetPos(), "QueenHornet", false);
#pragma inline_depth()
    QueenHornetAttacks = true;
    SpawnFunction(new CSpawnedFunc<CQ_WaspBossScript>("GuildmasterHelp", 0, this, &NScript::CQ_WaspBossScript::GuildmasterHelp));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed)
        WaspMenace();
#pragma inline_depth(0)
}
#pragma inline_depth()