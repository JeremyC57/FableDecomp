// Behaviour test for CTCInventoryStats::InitialiseMannequin @ 0x005CD767: no mannequin def -> null; otherwise create, copy present components, animate
#include <cstdio>
#include <cstring>
struct Log { int created; int copies; int added; int anim; int edit; };
static void Initialise(long mannequinDef, unsigned presentMask, int mannequinEditable, Log* log, int* mannequin)
{
    std::memset(log, 0, sizeof(*log));
    if (mannequinDef == 0) { *mannequin = 0; return; }
    *mannequin = 1;
    log->created = 1;
    log->copies = 1;                                  /* the +0x64 component */
    if (presentMask & 1) log->copies++;               /* 0x5E */
    if (presentMask & 2) log->copies++;               /* 0x78 */
    if (presentMask & 4) log->copies++;               /* 0x03 */
    log->copies++;                                    /* 0x5C, unconditional */
    log->added = 1;
    log->anim = 6;
    if (mannequinEditable) log->edit = 1;
}
int main()
{
    int ok = 1;
    Log log;
    int m = 5;
    Initialise(0, 7, 1, &log, &m);
    if (m != 0 || log.created) ok = 0;
    Initialise(12, 7, 1, &log, &m);
    if (m != 1 || log.copies != 5 || !log.added || log.anim != 6 || !log.edit) ok = 0;
    Initialise(12, 2, 0, &log, &m);
    if (log.copies != 3 || log.edit) ok = 0;
    if (!ok) { std::printf("005cd767_TEST FAIL\n"); return 1; }
    std::printf("005cd767_TEST PASS\n");
    return 0;
}