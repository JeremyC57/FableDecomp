// 00694020 behaviour test: CCreatureActionBase::FrameUpdate (match.cpp body: cut-scene tree member erase,
// LacksTC-headed AddTC retry loop), host stand-ins. C++03, g++ -w.
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }

static long g_FramesPerSecond = 30;
struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
    CCharString(const CCharString& o) : S(o.S) { }
};
struct CTCBase { virtual ~CTCBase() { } };
struct CEventBase { virtual ~CEventBase() { } };
struct CEventCutScene : CEventBase {
    CCharString Name;
    CEventCutScene(const CCharString& name) : Name(name) { }
};
struct CTCEntityEvents : CTCBase {
    std::vector<std::string> Got;
    void AddEvent(CEventBase* event) { Got.push_back(((CEventCutScene*)event)->Name.S); delete event; }
};
struct CCreatureActionBase;
struct SPausePair { CCreatureActionBase* first; long second; SPausePair(CCreatureActionBase* a, long b) : first(a), second(b) { } };
struct CPauseSet {
    std::map<CCreatureActionBase*, long> M;
    unsigned long Erase(CCreatureActionBase* const& key) { return (unsigned long)M.erase(key); }
    void Insert(const SPausePair& v) { M.insert(std::make_pair(v.first, v.second)); }
};
struct CTCHaste : CTCBase {
    CPauseSet Paused;
    int Recalcs;
    CTCHaste() : Recalcs(0) { }
    void Recalculate() { ++Recalcs; }
};
struct CSoundPair { long FirstSound, LastSound; };
struct CSoundInit { bool a; long b, c; CSoundInit(bool x, long y, long z) : a(x), b(y), c(z) { } };
struct CTCSound : CTCBase {
    unsigned long PlayRandomSound(const CSoundPair& s, const CSoundInit& init) { (void)init; return 1000 + s.FirstSound; }
};
struct CInterfaceMap {
    std::map<long, CTCBase*> M;
    CTCBase* FindInline(long key) { return M[key]; }
};
static CTCHaste g_haste;
static int g_addTCFailures = 0;
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CThing() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; }
    void SetTC(long t, CTCBase* c) { m_presentBits[t >> 5] |= 1UL << (t & 31); m_interfaces.M[t] = c; }
    CTCBase* AddTC(const CCharString& name, long a, long b)
    {
        (void)a; (void)b; logs("AddTC " + name.S);
        if (g_addTCFailures > 0) { --g_addTCFailures; return 0; }
        SetTC(0x8c, &g_haste); return &g_haste;
    }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    bool LacksTC(long type) const { if (HasTC(type)) return false; return true; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = (T*)m_interfaces.M[interfaceType];
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
struct CActionEventPointer { long Frame; bool Done; };
struct CDispatchActionEventIfActivationFrame {
    long Frame; CCreatureActionBase* Action;
    CDispatchActionEventIfActivationFrame(long f, CCreatureActionBase* a) : Frame(f), Action(a) { }
};
struct CIsActionEventDone { bool b; };
struct SRandomTag { };
static CDispatchActionEventIfActivationFrame For_each(CActionEventPointer* first, CActionEventPointer* last,
                                                      CDispatchActionEventIfActivationFrame f)
{
    for (; first != last; ++first)
        if (first->Frame == f.Frame) { first->Done = true; logs("dispatch"); }
    return f;
}
static CActionEventPointer* Find_if(CActionEventPointer* first, CActionEventPointer* last, CIsActionEventDone, const SRandomTag&)
{
    while (first != last && !first->Done) ++first;
    return first;
}
static CActionEventPointer* Remove_copy_if(CActionEventPointer* first, CActionEventPointer* last, CActionEventPointer* out,
                                           CIsActionEventDone)
{
    for (; first != last; ++first) if (!first->Done) *out++ = *first;
    return out;
}
struct CActionEventVector {
    CActionEventPointer* Start;
    CActionEventPointer* Finish;
    void Erase(CActionEventPointer* first, CActionEventPointer* last)
    {
        CActionEventPointer* out = first;
        for (CActionEventPointer* p = last; p != Finish; ++p) *out++ = *p;
        Finish = out;
    }
};
struct SCutSceneNode {
    SCutSceneNode* Parent;
    SCutSceneNode* Left;
    SCutSceneNode* Right;
    long Frame;
    CCharString Name;
    SCutSceneNode(long f, const char* n) : Parent(0), Left(0), Right(0), Frame(f), Name(n) { }
};
// stand-in tree: a sorted chain through Right, the header's Left is the leftmost node
static SCutSceneNode* Rebalance_for_erase(SCutSceneNode* z, SCutSceneNode*& root, SCutSceneNode*& leftmost, SCutSceneNode*& rightmost)
{
    (void)root;
    if (leftmost == z) leftmost = z->Right;
    if (rightmost == z) rightmost = 0;
    return z;
}
static int g_freed = 0;
struct SCutSceneTree {
    SCutSceneNode* _M_header;
    unsigned long _M_node_count;
    void erase(SCutSceneNode* pos)
    {
        SCutSceneNode* erased = Rebalance_for_erase(pos, _M_header->Parent, _M_header->Left, _M_header->Right);
        if (erased) { delete erased; ++g_freed; }
        --_M_node_count;
    }
};

struct CCreatureActionBase {
    CBaseIntelligentPointer PCreature;
    long TotalTime, ActionTime, FramesRunning;
    CSoundPair Sounds;
    unsigned long PlayingSoundID;
    float RunningTimeSeconds, LastRunningTimeSeconds;
    long StartFrame, PauseFrame, PauseTimer;
    bool AddedActionAnim, Finished, PerformedAction;
    long* PAnimEntry;
    CActionEventVector ActionEvents;
    SCutSceneTree CutSceneEvents;
    float Progress, SpeedScale;
    long AnimLength;
    int Performed, Finishes, Movements, AnimUpdates;
    void PerformAction() { ++Performed; logs("perform"); }
    void Finish() { ++Finishes; logs("finish"); }
    long GetAnimationLength() { return AnimLength; }
    void SetProgress(float p) { Progress = p; }
    void UpdateAnimation() { ++AnimUpdates; }
    void UpdateActionMovement() { ++Movements; }
    float GetSpeedScale() { return SpeedScale; }
    void FrameUpdate();
};

void CCreatureActionBase::FrameUpdate()
{
    LastRunningTimeSeconds = RunningTimeSeconds;
    if (RunningTimeSeconds == 0.0f && TotalTime == 0)
        SetProgress(0.0f);
    else
        SetProgress(RunningTimeSeconds / TotalTime);
    For_each(ActionEvents.Start, ActionEvents.Finish, CDispatchActionEventIfActivationFrame(FramesRunning, this));
    {
        CActionEventPointer* last = ActionEvents.Finish;
        CIsActionEventDone pred;
        pred.b = false;
        CActionEventPointer* first = Find_if(ActionEvents.Start, last, pred, SRandomTag());
        if (first != last)
            first = Remove_copy_if(first + 1, last, first, pred);
        ActionEvents.Erase(first, last);
    }
    UpdateAnimation();
    if (FramesRunning >= GetAnimationLength() + StartFrame && !Finished)
        UpdateActionMovement();
    if (TotalTime >= 0 && FramesRunning >= TotalTime && !Finished) {
        Finished = true;
        Finish();
        return;
    }
    FramesRunning = (long)(g_FramesPerSecond * RunningTimeSeconds);
    if (PauseTimer-- == 0) {
        CTCHaste* haste;
        if (PCreature.GetPItem()->GetInterface(&haste, 0x8c)) {
            CCreatureActionBase* key = this;
            haste->Paused.Erase(key);
            haste->Recalculate();
        }
    }
    if (FramesRunning == PauseFrame) {
        PauseFrame = -1;
        PauseTimer = (long)((1.0f - GetSpeedScale()) * 5.0f);
        CThing* creature;
        while ((creature = PCreature.GetPItem())->LacksTC(0x8c)) {
            CCharString name("CTCHaste");
            PCreature.GetPItem()->AddTC(name, 0, 0);
        }
        CTCHaste* haste = (CTCHaste*)creature->m_interfaces.FindInline(0x8c);
        haste->Paused.Insert(SPausePair(this, 0));
        haste->Recalculate();
    }
    if (ActionTime >= 0 && FramesRunning >= ActionTime && !PerformedAction &&
        (AddedActionAnim || PAnimEntry == 0 || *PAnimEntry == 0)) {
        PerformAction();
        PerformedAction = true;
        CTCSound* sound;
        if (PCreature.GetPItem()->GetInterface(&sound, 0x10))
            PlayingSoundID = sound->PlayRandomSound(Sounds, CSoundInit(true, 0, 0));
    }
    while (CutSceneEvents._M_node_count != 0) {
        SCutSceneNode* node = CutSceneEvents._M_header->Left;
        if (FramesRunning < node->Frame)
            break;
        CThing* creature = PCreature.GetPItem();
        if (creature->m_presentBits[0x42 >> 5] & (1UL << (0x42 & 31))) {
            CTCEntityEvents* events = (CTCEntityEvents*)creature->m_interfaces.FindInline(0x42);
            events->AddEvent(new CEventCutScene(CutSceneEvents._M_header->Left->Name));
        }
        CutSceneEvents.erase(CutSceneEvents._M_header->Left);
    }
    if (TotalTime >= 0 && FramesRunning > TotalTime)
        FramesRunning = TotalTime;
}

static void init(CCreatureActionBase& a, CThing* c, SCutSceneNode* header)
{
    a.PCreature.Item = c;
    a.TotalTime = 100; a.ActionTime = -1; a.FramesRunning = 0;
    a.Sounds.FirstSound = 7; a.Sounds.LastSound = 9; a.PlayingSoundID = 0;
    a.RunningTimeSeconds = 0.0f; a.LastRunningTimeSeconds = 0.0f;
    a.StartFrame = 0; a.PauseFrame = -1; a.PauseTimer = -5;
    a.AddedActionAnim = false; a.Finished = false; a.PerformedAction = false; a.PAnimEntry = 0;
    a.ActionEvents.Start = a.ActionEvents.Finish = 0;
    a.CutSceneEvents._M_header = header; a.CutSceneEvents._M_node_count = 0;
    a.Progress = -1.0f; a.SpeedScale = 1.0f; a.AnimLength = 1000;
    a.Performed = a.Finishes = a.Movements = a.AnimUpdates = 0;
}

int main()
{
    SCutSceneNode header(0, "header");
    CThing creature;
    CTCEntityEvents events;
    CTCSound sound;
    CCreatureActionBase a;

    // progress, frame count from running time, event dispatch and removal
    init(a, &creature, &header);
    CActionEventPointer evs[3] = { { 0, false }, { 5, false }, { 0, false } };
    a.ActionEvents.Start = evs; a.ActionEvents.Finish = evs + 3;
    a.RunningTimeSeconds = 1.0f;
    g_log = ""; a.FrameUpdate();
    CHECK(a.Progress == 1.0f / 100);
    CHECK(a.LastRunningTimeSeconds == 1.0f);
    CHECK(a.FramesRunning == 30);
    CHECK(a.ActionEvents.Finish - a.ActionEvents.Start == 1 && evs[0].Frame == 5);
    CHECK(a.AnimUpdates == 1 && a.Movements == 0);

    // zero time and zero total: progress 0; reaching TotalTime finishes and returns before the frame update
    init(a, &creature, &header); a.TotalTime = 0;
    a.FrameUpdate();
    CHECK(a.Progress == 0.0f && a.Finished && a.Finishes == 1 && a.FramesRunning == 0);
    a.FrameUpdate();
    CHECK(a.Finishes == 1);

    // animation end runs the movement update
    init(a, &creature, &header); a.AnimLength = 0; a.FrameUpdate();
    CHECK(a.Movements == 1);

    // pause frame: AddTC retried until the haste TC exists, then paused; the timer releases it later
    init(a, &creature, &header); a.PauseFrame = 30; a.RunningTimeSeconds = 1.0f; a.SpeedScale = 0.5f;   // (1 - 0.5) * 5 = 2.5 exactly: no x87 vs SSE rounding difference
    g_addTCFailures = 2; g_log = "";
    a.FrameUpdate();
    CHECK(g_log == "AddTC CTCHaste;AddTC CTCHaste;AddTC CTCHaste;");
    CHECK(a.PauseFrame == -1 && a.PauseTimer == 2);
    CHECK(g_haste.Paused.M.count(&a) == 1 && g_haste.Recalcs == 1);
    a.FrameUpdate(); a.FrameUpdate();
    CHECK(a.PauseTimer == 0 && g_haste.Paused.M.count(&a) == 1);
    a.FrameUpdate();
    CHECK(a.PauseTimer == -1 && g_haste.Paused.M.count(&a) == 0 && g_haste.Recalcs == 2);
    // TC already present: no AddTC
    a.PauseFrame = 30; g_log = ""; a.FrameUpdate();
    CHECK(g_log == "" && g_haste.Paused.M.count(&a) == 1);

    // action time: perform once, with the sound when the creature has one
    CThing c2; c2.SetTC(0x10, &sound);
    init(a, &c2, &header); a.ActionTime = 10; a.RunningTimeSeconds = 0.5f;
    a.FrameUpdate();
    CHECK(a.Performed == 1 && a.PerformedAction && a.PlayingSoundID == 1007);
    a.FrameUpdate();
    CHECK(a.Performed == 1);
    long animEntry = 1;
    init(a, &c2, &header); a.ActionTime = 10; a.RunningTimeSeconds = 0.5f; a.PAnimEntry = &animEntry;
    a.FrameUpdate();
    CHECK(a.Performed == 0);
    a.AddedActionAnim = true; a.FrameUpdate();
    CHECK(a.Performed == 1);

    // cut-scene events: due ones go to the entity events (if the creature has them) and are erased
    CThing c3; c3.SetTC(0x42, &events);
    init(a, &c3, &header); a.RunningTimeSeconds = 0.5f;           // frame 15
    SCutSceneNode* n1 = new SCutSceneNode(5, "CS_A");
    SCutSceneNode* n2 = new SCutSceneNode(15, "CS_B");
    SCutSceneNode* n3 = new SCutSceneNode(16, "CS_C");
    n1->Right = n2; n2->Right = n3;
    header.Left = n1; header.Right = n3; a.CutSceneEvents._M_node_count = 3;
    g_freed = 0;
    a.FrameUpdate();
    CHECK(events.Got.size() == 2 && events.Got[0] == "CS_A" && events.Got[1] == "CS_B");
    CHECK(a.CutSceneEvents._M_node_count == 1 && header.Left == n3 && g_freed == 2);
    // without the events TC the due node is still erased
    init(a, &creature, &header); a.CutSceneEvents._M_node_count = 1; a.RunningTimeSeconds = 1.0f;
    a.FrameUpdate();
    CHECK(a.CutSceneEvents._M_node_count == 0 && g_freed == 3 && events.Got.size() == 2);

    // clamp to TotalTime
    init(a, &creature, &header); a.TotalTime = 20; a.RunningTimeSeconds = 2.0f;
    a.FrameUpdate();
    CHECK(a.FramesRunning == 20);

    if (!g_fail) printf("00694020_TEST PASS\n");
    return g_fail;
}