// Behaviour test for 00e32200: the model below is a copy of src/00e32200.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int g_frames = 0, g_termAt = 1000, g_completeAt = 1000, g_aliveUntil = 1000, g_cleared = 0, g_impDestroyed = 0, g_infoDeleted = 0;
struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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

struct CBaseClass {                                      // polymorphic root of the script handles
    CBaseClass() {}
    virtual ~CBaseClass() {}
};

struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}
    virtual void v01() const {}
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
};
struct CScriptThingImp : CScriptThing {
    virtual bool IsAlive() const { return g_frames < g_aliveUntil; }
};
struct CGameScriptInterface {
    virtual void v000() {}
    virtual void NewScriptFrame() { ++g_frames; }
    virtual void ClearThingHasInformation(const CScriptThing& t) { ++g_cleared; if (!t.PImp.Data) g_cleared += 100; }
};
struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

struct CV_AmbushScamScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    bool QuestComplete;                                  // +0x49
    bool QuestActivated;                                 // +0x4a
    bool LeftRegion;                                     // +0x4b
    bool MoralitySwitch;                                 // +0x4c
    bool BuddyIntroComplete;                             // +0x4d
    bool DoTricksterIntro;                               // +0x4e
    bool DoAmbush;                                       // +0x4f
    bool TricksterApproachComplete;                      // +0x50
    bool TricksterSpeechComplete;                        // +0x51
    bool TricksterRetreatComplete;                       // +0x52
    bool TricksterQuestComplete;                         // +0x53
    bool TricksterWhiningComplete;                       // +0x54
    bool TricksterDeadOrLeft;                            // +0x55
    bool TricksterComplete;                              // +0x56
    void WatchForTricksterComplete(CScriptThing trickster);
};
static CV_AmbushScamScript* g_script = 0;
bool CScriptBase::IsActiveThreadTerminating() { if (g_frames >= g_completeAt) g_script->TricksterComplete = true; return g_frames >= g_termAt; }

void CV_AmbushScamScript::WatchForTricksterComplete(CScriptThing trickster)
{
    while (!TricksterComplete) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (trickster.IsAlive() && !IsActiveThreadTerminating())
        GSI->ClearThingHasInformation(trickster);
}

// ---- test doubles and driver ----
static void __fastcall DestroyImp(void* data) { delete (CScriptThingImp*)data; ++g_impDestroyed; }

static bool Run(int termAt, int completeAt, int aliveUntil, bool startComplete, int expectFrames, int expectCleared)
{
    static CGameScriptInterface gsi;
    CV_AmbushScamScript s; std::memset(&s, 0, sizeof(s)); s.GSI = &gsi; s.TricksterComplete = startComplete;
    g_script = &s; g_frames = 0; g_termAt = termAt; g_completeAt = completeAt; g_aliveUntil = aliveUntil; g_cleared = g_impDestroyed = g_infoDeleted = 0;
    {
        CScriptThingImp* imp = new CScriptThingImp; imp->PImp.Data = 0; imp->PImp.Info = 0;
        CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DestroyImp; info->Data = imp;
        CScriptThing holder; holder.PImp.Data = imp; holder.PImp.Info = info;
        CScriptThing arg; arg.PImp.Data = imp; arg.PImp.Info = info; ++info->RefCount;
        s.WatchForTricksterComplete(arg);
        arg.PImp.Data = 0; arg.PImp.Info = 0;           // the callee's parameter copy released one reference
        if (g_impDestroyed != 0 || g_infoDeleted != 0) return false;
    }
    if (g_impDestroyed != 1 || g_infoDeleted != 1) return false;
    if (g_frames != expectFrames || g_cleared != expectCleared) return false;
    return true;
}

int main()
{
    bool ok = true;
    ok = ok && Run(1000, 3, 1000, false, 3, 1);      // complete on frame 3, alive: marker cleared
    ok = ok && Run(1000, 1000, 1000, true, 0, 1);    // already complete: no frame, cleared
    ok = ok && Run(2, 1000, 1000, false, 2, 0);      // terminated while waiting: nothing
    ok = ok && Run(4, 4, 1000, false, 4, 0);         // complete and terminating on the same frame: nothing
    ok = ok && Run(1000, 3, 2, false, 3, 0);         // complete but the trickster is dead: nothing
    if (!ok) { std::printf("00e32200_TEST FAIL\n"); return 1; }
    std::printf("00e32200_TEST PASS\n");
    return 0;
}