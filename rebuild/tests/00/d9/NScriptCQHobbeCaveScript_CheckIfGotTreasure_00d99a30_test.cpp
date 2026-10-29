// Behaviour test for 0x00D99A30 NScript::CQ_HobbeCaveScript::CheckIfGotTreasure(): the source's model with a scripted
// game interface (region loaded in two windows of frames, the hero opening each chest once) and a thread that terminates.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1) : PStringData((void*)text) { (void)length; }
    ~CCharString() { PStringData = 0; }
};
class CCPPointerInfo { public:                          // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
class CBaseClass { public:                              // polymorphic root of the script handles
    CBaseClass() { }
    virtual ~CBaseClass() { }
};
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    CScriptThing(CScriptThing* impl) { PImp.Data = impl; PImp.Info = 0; }
    CScriptThing(const CScriptThing& other) : CBaseClass() { PImp.Data = other.PImp.Data; PImp.Info = other.PImp.Info; }
    virtual ~CScriptThing() {}            // 0x004AA840 where it stays out of line
    virtual void t001() { } virtual void t002() { } virtual void t003() { } virtual void t004() { } virtual void t005() { } virtual void t006() { } virtual void t007() { } virtual void t008() { } virtual void t009() { } virtual void t010() { } virtual void t011() { }
    virtual void t012() { } virtual void t013() { } virtual void t014() { } virtual void t015() { } virtual void t016() { } virtual void t017() { } virtual void t018() { } virtual void t019() { }
    virtual bool MsgIsUsedBy(const CCharString& user) const   // slot 20 (+0x50) 0x004AAB10
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsUsedBy(user);
    }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase { public:
    virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { }
    virtual void NewScriptFrame() const { }             // slot 7 (+0x1c)
    virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { }
    virtual bool IsRegionLoaded(const CCharString& region) const { (void)region; return false; }
    virtual void v013() { } virtual void v014() { } virtual void v015() { } virtual void v016() { } virtual void v017() { } virtual void v018() { } virtual void v019() { } virtual void v020() { } virtual void v021() { } virtual void v022() { }
    virtual void v023() { } virtual void v024() { } virtual void v025() { } virtual void v026() { } virtual void v027() { } virtual void v028() { } virtual void v029() { } virtual void v030() { } virtual void v031() { } virtual void v032() { }
    virtual void v033() { } virtual void v034() { } virtual void v035() { } virtual void v036() { } virtual void v037() { } virtual void v038() { } virtual void v039() { } virtual void v040() { } virtual void v041() { } virtual void v042() { }
    virtual void v043() { } virtual void v044() { } virtual void v045() { } virtual void v046() { } virtual void v047() { } virtual void v048() { } virtual void v049() { } virtual void v050() { } virtual void v051() { } virtual void v052() { }
    virtual void v053() { } virtual void v054() { } virtual void v055() { } virtual void v056() { } virtual void v057() { } virtual void v058() { } virtual void v059() { } virtual void v060() { } virtual void v061() { } virtual void v062() { }
    virtual void v063() { } virtual void v064() { } virtual void v065() { } virtual void v066() { } virtual void v067() { } virtual void v068() { } virtual void v069() { } virtual void v070() { } virtual void v071() { }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const { (void)name; return CScriptThing((CScriptThing*)0); }
};
class CScriptBase { public:                             // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();
};
class CTimer { public:                                  // 4 bytes: an index into the global script timers
    long TimerIndex;
};
namespace NScript {
class CQ_HobbeCaveScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     PMasterData;              // +0x44
    bool DoneIntro;                                     // +0x48
    bool SavedBeforeCaves;                              // +0x49
    bool BratInCutscene;                                // +0x4a
    bool MissionSucceeded;                              // +0x4b
    bool MissionFailed;                                 // +0x4c
    bool AlarmTriggered;                                // +0x4d
    bool KilledSideRoomHobbes;                          // +0x4e
    bool ReleaseBrat;                                   // +0x4f
    bool BratReturned;                                  // +0x50
    CTimer BanditComment;                               // +0x54
    long GotTreasure;                                   // +0x58
    long BanditHitCount;                                // +0x5c
    bool MakeBanditComment(const CCharString& line, const CScriptThing& bandit);   // 0x00D905C0
    void CheckIfGotTreasure();
};
}

void NScript::CQ_HobbeCaveScript::CheckIfGotTreasure()
{
    while (!IsActiveThreadTerminating()) {
        while (!Game->IsRegionLoaded("HobbeCaveMainChamber")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CScriptThing treasure1 = Game->GetThingWithScriptName("HC_Treasure1");
            CScriptThing treasure2 = Game->GetThingWithScriptName("HC_Treasure2");
            while (Game->IsRegionLoaded("HobbeCaveMainChamber")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                if (treasure1.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating())
                        return;
                    ++GotTreasure;
                    MakeBanditComment("TEXT_QST_036_BANDIT_GOT_TREASURE_ONE", Game->GetThingWithScriptName("BanditSacrifice"));
                }
                if (treasure2.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating())
                        return;
                    ++GotTreasure;
                    MakeBanditComment("TEXT_QST_036_BANDIT_GOT_TREASURE_TWO", Game->GetThingWithScriptName("BanditSacrifice"));
                }
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}

static int gFrame = 0;
static int gTerminateAt = 40;
static int gComments = 0;
static const char* gLastLine = 0;
static int gBanditLookups = 0;

static bool Same(const CCharString& s, const char* text) { return std::strcmp((const char*)s.PStringData, text) == 0; }

class CChest : public CScriptThing { public:
    int UsedAt;
    CChest(int usedAt) : CScriptThing((CScriptThing*)0), UsedAt(usedAt) { }
    virtual bool MsgIsUsedBy(const CCharString& user) const { return gFrame == UsedAt && Same(user, "SCRIPT_NAME_HERO"); }
};
static CChest gChest1(5), gChest2(7), gBandit(-1);

bool CScriptBase::IsActiveThreadTerminating() { return gFrame >= gTerminateAt; }

class CTestGame : public CGameScriptInterfaceBase { public:
    virtual void v000() { }
    virtual void v001() { }
    virtual void v002() { }
    virtual void v003() { }
    virtual void v004() { }
    virtual void v005() { }
    virtual void v006() { }
    virtual void NewScriptFrame() const { ++gFrame; }
    virtual void v008() { } virtual void v009() { } virtual void v010() { } virtual void v011() { }
    virtual bool IsRegionLoaded(const CCharString& region) const
    {
        return Same(region, "HobbeCaveMainChamber") && ((gFrame >= 3 && gFrame < 10) || (gFrame >= 15 && gFrame < 30));
    }
    virtual void v013() { }
    virtual void v014() { }
    virtual void v015() { }
    virtual void v016() { }
    virtual void v017() { }
    virtual void v018() { }
    virtual void v019() { }
    virtual void v020() { }
    virtual void v021() { }
    virtual void v022() { }
    virtual void v023() { }
    virtual void v024() { }
    virtual void v025() { }
    virtual void v026() { }
    virtual void v027() { }
    virtual void v028() { }
    virtual void v029() { }
    virtual void v030() { }
    virtual void v031() { }
    virtual void v032() { }
    virtual void v033() { }
    virtual void v034() { }
    virtual void v035() { }
    virtual void v036() { }
    virtual void v037() { }
    virtual void v038() { }
    virtual void v039() { }
    virtual void v040() { }
    virtual void v041() { }
    virtual void v042() { }
    virtual void v043() { }
    virtual void v044() { }
    virtual void v045() { }
    virtual void v046() { }
    virtual void v047() { }
    virtual void v048() { }
    virtual void v049() { }
    virtual void v050() { }
    virtual void v051() { }
    virtual void v052() { }
    virtual void v053() { }
    virtual void v054() { }
    virtual void v055() { }
    virtual void v056() { }
    virtual void v057() { }
    virtual void v058() { }
    virtual void v059() { }
    virtual void v060() { }
    virtual void v061() { }
    virtual void v062() { }
    virtual void v063() { }
    virtual void v064() { }
    virtual void v065() { }
    virtual void v066() { }
    virtual void v067() { }
    virtual void v068() { }
    virtual void v069() { }
    virtual void v070() { }
    virtual void v071() { }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const
    {
        if (Same(name, "HC_Treasure1")) return CScriptThing(&gChest1);
        if (Same(name, "HC_Treasure2")) return CScriptThing(&gChest2);
        if (Same(name, "BanditSacrifice")) { ++gBanditLookups; return CScriptThing(&gBandit); }
        return CScriptThing((CScriptThing*)0);
    }
};

bool NScript::CQ_HobbeCaveScript::MakeBanditComment(const CCharString& line, const CScriptThing& bandit)
{
    if (bandit.PImp.Data != &gBandit) return false;
    ++gComments;
    gLastLine = (const char*)line.PStringData;
    return true;
}

int main()
{
    CTestGame game;
    NScript::CQ_HobbeCaveScript script;
    script.Game = &game;
    script.GotTreasure = 0;
    script.CheckIfGotTreasure();
    int ok = gFrame == gTerminateAt && script.GotTreasure == 2 && gComments == 2 && gBanditLookups == 2
          && gLastLine != 0 && std::strcmp(gLastLine, "TEXT_QST_036_BANDIT_GOT_TREASURE_TWO") == 0;
    gFrame = 0; gTerminateAt = 0;
    script.GotTreasure = 0;
    script.CheckIfGotTreasure();
    ok = ok && script.GotTreasure == 0 && gFrame == 0;
    if (!ok) { std::printf("00d99a30_TEST FAIL\n"); return 1; }
    std::printf("00d99a30_TEST PASS\n");
    return 0;
}