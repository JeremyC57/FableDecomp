// Behaviour test for 0x01224CE0: the dynamic initializer of NScript::CQ_SunnyvaleMasterData::RelationSuffixes[3].
// The initializer default-constructs the three strings in ascending order before main and registers one exit function that destroys
// them in reverse order; the test checks construction order / emptiness and the reverse destruction at exit.
#include <stdio.h>
#include <stdlib.h>
static int order[8], n, mainDone;
class CCharString {
public:
    CCharString() : Data(0) { id = n + 1; order[n++] = id; }
    ~CCharString() { if (!mainDone) abort(); order[n++] = -id; }
    const char* Data; int id;
};
static void auditExit()
{
    const int want[6] = { 1, 2, 3, -3, -2, -1 };
    for (int i = 0; i < 6; ++i) if (order[i] != want[i] || n != 6) { printf("01224ce0_TEST FAIL\n"); exit(1); }
    printf("01224ce0_TEST PASS\n");
}
class TestAudit { public: TestAudit() { atexit(auditExit); } };
static TestAudit audit;
namespace NScript { class CQ_SunnyvaleMasterData { public: static CCharString RelationSuffixes[3]; }; }
CCharString NScript::CQ_SunnyvaleMasterData::RelationSuffixes[3];
int main()
{
    if (n != 3) { printf("01224ce0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (NScript::CQ_SunnyvaleMasterData::RelationSuffixes[i].Data != 0) { printf("01224ce0_TEST FAIL\n"); return 1; }
    mainDone = 1;
    return 0;
}
