#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <utility>

class CCharString {
public:
    const char* text;
    explicit CCharString(const char* value = "") : text(value) {}
    bool IsEmpty() const { return !*text; }
    bool operator==(const CCharString& other) const { return strcmp(text, other.text) == 0; }
};
struct CDefString {
    CCharString name;
    explicit CDefString(const char* text = "") : name(text) {}
    CCharString GetString() const { return name; }
    operator CCharString() const { return GetString(); }
};
struct CBrainBehaviour {
    CDefString Name;
    int Priority;
    int ScriptAIPriority;
    CBrainBehaviour(const char* text, int p, int s) : Name(text), Priority(p), ScriptAIPriority(s) {}
};
struct CBrainDef {
    mutable int refs;
    std::vector<CBrainBehaviour> Behaviours;
    std::vector<CDefString> DisabledBehaviours;
    CBrainDef() : refs(0) {}
};
template<class T> class CDefPointer {
    T* data;
public:
    explicit CDefPointer(T* p) : data(p) { if(data) ++data->refs; }
    CDefPointer(const CDefPointer& other) : data(other.data) { if(data) ++data->refs; }
    ~CDefPointer() { if(data) --data->refs; }
    T* Get() const { return data; }
    T* operator->() const { return data; }
};
enum EScriptAIPriority { FIRST_PRIORITY, SECOND_PRIORITY, THIRD_PRIORITY };
class CAIBrain;
struct CAIStateGroupBase {
    int priority, script, creator;
    CAIBrain* brain;
    int* creature;
    CCharString name;
    virtual void Initialise(CAIBrain& b, int* c, int p, EScriptAIPriority s, const CCharString& n) {
        brain=&b; creature=c; priority=p; script=s; name=n;
    }
    int GetPriority() const { return priority; }
};
struct StateGroupEntry {
    CCharString StateGroupName;
    CAIStateGroupBase* (*AllocFunc)();
};
struct StateGroupListFixture {
    typedef std::pair<int,CAIStateGroupBase*> Value;
    std::vector<Value> data;
    bool dirty;
    int updates;
    StateGroupListFixture() : dirty(false), updates(0) {}
    void Insert(int priority, CAIStateGroupBase* group) { data.push_back(Value(priority,group)); dirty=true; }
    static bool greaterPriority(const Value& a,const Value& b) { return a.first>b.first; }
    void Update() { ++updates; if(dirty) std::sort(data.begin(),data.end(),greaterPriority); dirty=false; }
};
class CAIBrain {
public:
    int* Creature;
    static StateGroupEntry StateGroupInfo[];
    StateGroupListFixture StateGroupList;
    void InitStateGroups(CDefPointer<const CBrainDef> pbrain_def);
};
static CAIStateGroupBase pool[8];
static int allocations;
static CAIStateGroupBase* make(int kind) { pool[allocations].creator=kind;return &pool[allocations++]; }
static CAIStateGroupBase* firstFactory(){return make(1);}
static CAIStateGroupBase* secondFactory(){return make(2);}
StateGroupEntry CAIBrain::StateGroupInfo[]={
    {CCharString("Idle"),firstFactory}, {CCharString("Trade"),secondFactory}, {CCharString(""),0}
};
void CAIBrain::InitStateGroups(CDefPointer<const CBrainDef> pbrain_def)
{
    CAIStateGroupBase* state_group;
    if (pbrain_def.Get()) {
        long max_groups = pbrain_def->Behaviours.size();
        for (long c1 = 0; c1 < max_groups; ++c1) {
            CCharString behaviour_name = pbrain_def->Behaviours[c1].Name.GetString();
            if (!behaviour_name.IsEmpty()) {
                bool disabled = false;
                for (long c2 = 0; c2 < pbrain_def->DisabledBehaviours.size(); ++c2) {
                    if (behaviour_name == pbrain_def->DisabledBehaviours[c2]) {
                        disabled = true;
                        break;
                    }
                }
                if (!disabled) {
                    for (long c3 = 0; StateGroupInfo[c3].AllocFunc; ++c3) {
                        if (StateGroupInfo[c3].StateGroupName == behaviour_name) {
                            state_group = StateGroupInfo[c3].AllocFunc();
                            state_group->Initialise(*this, Creature, pbrain_def->Behaviours[c1].Priority,
                                (EScriptAIPriority)pbrain_def->Behaviours[c1].ScriptAIPriority, StateGroupInfo[c3].StateGroupName);
                            StateGroupList.Insert(state_group->GetPriority(), state_group);
                            break;
                        }
                    }
                }
            }
        }
    }
    StateGroupList.Update();
}

int main() {
    const char* names[]={"", "Idle", "Trade", "Other", "IdLe"};
    for(int valid=0;valid!=2;++valid)
    for(int count=0;count!=4;++count)
    for(int first=0;first!=5;++first)
    for(int disabled=0;disabled!=4;++disabled)
    for(int seed=0;seed!=2;++seed) {
        CBrainDef def;
        if(disabled&1) def.DisabledBehaviours.push_back(CDefString("Idle"));
        if(disabled&2) def.DisabledBehaviours.push_back(CDefString("Trade"));
        for(int k=0;k<count;++k) def.Behaviours.push_back(CBrainBehaviour(names[(first+k)%5],7+k*3,k));
        int creature=13; CAIBrain brain; brain.Creature=&creature;allocations=0;
        CAIStateGroupBase existing;
        if(seed) brain.StateGroupList.Insert(2,&existing);
        {
            CDefPointer<const CBrainDef> handle(valid?&def:0);
            brain.InitStateGroups(handle);
            if(def.refs!=valid) return 1;
        }
        if(def.refs!=0 || brain.StateGroupList.updates!=1 || brain.StateGroupList.dirty) return 2;
        int expected=0;
        if(valid) for(int i=0;i<count;++i) {
            int kind=(first+i)%5;
            if((kind!=1 && kind!=2) || (disabled&(1<<(kind-1)))) continue;
            CAIStateGroupBase& group=pool[expected++];
            if(group.creator!=kind || group.priority!=7+i*3 || group.script!=i) return 3;
            if(group.brain!=&brain || group.creature!=&creature || strcmp(group.name.text,names[kind])) return 4;
        }
        if(allocations!=expected || (int)brain.StateGroupList.data.size()!=expected+seed) return 5;
        for(int j=0;j<expected;++j)
            if(brain.StateGroupList.data[j].second!=&pool[expected-1-j]) return 6;
        if(seed && brain.StateGroupList.data.back().second!=&existing) return 7;
    }
    puts("0088bf30_TEST PASS"); return 0;
}