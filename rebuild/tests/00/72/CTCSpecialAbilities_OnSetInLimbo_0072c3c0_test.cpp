#include <cstdio>
#include <list>
class CThing { public: bool dead, unconscious; };
class CThingGameObject : public CThing {};
class TestFollowerPointer {
public:
    CThingGameObject* value;
    CThingGameObject* GetPItem() const { return value; }
    CThingGameObject& operator*() const { return *value; }
};
static int checked;
static bool IS_THING_ALIVE(const TestFollowerPointer& ref) { return ref.value && !ref.value->dead; }
namespace NCreatureMode { bool IsUnconscious(const CThing& thing) { ++checked; return thing.unconscious; } }
class CTCSpecialAbilities {
public:
    class CUndeadFollower { public: TestFollowerPointer PThing; };
    std::list<CUndeadFollower> UndeadFollowers;
    CThingGameObject* reclaimed[4];
    int count;
    void ReclaimUndeadFollower(CThingGameObject& thing) { if (count<4) reclaimed[count]=&thing; ++count; }
    void OnSetInLimbo(bool);
};
void CTCSpecialAbilities::OnSetInLimbo(bool in_limbo)
{
    if (in_limbo) {
        for (std::list<CUndeadFollower>::iterator it = UndeadFollowers.begin(); it != UndeadFollowers.end(); ++it) {
            if (IS_THING_ALIVE(it->PThing) && !NCreatureMode::IsUnconscious(*it->PThing))
                ReclaimUndeadFollower(*static_cast<CThingGameObject*>(it->PThing.GetPItem()));
        }
    }
}

int main()
{
    for (int population=0; population<=5; ++population) {
        for (int on=0; on<=1; ++on) {
            CTCSpecialAbilities tc;
            CThingGameObject things[5];
            tc.count=0;
            checked=0;
            for (int i=0; i<population; ++i) {
                things[i].dead = i==2;
                things[i].unconscious = i==3;
                CTCSpecialAbilities::CUndeadFollower follower;
                follower.PThing.value = i==0 ? 0 : &things[i];
                tc.UndeadFollowers.push_back(follower);
            }
            tc.OnSetInLimbo(on!=0);
            const int expected=on*((population>=2)+(population==5));
            if (tc.count!=expected || checked!=expected+on*(population>=4)) return 1;
            if (expected && tc.reclaimed[0]!=&things[1]) return 1;
            if (expected==2 && tc.reclaimed[1]!=&things[4]) return 1;
        }
    }
    std::puts("0072c3c0_TEST PASS");
    return 0;
}
