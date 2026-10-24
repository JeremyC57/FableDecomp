// Behaviour test for CSoundEmitterLH::CSoundEmitterLH(CGameScriptThing*) @ 0x00AAC530
#include <cstdio>
#include <cstdlib>

struct CGameScriptThing {
    long m_counter;
    long DecrementScriptCounter() { return m_counter++; }
};

struct SNode { SNode* m_pNext; SNode* m_pPrev; long m_a; long m_b; };

struct CList {
    SNode* m_pHead;
    CList()
    {
        m_pHead = 0;
        SNode* node = (SNode*)std::malloc(sizeof(SNode));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
};

struct CSoundEmitterLH {
    CGameScriptThing* m_pThing;
    float m_positionX, m_positionY, m_positionZ;
    CList m_sounds;
    bool m_active, m_paused, m_finished;
    long m_scriptCounter;
    CSoundEmitterLH(CGameScriptThing* thing)
        : m_pThing(thing), m_sounds(), m_active(false), m_paused(false), m_finished(false),
          m_scriptCounter(m_pThing->DecrementScriptCounter())
    {
    }
};

int main()
{
    CGameScriptThing thing;
    thing.m_counter = 41;
    CSoundEmitterLH a(&thing);
    CSoundEmitterLH b(&thing);
    int ok = 1;
    if (a.m_pThing != &thing || a.m_scriptCounter != 41 || b.m_scriptCounter != 42 || thing.m_counter != 43) ok = 0;
    if (a.m_active || a.m_paused || a.m_finished) ok = 0;
    if (a.m_sounds.m_pHead == 0 || a.m_sounds.m_pHead->m_pNext != a.m_sounds.m_pHead
        || a.m_sounds.m_pHead->m_pPrev != a.m_sounds.m_pHead) ok = 0;
    std::free(a.m_sounds.m_pHead);
    std::free(b.m_sounds.m_pHead);
    if (!ok) { std::printf("00aac530_TEST FAIL\n"); return 1; }
    std::printf("00aac530_TEST PASS\n");
    return 0;
}