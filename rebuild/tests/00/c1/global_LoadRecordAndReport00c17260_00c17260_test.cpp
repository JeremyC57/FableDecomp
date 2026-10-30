// Behaviour test for LoadRecordAndReport_00c17260 (0x00C17260): load the record (size 0x2000, with the owner); when the
// owner allows quick assignment and the record's test fails, append the text to the owner's message chunk.
#include <cstdio>
struct Record { bool ok; };
struct Owner { bool quick; int appended; long lastSize; };
static void Load(Record* r, long size, Owner* o, bool ok) { o->lastSize = size; r->ok = ok; }
static void LoadRecordAndReport(Owner* owner, bool loadOk, void* argument)
{
    Record record;
    Load(&record, 0x2000, owner, loadOk);
    (void)argument;
    if (owner->quick) {
        if (!record.ok)
            owner->appended++;
    }
}
int main()
{
    Owner a = { true, 0, 0 }; LoadRecordAndReport(&a, false, 0);
    if (a.appended != 1 || a.lastSize != 0x2000) { std::printf("00c17260_TEST FAIL\n"); return 1; }
    Owner b = { true, 0, 0 }; LoadRecordAndReport(&b, true, 0);
    Owner c = { false, 0, 0 }; LoadRecordAndReport(&c, false, 0);
    if (b.appended != 0 || c.appended != 0) { std::printf("00c17260_TEST FAIL\n"); return 1; }
    std::printf("00c17260_TEST PASS\n"); return 0;
}