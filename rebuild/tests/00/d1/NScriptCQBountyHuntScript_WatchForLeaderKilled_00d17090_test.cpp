// Behaviour test for 00d17090: the model below is a copy of src/00d17090.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int g_ctors = 0, g_dtors = 0, g_badKiller = 0, g_frames = 0, g_termAt = 1000, g_killedAt = 1000, g_decapAt = 1000;
static int g_impDestroyed = 0, g_infoDeleted = 0;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1) { ++g_ctors; if (std::strcmp(text, "") != 0 || length != -1) g_badKiller = 1; PStringData = (void*)text; }
    ~CCharString() { ++g_dtors; }
};

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

class CBaseClass { public:
    virtual ~CBaseClass() {}
};

class CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointerScriptThing()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
                ++g_infoDeleted;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    virtual ~CScriptThing() {}
    virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {}
    virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {}
    virtual bool MsgIsKilledBy(const CCharString& killer) const     // slot 18 (+0x48)
    {
        if (PImp.Data)
            return PImp.Data->MsgIsKilledBy(killer);
        return false;
    }
};
struct CScriptThingImp : CScriptThing {                 // the game-side implementation the counted pointer owns
    virtual bool MsgIsKilledBy(const CCharString& killer) const
    {
        if (std::strcmp((const char*)killer.PStringData, "") != 0) g_badKiller = 1;
        return g_frames >= g_killedAt;
    }
};

class CGameScriptInterface { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const { ++g_frames; }  // slot 7 (+0x1c)
};

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0xd9];
    bool          BountyHuntDecapitation;               // +0xd9
};

struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating() { if (g_frames >= g_decapAt) PMasterDataHook()->BountyHuntDecapitation = true; return g_frames >= g_termAt; }
    CQ_SunnyvaleMasterData* PMasterDataHook();
};

class CQ_BountyHuntScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    unsigned char _flags048[7];                         // +0x48 .. +0x4f (QuestFailed .. VillagerIntroDone)
    bool LeaderKilled;                                  // +0x4f
    void WatchForLeaderKilled(CScriptThing leader);
};
CQ_SunnyvaleMasterData* CScriptBase::PMasterDataHook() { return ((CQ_BountyHuntScript*)this)->PMasterData; }

void CQ_BountyHuntScript::WatchForLeaderKilled(CScriptThing leader)
{
    for (;;) {
        bool waiting = !(leader.MsgIsKilledBy("") || PMasterData->BountyHuntDecapitation);
        if (!waiting)
            break;
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    LeaderKilled = true;
}
}

// ---- test doubles and driver ----
static void __fastcall DestroyImp(void* data) { delete (CScriptThingImp*)data; ++g_impDestroyed; }

static bool Run(int termAt, int killedAt, int decapAt, bool nullLeader, int expectFrames, bool expectKilled)
{
    static CGameScriptInterface gsi;
    NScript::CQ_SunnyvaleMasterData master; std::memset(&master, 0, sizeof(master));
    NScript::CQ_BountyHuntScript s; std::memset(s._body, 0, sizeof(s._body)); std::memset(s._flags048, 0, sizeof(s._flags048));
    s.GSI = &gsi; s.PMasterData = &master; s.LeaderKilled = false;
    g_frames = 0; g_termAt = termAt; g_killedAt = killedAt; g_decapAt = decapAt; g_ctors = g_dtors = g_badKiller = 0; g_impDestroyed = g_infoDeleted = 0;
    {
        CScriptThing leader;
        leader.PImp.Data = 0; leader.PImp.Info = 0;
        if (!nullLeader) {
            CScriptThingImp* imp = new CScriptThingImp; imp->PImp.Data = 0; imp->PImp.Info = 0;
            CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DestroyImp; info->Data = imp;
            leader.PImp.Data = imp; leader.PImp.Info = info;
            // the by-value argument copy shares the block
            CScriptThing copy; copy.PImp.Data = imp; copy.PImp.Info = info; ++info->RefCount;
            s.WatchForLeaderKilled(copy);
            copy.PImp.Data = 0; copy.PImp.Info = 0;   // the callee's parameter copy released one reference
            if (g_impDestroyed != 0 || g_infoDeleted != 0) return false;   // the leader local still holds the block
        } else {
            s.WatchForLeaderKilled(leader);
        }
    }
    if (!nullLeader && (g_impDestroyed != 1 || g_infoDeleted != 1)) return false;   // released once the local dies
    if (g_frames != expectFrames) return false;
    if (s.LeaderKilled != expectKilled) return false;
    if (g_ctors != g_dtors || g_badKiller) return false;
    return true;
}

int main()
{
    bool ok = true;
    ok = ok && Run(1000, 3, 1000, false, 3, true);     // leader killed on frame 3: flag set
    ok = ok && Run(1000, 1000, 2, false, 2, true);     // decapitation on frame 2 (seen through the master data): flag set
    ok = ok && Run(4, 1000, 1000, false, 4, false);    // thread terminated while waiting: nothing set
    ok = ok && Run(5, 5, 1000, false, 5, false);       // killed and terminating on the same frame: the post-loop test refuses
    ok = ok && Run(1000, 0, 1000, false, 0, true);     // already dead: no frame waited
    ok = ok && Run(1000, 1000, 3, true, 3, true);      // null leader: MsgIsKilledBy is false until decapitation
    if (!ok) { std::printf("00d17090_TEST FAIL\n"); return 1; }
    std::printf("00d17090_TEST PASS\n");
    return 0;
}