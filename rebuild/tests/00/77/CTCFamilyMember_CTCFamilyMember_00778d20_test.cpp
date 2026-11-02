// test: CTCFamilyMember::~CTCFamilyMember -- implicit dtor: destroy the FamilyMembers vector, then the CTCBase base
#include <stdio.h>
#include <vector>
static int baseDtors = 0, liveVectors = 0;
class CThing { public: int id; };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing& t) : ParentThing(&t) { } virtual ~CTCBase() { ++baseDtors; } };
struct TrackedVector : std::vector<const CThing*> { TrackedVector() { ++liveVectors; } ~TrackedVector() { --liveVectors; } };
class CTCFamilyMember : public CTCBase { public:
    TrackedVector FamilyMembers;
    CTCFamilyMember(CThing& t) : CTCBase(t) { }
};
int main() {
    CThing a = { 1 }, b = { 2 }, owner = { 3 };
    CTCBase* p = new CTCFamilyMember(owner);
    ((CTCFamilyMember*)p)->FamilyMembers.push_back(&a);
    ((CTCFamilyMember*)p)->FamilyMembers.push_back(&b);
    if (liveVectors != 1) { printf("00778d20_TEST FAIL\n"); return 1; }
    delete p;
    if (liveVectors != 0 || baseDtors != 1) { printf("00778d20_TEST FAIL\n"); return 1; }
    printf("00778d20_TEST PASS\n");
    return 0;
}