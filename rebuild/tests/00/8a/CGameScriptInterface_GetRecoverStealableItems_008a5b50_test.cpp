// behaviour test for 0x008A5B50 CGameScriptInterface::GetRecoverStealableItems(CScriptThing): false for a script thing that resolves no thing, else the AI scratchpad's RecoverStealableItems flag; the by-value script thing's counted reference is released on return
#include <cstdio>
struct Scratchpad { bool Other; bool RecoverStealableItems; };
struct Thing { unsigned int UsingComponents[9]; Scratchpad* Tc; };
struct Info { int RefCount; };
struct ScriptThing {                                   // a counted handle onto a thing
    Thing* Target; Info* Count;
    ScriptThing(Thing* t, Info* c) : Target(t), Count(c) { if (Count) Count->RefCount++; }
    ScriptThing(const ScriptThing& o) : Target(o.Target), Count(o.Count) { if (Count) Count->RefCount++; }
    ~ScriptThing() { if (Count) Count->RefCount--; }
    Thing* GetThing() const { return Target; }
};
static bool Query(ScriptThing thing)                   // the function under test
{
    if (!thing.GetThing())
        return false;
    Thing* t = thing.GetThing();
    Scratchpad* tc = 0;
    if (t->UsingComponents[0xAA >> 5] & (1u << (0xAA & 31)))
        tc = t->Tc;
    return tc->RecoverStealableItems;
}
int main()
{
    int fail = 0;
    Info info; info.RefCount = 1;
    Scratchpad pad; pad.Other = true; pad.RecoverStealableItems = true;
    Thing t; for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0; t.UsingComponents[5] = 0x400u; t.Tc = &pad;
    ScriptThing none(0, &info);
    if (Query(none)) fail |= 1;
    ScriptThing some(&t, &info);
    if (!Query(some)) fail |= 2;
    pad.RecoverStealableItems = false;
    if (Query(some)) fail |= 4;
    if (info.RefCount != 3) fail |= 8;                 // the two handles alive, the by-value copies released
    if (sizeof(ScriptThing) != (unsigned int)(2 * sizeof(void*))) fail |= 16;
    if (fail) { std::printf("008a5b50_TEST FAIL %d\n", fail); return 1; }
    std::printf("008a5b50_TEST PASS\n");
    return 0;
}