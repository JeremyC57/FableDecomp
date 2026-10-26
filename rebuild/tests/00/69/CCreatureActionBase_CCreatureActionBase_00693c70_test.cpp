// Behaviour test for CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long)
// (0x00693C70): field defaults, the inline empty map header, the default animation entry and the action-id counter.
#include <cstdio>
#include <cstdlib>

class CThing { public: int id; };
class CThingCreatureBase : public CThing { public:};
class CBaseObject { public: void* IntelligentPointer; CBaseObject() : IntelligentPointer(0) {} virtual ~CBaseObject() {} };
struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    void SetPItem(CThing* item) { PItem = item; }
};
struct CCreaturePointer : CBaseIntelligentPointer {
    CCreaturePointer(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
struct CThingPointer : CBaseIntelligentPointer { CThingPointer() : CBaseIntelligentPointer() {} };
class CAnimationEntry { public:
    long a, b, c, type;
    CAnimationEntry(long a_, long b_, long c_, long t) : a(a_), b(b_), c(c_), type(t) {}
};
class CCPPointerInfo { public: long Count; };
struct CAnimationEntryPointer {
    CAnimationEntry* Data; CCPPointerInfo* Info;
    CAnimationEntryPointer() : Data(0), Info(0) {}
    void Reset(CAnimationEntry* entry) { Data = entry; }
};
class CSoundPair { public: long FirstSound, LastSound; };
class C3DVector { public: float X, Y, Z; C3DVector() {} };
struct _Rb_tree_node_base { char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
struct SCutSceneEventNode : _Rb_tree_node_base { long first; void* second; };
struct CCutSceneEventMap {
    SCutSceneEventNode* _M_header; unsigned long _M_node_count;
    CCutSceneEventMap() : _M_header(0)
    {
        _M_header = (SCutSceneEventNode*)std::malloc(sizeof(SCutSceneEventNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
template <class T> struct CVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {} };

class CCreatureActionBase : public CBaseObject { public:
    CCreaturePointer PCreature;
    long TotalTime, ActionTime, AnimDelayTime, FramesRunning, InterruptionGroupID, InterruptionPriority, ActionID;
    CThingPointer PItemRemoved;
    CSoundPair Sounds;
    unsigned long PlayingSoundID;
    float RunningTimeSeconds, LastRunningTimeSeconds;
    long StartFrame, PauseFrame, PauseTimer;
    C3DVector AnimMovementStep;
    bool AddedActionAnim, Finished, PerformedAction, Initialised;
    long NextMovementPosWorldFrame;
    C3DVector NextMovementPos;
    CAnimationEntryPointer PAnimEntry, PPreceedingPAnimEntry;
    CVector<int> ActionEvents;
    CCutSceneEventMap CutSceneEvents;
    CVector<CThingPointer> DependentThings;
    static long NextActionID;
    CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long interruptionGroupID, long interruptionPriority);
};
long CCreatureActionBase::NextActionID = 100;

CCreatureActionBase::CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long interruptionGroupID, long interruptionPriority)
    : PCreature(creature)
    , TotalTime(1)
    , ActionTime(1)
    , AnimDelayTime(0)
    , FramesRunning(0)
    , InterruptionGroupID(interruptionGroupID)
    , InterruptionPriority(interruptionPriority)
    , ActionID(0)
    , Sounds(sounds)
    , PlayingSoundID(0)
    , RunningTimeSeconds(0.0f)
    , LastRunningTimeSeconds(0.0f)
    , StartFrame(0)
    , PauseFrame(-1)
    , PauseTimer(-1)
    , AddedActionAnim(false)
    , Finished(false)
    , PerformedAction(false)
    , Initialised(false)
    , NextMovementPosWorldFrame(0)
{
    PAnimEntry.Reset(new CAnimationEntry(0, 0, 0, 2));
    ActionID = NextActionID;
    ++NextActionID;
}

int main()
{
    CThingCreatureBase hero;
    CSoundPair sounds = { 7, 9 };
    CCreatureActionBase a(&hero, sounds, 3, 11);
    CCreatureActionBase b(&hero, sounds, 4, 12);
    bool ok = a.PCreature.PItem == &hero && a.PItemRemoved.PItem == 0 && a.TotalTime == 1 && a.ActionTime == 1
        && a.AnimDelayTime == 0 && a.InterruptionGroupID == 3 && a.InterruptionPriority == 11
        && a.Sounds.FirstSound == 7 && a.Sounds.LastSound == 9 && a.PauseFrame == -1 && a.PauseTimer == -1
        && !a.Finished && a.PAnimEntry.Data != 0 && a.PAnimEntry.Data->type == 2 && a.PAnimEntry.Info == 0
        && a.PPreceedingPAnimEntry.Data == 0 && a.ActionEvents._M_start == 0 && a.DependentThings._M_finish == 0
        && a.CutSceneEvents._M_node_count == 0 && a.CutSceneEvents._M_header->_M_left == a.CutSceneEvents._M_header
        && a.ActionID == 100 && b.ActionID == 101 && CCreatureActionBase::NextActionID == 102;
    if (!ok) { std::printf("00693c70_TEST FAIL\n"); return 1; }
    std::printf("00693c70_TEST PASS\n");
    return 0;
}