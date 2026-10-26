// Behaviour test for the CCreatureAction_DragonBurninateInto deleting destructor (0x00953A40): the
// MovementPoints storage is freed (when allocated) before the action base destructor.
#include <cstdio>
#include <cstdlib>

static int g_events[8];
static int g_count = 0;
static void* g_freed = 0;

static void FreeBlock(void* p) { g_freed = p; g_events[g_count++] = 1; std::free(p); }

struct CDragonActionBase {
    virtual ~CDragonActionBase() { g_events[g_count++] = 2; }
    unsigned char _body[0x110];
};

class C3DVector { public: float X; float Y; float Z; };

struct C3DVectorVector {
    C3DVector* m_first; C3DVector* m_last; C3DVector* m_end;
    C3DVectorVector() : m_first(0), m_last(0), m_end(0) {}
    ~C3DVectorVector() { if (m_first) FreeBlock(m_first); }
};

class CCreatureAction_DragonBurninateInto : public CDragonActionBase { public:
    C3DVector       PreviousPoint;
    C3DVectorVector MovementPoints;
    float           BurnStart;
    float           BurnStop;
    C3DVector       FlameStartPos;
    C3DVector       FlameEndPos;
    CCreatureAction_DragonBurninateInto() : BurnStart(0.0f), BurnStop(0.0f) {}
};

int main()
{
    CCreatureAction_DragonBurninateInto* a = new CCreatureAction_DragonBurninateInto();
    void* block = std::malloc(36);
    a->MovementPoints.m_first = (C3DVector*)block;
    CDragonActionBase* base = a;
    delete base;
    bool ok = g_freed == block && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    delete new CCreatureAction_DragonBurninateInto();
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("00953a40_TEST FAIL\n");
        return 1;
    }
    std::printf("00953a40_TEST PASS\n");
    return 0;
}