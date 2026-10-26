#include <cstdio>

struct HomeObj {
    int id;
    int GetHomeId();
};
int HomeObj::GetHomeId() { return id; }

class CThingAICreature { public:
    char pad[0x168];
    HomeObj home;
};

class CThingFilter_HasBuildingAsHome { public:
    int HomeBuilding;
    bool operator()(CThingAICreature* creature);
};

bool CThingFilter_HasBuildingAsHome::operator()(CThingAICreature* creature)
{
    return creature->home.GetHomeId() == this->HomeBuilding;
}

int main()
{
    CThingAICreature c;
    c.home.id = 42;

    CThingFilter_HasBuildingAsHome fMatch;
    fMatch.HomeBuilding = 42;
    CThingFilter_HasBuildingAsHome fMiss;
    fMiss.HomeBuilding = 7;

    bool a = fMatch(&c);   // true
    bool b = fMiss(&c);    // false

    if (a && !b) {
        printf("HASBUILDINGHOME_OK\n");
        return 0;
    }
    printf("HASBUILDINGHOME_FAIL a=%d b=%d\n", (int)a, (int)b);
    return 1;
}