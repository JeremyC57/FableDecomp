// Behaviour test for CSoundSystem_00c131c0::PlaySoundList (0x00C131C0): standalone copy of the model; lock, list, start and group helpers faked.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
static int g_locks = 0, g_unlocks = 0;
class PKCCriticalSection { public: int dummy; };
static PKCCriticalSection g_lock_0143bb14;
class PKCProtectionInstance { public:
    PKCCriticalSection* m_pTarget; long m_state;
    PKCProtectionInstance(PKCCriticalSection* t) : m_pTarget(t), m_state(1) { g_locks++; }
    ~PKCProtectionInstance() { g_unlocks++; }
};
struct CSoundRequest_00c131c0 { int tag; };
class CSoundSystem_00c131c0;
class CSoundGroup_00c29ec0;
struct CSoundInstance_00c131c0 { long m_id; CSoundGroup_00c29ec0* m_group; CSoundInstance_00c131c0* m_nextInGroup; };
class CSoundGroup_00c29ec0 { public:
    long m_id; CSoundInstance_00c131c0* m_first;
    void AddMember(CSoundInstance_00c131c0* s) { s->m_group = this; s->m_nextInGroup = m_first; m_first = s; }
    void Init(CSoundSystem_00c131c0*, long id, CSoundInstance_00c131c0* first) { m_id = id; m_first = 0; AddMember(first); }
};
static CSoundGroup_00c29ec0 g_groups[8]; static int g_nGroups = 0;
class CSoundGroupPool_00c21f20 { public: CSoundGroup_00c29ec0* Allocate() { return &g_groups[g_nGroups++]; } };
static long g_valid[8]; static void* g_items[8]; static unsigned long g_count = 0;
class ISoundList_00c131c0 { public:
    long IsValid(unsigned long i) { return g_valid[i]; }
    unsigned long GetCount() { return g_count; }
    void* GetItem(unsigned long i) { return g_items[i]; }
};
static ISoundList_00c131c0 g_list;
class ISoundListSource_00c131c0 { public: ISoundList_00c131c0* GetList() { return &g_list; } };
static CSoundInstance_00c131c0 g_sounds[8];
static const CSoundRequest_00c131c0* g_lastRequest = 0; static int g_starts = 0;
class CSoundSystem_00c131c0 { public:
    CSoundGroupPool_00c21f20 m_groupPool;
    long m_nextGroupId;
    CSoundInstance_00c131c0* StartSound(void* item, void* parent, CSoundRequest_00c131c0* request, long flags)
    {
        g_starts++; g_lastRequest = request; if (parent || flags) g_fail = 1;
        if (!item) return 0;
        CSoundInstance_00c131c0* s = (CSoundInstance_00c131c0*)item; s->m_group = 0; s->m_nextInGroup = 0; return s;
    }
    long PlaySoundList(ISoundListSource_00c131c0* source, CSoundRequest_00c131c0* request)
    {
        PKCProtectionInstance lock(&g_lock_0143bb14);
        CSoundInstance_00c131c0* previous = 0;
        for (unsigned long i = 0; i < source->GetList()->GetCount(); i++)
        {
            if (!source->GetList()->IsValid(i))
                continue;
            CSoundInstance_00c131c0* sound = StartSound(source->GetList()->GetItem(i), 0, request, 0);
            if (!sound)
                continue;
            if (previous)
            {
                if (!previous->m_group)
                {
                    CSoundGroup_00c29ec0* group = m_groupPool.Allocate();
                    group->Init(this, m_nextGroupId++, previous);
                }
                previous->m_group->AddMember(sound);
            }
            previous = sound;
        }
        if (!previous)
            return -1;
        if (previous->m_group == 0)
            return previous->m_id;
        return previous->m_group->m_id;
    }
};
int main()
{
    for (int i = 0; i < 8; i++) { g_sounds[i].m_id = 100 + i; g_items[i] = &g_sounds[i]; g_valid[i] = 1; }
    CSoundSystem_00c131c0 sys; sys.m_nextGroupId = 7;
    ISoundListSource_00c131c0 src; CSoundRequest_00c131c0 req = { 1 };
    // empty list: -1, lock taken and released
    g_count = 0; CHECK(sys.PlaySoundList(&src, &req) == -1); CHECK(g_locks == 1 && g_unlocks == 1 && g_starts == 0);
    // one sound: its own id, no group
    g_count = 1; CHECK(sys.PlaySoundList(&src, &req) == 100); CHECK(g_nGroups == 0 && g_lastRequest == &req);
    // three sounds, the middle one invalid: one group numbered 7, both sounds in it
    g_count = 3; g_valid[1] = 0;
    CHECK(sys.PlaySoundList(&src, &req) == 7); CHECK(g_nGroups == 1 && sys.m_nextGroupId == 8);
    CHECK(g_sounds[0].m_group == &g_groups[0] && g_sounds[2].m_group == &g_groups[0] && g_groups[0].m_first == &g_sounds[2]);
    CHECK(g_sounds[2].m_nextInGroup == &g_sounds[0]);
    // failed starts are skipped; all failed -> -1
    g_valid[1] = 1; g_items[0] = 0; g_items[1] = 0; g_count = 2; CHECK(sys.PlaySoundList(&src, &req) == -1);
    // four started sounds share one group (only one allocation)
    for (int i = 0; i < 8; i++) g_items[i] = &g_sounds[i];
    g_count = 4; CHECK(sys.PlaySoundList(&src, &req) == 8); CHECK(g_nGroups == 2 && sys.m_nextGroupId == 9);
    CHECK(g_groups[1].m_first == &g_sounds[3] && g_sounds[3].m_nextInGroup == &g_sounds[2] && g_sounds[1].m_nextInGroup == &g_sounds[0]);
    CHECK(g_locks == 5 && g_unlocks == 5);
    if (g_fail) { std::printf("00c131c0_TEST FAIL\n"); return 1; }
    std::printf("00c131c0_TEST PASS\n");
    return 0;
}