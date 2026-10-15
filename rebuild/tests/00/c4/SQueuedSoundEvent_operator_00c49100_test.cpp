// Behaviour test for SQueuedSoundEvent::operator= @ 0x00C49100.
#include <cstdio>
static int g_ptrAssigns = 0;
struct CIntelligentPointer {
    void* m_pItem;
    virtual ~CIntelligentPointer() { }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { m_pItem = o.m_pItem; ++g_ptrAssigns; return *this; }
};
struct C3DVector { float X, Y, Z; };
struct SFourWords { unsigned int m_words[4]; };
struct SQueuedSoundEvent {
    unsigned int m_id, m_kind; C3DVector m_position; CIntelligentPointer m_creature; unsigned int m_param1C, m_param20;
    SFourWords m_block; unsigned int m_param34, m_param38, m_param3C;
    SQueuedSoundEvent& operator=(const SQueuedSoundEvent& o)
    {
        m_id = o.m_id; m_kind = o.m_kind; m_position = o.m_position; m_creature = o.m_creature; m_param1C = o.m_param1C;
        m_param20 = o.m_param20; m_block = o.m_block; m_param34 = o.m_param34; m_param38 = o.m_param38; m_param3C = o.m_param3C;
        return *this;
    }
};
int main()
{
    SQueuedSoundEvent a, b; int creature = 0;
    a.m_id = 1; a.m_kind = 2; a.m_position.X = 3; a.m_position.Y = 4; a.m_position.Z = 5; a.m_creature.m_pItem = &creature;
    a.m_param1C = 6; a.m_param20 = 7; for (int i = 0; i < 4; ++i) a.m_block.m_words[i] = 10 + i; a.m_param34 = 8; a.m_param38 = 9; a.m_param3C = 11;
    b.m_creature.m_pItem = 0;
    SQueuedSoundEvent& r = (b = a);
    int ok = &r == &b && b.m_id == 1 && b.m_kind == 2 && b.m_position.X == 3 && b.m_position.Z == 5 && b.m_creature.m_pItem == &creature
          && g_ptrAssigns == 1 && b.m_param1C == 6 && b.m_param20 == 7 && b.m_block.m_words[0] == 10 && b.m_block.m_words[3] == 13
          && b.m_param34 == 8 && b.m_param38 == 9 && b.m_param3C == 11;
    if (!ok) { std::printf("00c49100_TEST FAIL\n"); return 1; }
    std::printf("00c49100_TEST PASS\n");
    return 0;
}