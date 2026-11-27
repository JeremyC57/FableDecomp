#include <list>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdio>
struct CCharString {
    const char* value;
    CCharString(const char* text = "") : value(text) {}
    bool StartsWith(const CCharString& prefix) const { return std::strncmp(value, prefix.value, std::strlen(prefix.value)) == 0; }
    bool operator<(const CCharString& rhs) const { return std::strcmp(value, rhs.value) < 0; }
};
namespace { CCharString QUEST_CARD_POS_MARKER_NAME_PREFIX("QuestCard"); }
struct C3DVector { int number; C3DVector(int n = 0) : number(n) {} };
struct CTCThingOwner;
struct CThing {
    CCharString name;
    C3DVector pos;
    bool alive;
    CTCThingOwner* owner;
    CThing(const char* n = "", int i = 0, bool a = true) : name(n), pos(i), alive(a), owner(0) {}
    bool PeekTC(const CTCThingOwner** dest) const { *dest = owner; return owner != 0; }
    const CCharString& GetScriptName() const { return name; }
    const C3DVector& GetPos() const { return pos; }
};
bool IS_THING_ALIVE(const CThing* thing) { return thing && thing->alive; }
template<class T> struct CIntelligentPointer {
    T* ptr;
    CIntelligentPointer(T* p) : ptr(p) {}
    T* Get() const { return ptr; }
};
struct CTCThingOwner {
    std::list<CIntelligentPointer<CThing> > objects;
    const std::list<CIntelligentPointer<CThing> >& GetObjectList() const { return objects; }
};
template<class K, class V> struct CVectorMap : std::vector<std::pair<K,V> > {
    typedef std::pair<K,V> Pair;
    struct Less { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };
    void Insert(const K& k, const V& v) { this->push_back(Pair(k,v)); }
    void Update() { std::sort(this->begin(), this->end(), Less()); }
};
class CTCHeroCentre {
public:
    CThing parent;
    std::vector<C3DVector> QuestCardPositions;
    CThing& GetParentThing() { return parent; }
    virtual void OnInitialActivate();
};
void CTCHeroCentre::OnInitialActivate()
{
    const CTCThingOwner* ptc_thing_owner;
    if (GetParentThing().PeekTC(&ptc_thing_owner)) {
        std::list<CIntelligentPointer<CThing> > owned_objects(ptc_thing_owner->GetObjectList());
        CVectorMap<CCharString, C3DVector> quest_card_positions;
        for (std::list<CIntelligentPointer<CThing> >::const_iterator it = owned_objects.begin(); it != owned_objects.end(); ++it) {
            const CThing* powned_thing = it->Get();
            if (IS_THING_ALIVE(powned_thing) && powned_thing->GetScriptName().StartsWith(QUEST_CARD_POS_MARKER_NAME_PREFIX))
                quest_card_positions.Insert(powned_thing->GetScriptName(), powned_thing->GetPos());
        }
        quest_card_positions.Update();
        for (long c1 = 0; c1 < quest_card_positions.size(); ++c1)
            QuestCardPositions.push_back(quest_card_positions[c1].second);
    }
}

int main() {
    CTCHeroCentre hero;
    CTCThingOwner owner;
    hero.QuestCardPositions.push_back(C3DVector(99));
    hero.OnInitialActivate();
    if (hero.QuestCardPositions.size() != 1) return 1;
    hero.parent.owner = &owner;
    hero.OnInitialActivate();
    if (hero.QuestCardPositions.size() != 1) return 2;
    CThing things[] = { CThing("QuestCardZ",3), CThing("QuestCardA",2), CThing("questCardB",4), CThing("QuestCard",1), CThing("QuestCardB",5,false) };
    for (int i = 0; i < 5; ++i) owner.objects.push_back(&things[i]);
    owner.objects.push_back(static_cast<CThing*>(0));
    hero.OnInitialActivate();
    hero.OnInitialActivate();
    const int expected[] = {99,1,2,3,1,2,3};
    if (hero.QuestCardPositions.size() != 7 || owner.objects.size() != 6) return 3;
    for (int j = 0; j < 7; ++j) if (hero.QuestCardPositions[j].number != expected[j]) return 4;
    std::puts("0077dc40_TEST PASS");
    return 0;
}