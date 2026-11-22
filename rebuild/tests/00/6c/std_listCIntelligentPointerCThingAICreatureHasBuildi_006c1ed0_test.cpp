#include <stdio.h>
#include <list>
#include <iterator>
struct CThing {};
struct CThingBuilding : CThing {};
struct CThingAICreature {
    bool dead;
    CThingBuilding* home;
    CThingBuilding* work;
    mutable unsigned homeReads;
    mutable unsigned workReads;
    CThingBuilding* GetHomeBuilding() const { ++homeReads; return home; }
    CThingBuilding* GetWorkBuilding() const { ++workReads; return work; }
};
template<class T> class CIntelligentPointer {
    T* p;
public:
    CIntelligentPointer(T* item=0):p(item) {}
    T* operator->() const { return p; }
    T* Get() const { return p; }
};
template<class T> bool IS_THING_ALIVE(const T& p) { return p.Get() && !p->dead; }
class HasBuildingAsHomeOrWork {
    CThing* Building;
public:
    explicit HasBuildingAsHomeOrWork(CThing* b):Building(b) {}
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const
    {
        if (IS_THING_ALIVE(creature)) {
            if (creature->GetHomeBuilding() == Building || creature->GetWorkBuilding() == Building)
                return true;
        }
        return false;
    }
};
namespace reconstruction {
template<class Input, class Output, class Predicate>
Output copy_if(Input first, Input last, Output result, Predicate pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}
}
int main() {
    typedef std::list<CIntelligentPointer<CThingAICreature> > List;
    CThingBuilding target, other;
    CThingAICreature people[8];
    for (unsigned count=0;count<=9;++count) {
        List original, out;
        unsigned expected[8], used=0;
        if (count) original.push_back(0);
        for (unsigned i=0;i+1<count;++i) {
            people[i].dead=(i&4)!=0;
            people[i].home=(i&2)?&target:&other;
            people[i].work=(i&1)?&target:&other;
            people[i].homeReads = people[i].workReads = 0;
            original.push_back(&people[i]);
            if ((i&4)==0 && (i&3)!=0) expected[used++]=i;
        }
        out.push_back(0);
        const List& input=original;
        std::back_insert_iterator<List> end=reconstruction::copy_if(input.begin(),input.end(),
            std::back_inserter(out),HasBuildingAsHomeOrWork(&target));
        *end=CIntelligentPointer<CThingAICreature>();
        if (out.size()!=used+2 || original.size()!=count) return 1;
        List::const_iterator p=out.begin();
        if (p->Get()!=0) return 2;
        ++p;
        for (unsigned k=0;k<used;++k,++p) if(p->Get()!=&people[expected[k]])return 3;
        if (p->Get() != 0) return 4;
        for (unsigned i = 0; i + 1 < count; ++i) {
            if (people[i].homeReads != ((i & 4) ? 0U : 1U) ||
                people[i].workReads != ((i & 6) ? 0U : 1U)) return 5;
        }
    }
    puts("006c1ed0_TEST PASS");
    return 0;
}