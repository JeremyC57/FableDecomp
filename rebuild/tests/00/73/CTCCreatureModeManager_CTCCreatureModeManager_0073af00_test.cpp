// Behaviour test for 0x0073AF00 CTCCreatureModeManager::CTCCreatureModeManager(CThing*)
#include <cstdio>
#include <cstdlib>

struct CThing { int id; };
long g_framesPerSecond = 0;

template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0)
    {
        node* sentinel = (node*)std::malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

template <class T> struct CRetailVector {
    T* m_start;
    T* m_finish;
    T* m_endOfStorage;
    CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};

struct CModeBase;
struct CModePointer { CModeBase* m_pObject; void* m_pCount; };

static int g_baseCalls = 0;
static long g_addModeArg = -1;
static int g_addModeCalls = 0;
static long g_timeAtAddMode = -1;

struct CTCBase_Calls {
    CThing* ParentThing;
    CTCBase_Calls(CThing* parent) : ParentThing(parent) { ++g_baseCalls; }
};

struct CTCCreatureModeManager : CTCBase_Calls {
    CRetailList<CModePointer> PModes;
    long TimeLeftForCurrentMode;
    long TimeForNextIdleMode;
    CRetailVector<long> AvailableIdleModes;
    CTCCreatureModeManager(CThing* parent);
    bool AddMode(long modeType)
    {
        ++g_addModeCalls;
        g_addModeArg = modeType;
        g_timeAtAddMode = TimeForNextIdleMode;
        return true;
    }
};

CTCCreatureModeManager::CTCCreatureModeManager(CThing* parent)
    : CTCBase_Calls(parent)
{
    TimeForNextIdleMode = (long)(g_framesPerSecond * 30.0f);
    AddMode(0);
}

int main()
{
    CThing thing;
    thing.id = 7;
    g_framesPerSecond = 25;
    CTCCreatureModeManager m(&thing);
    int ok = 1;
    if (g_baseCalls != 1 || m.ParentThing != &thing) ok = 0;
    if (!m.PModes.head || m.PModes.head->next != m.PModes.head || m.PModes.head->prev != m.PModes.head) ok = 0;
    if (m.AvailableIdleModes.m_start || m.AvailableIdleModes.m_finish || m.AvailableIdleModes.m_endOfStorage) ok = 0;
    if (m.TimeForNextIdleMode != 750) ok = 0;
    if (g_addModeCalls != 1 || g_addModeArg != 0 || g_timeAtAddMode != 750) ok = 0;
    g_framesPerSecond = 30;
    CTCCreatureModeManager m2(&thing);
    if (m2.TimeForNextIdleMode != 900 || g_addModeCalls != 2) ok = 0;
    std::free(m.PModes.head);
    std::free(m2.PModes.head);
    if (!ok) { std::printf("0073af00_TEST FAIL\n"); return 1; }
    std::printf("0073af00_TEST PASS\n");
    return 0;
}