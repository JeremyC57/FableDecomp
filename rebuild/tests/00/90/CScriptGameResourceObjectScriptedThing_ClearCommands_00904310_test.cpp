// Behaviour test for 0x00904310 CScriptGameResourceObjectScriptedThing::ClearCommands: a host model of the
// thing (interface bitset + sorted component map), the intelligent pointer and the scripted-control component.
#include <cstdio>
struct Control { int cleanups; Control() : cleanups(0) {} void CleanupAllActions() { ++cleanups; } };
struct Entry { long type; Control* component; };
struct Thing {
    unsigned int bits[9]; Entry entries[4]; int count; bool dead;
    Thing() : count(0), dead(false) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long t, Control* c) { entries[count].type = t; entries[count].component = c; ++count; entries[count].type = 0; entries[count].component = 0; bits[t >> 5] |= 1u << (t & 31); }
    Entry* LowerBound(const long* t) { int i = 0; while (i < count && entries[i].type < *t) ++i; return &entries[i]; }
    template<class T> bool GetInterface(T** out, long type)
    {
        if (bits[type >> 5] & (1u << (type & 31))) {
            long key = type; Entry* it = LowerBound(&key); Entry* last = &entries[count];
            if (it == last || type < it->type) it = last;
            *out = it->component; return true;
        }
        return false;
    }
};
struct IP { Thing* item; Thing* GetPItem() { return item; } };
struct ScriptedThing {
    IP PThing;
    void ClearCommands()
    {
        if (PThing.GetPItem()) {
            if (!PThing.GetPItem()->dead) {
                Control* control = 0;
                PThing.GetPItem()->GetInterface(&control, 0x1f);
                if (control) control->CleanupAllActions();   // retail calls through the unset local on a miss
            }
        }
    }
};
int main()
{
    Thing t; Control c; t.Add(0x1f, &c);
    ScriptedThing s; s.PThing.item = &t;
    s.ClearCommands();
    if (c.cleanups != 1) { std::printf("00904310_TEST FAIL\n"); return 1; }
    t.dead = true; s.ClearCommands();
    if (c.cleanups != 1) { std::printf("00904310_TEST FAIL\n"); return 1; }
    s.PThing.item = 0; s.ClearCommands();
    if (c.cleanups != 1) { std::printf("00904310_TEST FAIL\n"); return 1; }
    std::printf("00904310_TEST PASS\n");
    return 0;
}