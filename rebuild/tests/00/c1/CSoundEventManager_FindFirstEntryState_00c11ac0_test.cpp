// Behaviour test for 0x00C11AC0 CSoundEventManager::FindFirstEntryState (libpk, descriptive): resolve the key, walk the
// object's entry table group by group and return the first non-negative state of (tag, entry), else -1.
#include <cstdio>

static int g_locks = 0;
struct Lock { Lock() { ++g_locks; } ~Lock() { --g_locks; } };

struct Entry { int state; };
struct EntryTable {
    int groups; int counts[3]; Entry* entries[3][3];
    virtual unsigned int GetEntryCount(unsigned int g) { return counts[g]; }
    virtual unsigned int GetGroupCount() { return groups; }
    virtual Entry* GetEntry(unsigned int g, unsigned int i) { return entries[g][i]; }
    virtual ~EntryTable() { }
};
struct Object {
    EntryTable* table;
    virtual EntryTable* GetEntryTable() { return table; }
    virtual ~Object() { }
};
struct Manager {
    Object* objects[2];
    int queries;
    virtual long GetEntryState(void* tag, Entry* e) { ++queries; return tag == (void*)1 ? e->state : -5; }
    virtual ~Manager() { }
    Object* Find(int key) { return key >= 0 && key < 2 ? objects[key] : 0; }
    long FindFirstEntryState(void* tag, int key)
    {
        Lock lock;
        Object* object = Find(key);
        if (object == 0)
            return -1;
        for (unsigned int group = 0; group < object->GetEntryTable()->GetGroupCount(); ++group)
            for (unsigned int index = 0; index < object->GetEntryTable()->GetEntryCount(group); ++index) {
                Entry* entry = object->GetEntryTable()->GetEntry(group, index);
                long state = GetEntryState(tag, entry);
                if (state >= 0)
                    return state;
            }
        return -1;
    }
};

int main()
{
    int fail = 0;
    Entry e[4] = { { -1 }, { -2 }, { 7 }, { 9 } };
    EntryTable t; t.groups = 2; t.counts[0] = 2; t.counts[1] = 2;
    t.entries[0][0] = &e[0]; t.entries[0][1] = &e[1]; t.entries[1][0] = &e[2]; t.entries[1][1] = &e[3];
    EntryTable empty; empty.groups = 0;
    Object o; o.table = &t;
    Object o2; o2.table = &empty;
    Manager m; m.objects[0] = &o; m.objects[1] = &o2; m.queries = 0;
    if (m.FindFirstEntryState((void*)1, 0) != 7 || m.queries != 3) fail = 1;   // first non-negative is group 1, entry 0
    if (m.FindFirstEntryState((void*)2, 0) != -1) fail = 1;                     // no state for this tag
    if (m.FindFirstEntryState((void*)1, 1) != -1) fail = 1;                     // empty table
    if (m.FindFirstEntryState((void*)1, 5) != -1) fail = 1;                     // unknown key
    if (g_locks != 0) fail = 1;
    if (fail) { std::printf("00c11ac0_TEST FAIL\n"); return 1; }
    std::printf("00c11ac0_TEST PASS\n");
    return 0;
}