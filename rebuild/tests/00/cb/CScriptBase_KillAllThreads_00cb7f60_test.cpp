// Behaviour test for CScriptBase::KillAllThreads (0x00CB7F60): every spawned thread is terminated with PActiveThread
// pointing at it (restored afterwards) and the thread list is cleared; every active entity script is terminated the same
// way, a script with flag bit 0 whose entity is alive first marks the entity to die on level unload; the map is emptied.
#include <cstdio>
#include <list>
#include <vector>

struct Process { bool terminated; Process* seenActive; Process() : terminated(false), seenActive(0) { } };
struct Entity { bool alive; bool killOnUnload; Entity() : alive(false), killOnUnload(false) { } };
struct EntityScript : Process { unsigned long Flags; Entity* entity; EntityScript() : Flags(0), entity(0) { } };
struct Script {
    std::list<Process*> Threads;
    std::vector<EntityScript*> EntityScripts;
    Process* PActiveThread;
    void TerminateProcess(Process* p) { p->terminated = true; p->seenActive = PActiveThread; }
    void KillAllThreads()
    {
        Process* active = PActiveThread;
        for (std::list<Process*>::iterator it = Threads.begin(); it != Threads.end(); ++it) {
            PActiveThread = *it;
            TerminateProcess(*it);
            PActiveThread = active;
        }
        Threads.clear();
        Process* activeScript = PActiveThread;
        for (std::vector<EntityScript*>::iterator it = EntityScripts.begin(); it != EntityScripts.end(); ++it) {
            EntityScript* s = *it;
            PActiveThread = s;
            if (s->Flags & 1) {
                if (s->entity->alive)
                    s->entity->killOnUnload = true;
            }
            TerminateProcess(s);
            PActiveThread = activeScript;
        }
        EntityScripts.erase(EntityScripts.begin(), EntityScripts.end());
    }
};

int main()
{
    Process main_, t1, t2;
    EntityScript s1, s2, s3;
    Entity e1, e2, e3;
    e1.alive = true; e2.alive = false; e3.alive = true;
    s1.Flags = 1; s1.entity = &e1;
    s2.Flags = 1; s2.entity = &e2;
    s3.Flags = 0; s3.entity = &e3;
    Script sc;
    sc.PActiveThread = &main_;
    sc.Threads.push_back(&t1); sc.Threads.push_back(&t2);
    sc.EntityScripts.push_back(&s1); sc.EntityScripts.push_back(&s2); sc.EntityScripts.push_back(&s3);
    sc.KillAllThreads();
    bool ok = sc.Threads.empty() && sc.EntityScripts.empty() && sc.PActiveThread == &main_;
    ok = ok && t1.terminated && t1.seenActive == &t1 && t2.terminated && t2.seenActive == &t2;
    ok = ok && s1.terminated && s1.seenActive == &s1 && s3.seenActive == &s3;
    ok = ok && e1.killOnUnload && !e2.killOnUnload && !e3.killOnUnload;
    std::printf(ok ? "00cb7f60_TEST PASS\n" : "00cb7f60_TEST FAIL\n");
    return 0;
}