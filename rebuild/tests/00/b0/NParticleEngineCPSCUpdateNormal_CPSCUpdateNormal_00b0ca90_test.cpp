// Behaviour test for 0x00B0CA90 ~CPSCUpdateNormal: every decal emitter is killed first, then the decal-emitter list (each
// intelligent pointer destroyed, nodes freed) and DecalEmitterName are destroyed, then the base chain.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
static int g_destroyed = 0;
struct Node { Node* next; Node* prev; };
class CEmitterList {
public:
    CEmitterList() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void push() { Node* n = (Node*)std::malloc(sizeof(Node)); n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    ~CEmitterList()
    {
        Node* cur = head->next;
        while (cur != head) { Node* tmp = cur; cur = cur->next; ++g_destroyed; std::free(tmp); }
        head->next = head; head->prev = head;
        std::free(head);
        Log("L");
    }
    Node* head;
};
class CCharString { public: ~CCharString() { Log("N"); } };
class CBaseClassNonCopyable { public: virtual ~CBaseClassNonCopyable() { Log("B"); } };
class CPSCBase : public CBaseClassNonCopyable { public: void* ParticleSystem; };
class CPSCUpdateNormal : public CPSCBase {
public:
    CPSCUpdateNormal() : Alive(2) { }
    virtual ~CPSCUpdateNormal() { KillAllDecalEmitters(); }
    void KillAllDecalEmitters() { Alive = 0; Log("K"); }
    int          Alive;
    CCharString  DecalEmitterName;
    CEmitterList DecalEmitterList;
};
int main()
{
    g_log[0] = 0;
    CPSCUpdateNormal* component = new CPSCUpdateNormal;
    component->DecalEmitterList.push();
    component->DecalEmitterList.push();
    CBaseClassNonCopyable* base = component;
    delete base;
    if (std::strcmp(g_log, "KLNB") != 0 || g_destroyed != 2) { std::printf("00b0ca90_TEST FAIL\n"); return 1; }
    std::printf("00b0ca90_TEST PASS\n");
    return 0;
}