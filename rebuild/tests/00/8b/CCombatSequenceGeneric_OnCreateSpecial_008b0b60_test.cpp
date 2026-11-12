// Behaviour test for CCombatSequenceGeneric::OnCreateSpecial (0x008B0B60): a copy of the model with small stand-in containers.
#include <cstdio>
#include <cstring>

static int g_liveInfos = 0;
static int g_deletedActions = 0;
static int g_deletedSequences = 0;

class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};

template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (Data) {
            Info = new CCPPointerInfo;
            Info->RefCount = 1; Info->DeleteFunc = DeleteData; Info->Data = Data;
            ++g_liveInfos;
        }
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer() { ReleaseData(); }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            ReleaseData();
            Data = other.Data;
            Info = other.Info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
                --g_liveInfos;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *operator->(); }
    static void DeleteData(void* data) { delete static_cast<T*>(data); }
};

class CThingCreatureBase { public: int Id; };
enum EActionRegisteredType { ACTION_NULL = 0, ACTION_COMBAT_IDLE = 21, ACTION_COMBAT_ATTACK_MAIN = 25 };

class CCombatActionBase { public:
    EActionRegisteredType Type;
    CThingCreatureBase* Creature;
    CThingCreatureBase* Target;
    ~CCombatActionBase() { ++g_deletedActions; }
    static CCountedPointer<CCombatActionBase> GetPCombatActionFromEnum(EActionRegisteredType type, CThingCreatureBase& creature,
                                                                      CThingCreatureBase& target)
    {
        if (type == ACTION_NULL)
            return CCountedPointer<CCombatActionBase>();
        CCombatActionBase* action = new CCombatActionBase;
        action->Type = type; action->Creature = &creature; action->Target = &target;
        return CCountedPointer<CCombatActionBase>(action);
    }
};

class CCombatAction { public: CCountedPointer<CCombatActionBase> CreatureAction; };

struct CActionVector {                                     // stand-in for std::vector<CCombatAction>
    CCombatAction Items[8];
    int Count;
    CActionVector() : Count(0) { }
    void push_back(const CCombatAction& x) { Items[Count++] = x; }
    CActionVector& operator=(const CActionVector& other)
    {
        int i;
        for (i = 0; i < other.Count; ++i) Items[i] = other.Items[i];
        for (; i < Count; ++i) Items[i] = CCombatAction();
        Count = other.Count;
        return *this;
    }
};

class CCombatSequenceDef { public:
    const char* InstantiationName;
    EActionRegisteredType ActionList[4];
    int ActionCount;
};

class CCombatSequenceBase { public:
    bool Running;
    long NoAvailableActions;
    long CurrentActionIndex;
    CActionVector CombatActions;
    CThingCreatureBase* Creature;
    CThingCreatureBase* Target;
    bool CreateCalled;
    CCombatSequenceBase(CThingCreatureBase& creature, CThingCreatureBase& target)
        : Running(false), NoAvailableActions(0), CurrentActionIndex(0), Creature(&creature), Target(&target), CreateCalled(false) { }
    virtual ~CCombatSequenceBase() { ++g_deletedSequences; }
    virtual void OnCreate() { }
    void CloneActions(const CCombatSequenceBase& other)
    {
        CombatActions = other.CombatActions;
        NoAvailableActions = other.NoAvailableActions;
    }
    void PushAction(CCombatAction& action)
    {
        CombatActions.push_back(action);
        ++NoAvailableActions;
    }
};

typedef CCombatSequenceBase* (*TSequenceCreator)(CThingCreatureBase&, CThingCreatureBase&);

struct SSpecialEntry { const char* Name; TSequenceCreator Creator; };
struct CSpecialMap {                                       // stand-in for std::map<CCharString, TSequenceCreator>
    const SSpecialEntry* Entries;
    int Count;
    const SSpecialEntry* find(const char* name) const
    {
        for (int i = 0; i < Count; ++i)
            if (std::strcmp(Entries[i].Name, name) == 0)
                return &Entries[i];
        return end();
    }
    const SSpecialEntry* end() const { return Entries + Count; }
};

struct CCombatSequenceGeneric : CCombatSequenceBase {
    const CCombatSequenceDef* Def;
    CCombatSequenceGeneric(CThingCreatureBase& creature, CThingCreatureBase& target, const CCombatSequenceDef* def)
        : CCombatSequenceBase(creature, target), Def(def) { }
    virtual void OnCreateSpecial(const CSpecialMap& specials)
    {
        if (Def->ActionCount == 0) {
            const char* name = Def->InstantiationName;
            const SSpecialEntry* found = specials.find(name);
            if (found != specials.end()) {
                TSequenceCreator creator = found->Creator;
                CCountedPointer<CCombatSequenceBase> sequence(creator(*Creature, *Target));
                sequence->OnCreate();
                CloneActions(*sequence);
            }
        }
        else {
            for (const EActionRegisteredType* it = Def->ActionList; it != Def->ActionList + Def->ActionCount; ++it) {
                EActionRegisteredType type = *it;
                CCombatAction action;
                action.CreatureAction = CCombatActionBase::GetPCombatActionFromEnum(type, *Creature, *Target);
                PushAction(action);
            }
        }
        CreateCalled = true;
    }
};

struct CSpecialSequence : CCombatSequenceBase {            // a registered special: two actions made in OnCreate
    CSpecialSequence(CThingCreatureBase& creature, CThingCreatureBase& target) : CCombatSequenceBase(creature, target) { }
    virtual void OnCreate()
    {
        CCombatAction first, second;
        first.CreatureAction = CCombatActionBase::GetPCombatActionFromEnum(ACTION_COMBAT_IDLE, *Creature, *Target);
        second.CreatureAction = CCombatActionBase::GetPCombatActionFromEnum(ACTION_COMBAT_ATTACK_MAIN, *Creature, *Target);
        PushAction(first);
        PushAction(second);
    }
};
static int g_created = 0;
static CCombatSequenceBase* CreateSpecial(CThingCreatureBase& creature, CThingCreatureBase& target)
{
    ++g_created;
    return new CSpecialSequence(creature, target);
}

static int Fail(int line) { std::printf("008b0b60_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(x) do { if (!(x)) return Fail(__LINE__); } while (0)

int main()
{
    CThingCreatureBase creature = { 1 }, target = { 2 };
    static const SSpecialEntry entries[] = { { "OTHER", 0 }, { "SPECIAL", CreateSpecial } };
    CSpecialMap specials = { entries, 2 };

    {   // special found: created, OnCreate run, actions and count copied, the special released (deleted) afterwards
        CCombatSequenceDef def = { "SPECIAL", { ACTION_NULL }, 0 };
        CCombatSequenceGeneric sequence(creature, target, &def);
        sequence.OnCreateSpecial(specials);
        CHECK(g_created == 1 && g_deletedSequences == 1);
        CHECK(sequence.CreateCalled && sequence.NoAvailableActions == 2 && sequence.CombatActions.Count == 2);
        CHECK(sequence.CombatActions.Items[0].CreatureAction.Data->Type == ACTION_COMBAT_IDLE);
        CHECK(sequence.CombatActions.Items[1].CreatureAction.Data->Type == ACTION_COMBAT_ATTACK_MAIN);
        CHECK(sequence.CombatActions.Items[1].CreatureAction.Data->Creature == &creature);
        CHECK(sequence.CombatActions.Items[1].CreatureAction.Data->Target == &target);
        CHECK(sequence.CombatActions.Items[0].CreatureAction.Info->RefCount == 1);   // only our copy survives the special
        CHECK(g_deletedActions == 0 && g_liveInfos == 2);
    }
    CHECK(g_deletedActions == 2 && g_liveInfos == 0 && g_deletedSequences == 2);

    {   // special not registered: nothing created, still marked created
        CCombatSequenceDef def = { "MISSING", { ACTION_NULL }, 0 };
        CCombatSequenceGeneric sequence(creature, target, &def);
        sequence.OnCreateSpecial(specials);
        CHECK(g_created == 1 && sequence.CreateCalled && sequence.NoAvailableActions == 0 && sequence.CombatActions.Count == 0);
    }

    {   // action list: one action per enum (a null one is still pushed and counted), each held exactly once; the name is not looked up
        CCombatSequenceDef def = { "SPECIAL", { ACTION_COMBAT_ATTACK_MAIN, ACTION_NULL, ACTION_COMBAT_IDLE }, 3 };
        CCombatSequenceGeneric sequence(creature, target, &def);
        sequence.OnCreateSpecial(specials);
        CHECK(g_created == 1);
        CHECK(sequence.CreateCalled && sequence.NoAvailableActions == 3 && sequence.CombatActions.Count == 3);
        CHECK(sequence.CombatActions.Items[0].CreatureAction.Data->Type == ACTION_COMBAT_ATTACK_MAIN);
        CHECK(sequence.CombatActions.Items[0].CreatureAction.Info->RefCount == 1);
        CHECK(sequence.CombatActions.Items[1].CreatureAction.Data == 0 && sequence.CombatActions.Items[1].CreatureAction.Info == 0);
        CHECK(sequence.CombatActions.Items[2].CreatureAction.Data->Type == ACTION_COMBAT_IDLE);
        CHECK(sequence.CombatActions.Items[2].CreatureAction.Info->RefCount == 1);
        CHECK(g_liveInfos == 2 && g_deletedActions == 2);
    }
    CHECK(g_liveInfos == 0 && g_deletedActions == 4);

    std::printf("008b0b60_TEST PASS\n");
    return 0;
}