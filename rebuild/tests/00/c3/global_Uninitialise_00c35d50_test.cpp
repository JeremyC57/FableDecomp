#include <cstdio>

struct CTarget { int id; };
struct CMarkedBuffer { void* m_pBlock; void* m_pCursor; void* m_pBuffer; };

struct COwner {
    CMarkedBuffer m_buffer; CTarget* m_pTarget; int shutdowns; int sets; CTarget* lastSet;
    COwner() : m_pTarget(0), shutdowns(0), sets(0), lastSet((CTarget*)1) { m_buffer.m_pBlock = m_buffer.m_pCursor = m_buffer.m_pBuffer = 0; }
    void Shutdown() { ++shutdowns; }
    bool SetTarget(CTarget* t) { ++sets; lastSet = t; m_pTarget = t; return true; }
};

void Uninitialise(COwner* owner)
{
    if (owner->m_buffer.m_pBuffer)
        owner->Shutdown();
    else if (owner->m_pTarget)
        owner->SetTarget(0);
}

int main()
{
    COwner o; CTarget t; int buf = 0;
    Uninitialise(&o);                                         // nothing live: no call
    if (o.shutdowns || o.sets) { std::printf("00c35d50_TEST FAIL\n"); return 1; }
    o.m_pTarget = &t; Uninitialise(&o);                       // target only: detached
    if (o.shutdowns || o.sets != 1 || o.lastSet != 0 || o.m_pTarget) { std::printf("00c35d50_TEST FAIL\n"); return 1; }
    o.m_pTarget = &t; o.m_buffer.m_pBuffer = &buf; Uninitialise(&o);   // buffer live: shutdown wins
    if (o.shutdowns != 1 || o.sets != 1 || o.m_pTarget != &t) { std::printf("00c35d50_TEST FAIL\n"); return 1; }
    std::printf("00c35d50_TEST PASS\n");
    return 0;
}