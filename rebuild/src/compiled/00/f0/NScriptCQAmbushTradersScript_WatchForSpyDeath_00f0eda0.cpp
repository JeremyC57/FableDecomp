// NScript::CQ_AmbushTradersScript::WatchForSpyDeath(CScriptThing spy) -- the spawned "WatchForSpyDeath" thread of the Ambush Traders quest: every script frame ask the spy whether it was killed; once it is, flag SpyDead on the quest and AmbushTradersSpyDead in the Sunnyvale master data; stop when the thread is terminated
// Leaf: ?WatchForSpyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F0EDA0, 230 bytes, thiscall, `ret 0Ch` (the by-value CScriptThing, destroyed by the callee).
// Identity (proven): the row's one reference is the CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing> member-pointer store
//   `mov [esi+34h], 0F0EDA0h` at 0x00F0D7B9 (spawn vtable 0x012F7EBC, 0x48-byte object, the CScriptThing argument copied to
//   +0x3c), right after the thread name "ParentClass." 0x012C9934 + "WatchForSpyDeath" 0x012F843C; Ego_r.pdb and FableWin.pdb
//   ?WatchForSpyDeath@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z. The members it sets are Ego_r
//   CQ_AmbushTradersScript::SpyDead (+0x6c - 0x14 = +0x58) and CQ_SunnyvaleMasterData::AmbushTradersSpyDead (+0xd4).
//   Catalog label sub_00f0eda0 (gap scan).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0, ~CCharString
//   0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptThing slot 18 MsgIsKilledBy (inline wrapper over
//   PImp.Data's slot 18); Game slot 7 NewScriptFrame.
// Data: "" 0x0122D70E; CScriptThing vtable 0x01238C8C (stored by the inline dtor at both exits).
// Layout: CQ_AmbushTradersScript at Ego_r - 0x14 (as the landed WatchForEnemyDeath 0x00F08590): Game +0x40, MasterScriptData
//   +0x44, SpyDead +0x58.
// Shape: `for (;;)` with the termination return at the top (VC7.1 rotates it as retail does and sinks both returns' inline
//   dtor copies to the end in source order; `while (!IsActiveThreadTerminating())` defers only the arm's return copy, so the
//   two copies came out swapped, 230v230); the script IF macro's termination test opens the arm. CScriptThing's virtual dtor and the counted pointer's dtor are `__forceinline` (both exits inline the release).
#include <string.h>
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    __forceinline virtual ~CScriptThing() {}            // 0x004AA840 where it stays out of line
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006();
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012();
    virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const   // slot 18 (+0x48) 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
};
class CScriptBase { public:                             // retail 0x40 bytes
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData* MasterScriptData;                            // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData { public:                  // Ego_r.pdb layout up to the member used
    unsigned char _before[0xd4];
    bool AmbushTradersSpyDead;                          // +0xd4
};
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFailed;                                   // +0x48
    bool QuestFinished;                                 // +0x49
    bool QuestSucceeded;                                // +0x4a
    bool QuestOver;                                     // +0x4b
    bool RegionInvalid;                                 // +0x4c
    bool BanditIntroDone;                               // +0x4d
    bool TraderEscaped;                                 // +0x4e
    bool NoEnemiesLeft;                                 // +0x4f
    bool PhaseComplete;                                 // +0x50
    bool ConvoyTrigger2;                                // +0x51
    bool AllBanditsDead;                                // +0x52
    bool AllTradersDead;                                // +0x53
    bool AllGuardsDead;                                 // +0x54
    bool ContactDead;                                   // +0x55
    bool ContactAttackedByHero;                         // +0x56
    bool HeroDead;                                      // +0x57
    bool SpyDead;                                       // +0x58
    void WatchForSpyDeath(CScriptThing spy);
};

void CQ_AmbushTradersScript::WatchForSpyDeath(CScriptThing spy)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (spy.MsgIsKilledBy("")) {
            if (IsActiveThreadTerminating())
                return;
            SpyDead = true;
            MasterScriptData->AmbushTradersSpyDead = true;
        }
        Game->NewScriptFrame();
    }
}
}   // namespace NScript