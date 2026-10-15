// Behaviour test for CTCSoundPlayer::FrameUpdate (0x0066BBF0): the sound update runs only with bit 26 of the thing word set.
#include <cstdio>

static int g_updates = 0;

struct CThing {
    unsigned long m_tcBits28;
};

struct CTCSoundPlayer {
    CThing* ParentThing;
    void UpdateSounds() { ++g_updates; }
    void FrameUpdate();
};

void CTCSoundPlayer::FrameUpdate()
{
    if (ParentThing->m_tcBits28 & 0x4000000)
        UpdateSounds();
}

int main()
{
    CThing t; t.m_tcBits28 = 0x3FFFFFF;
    CTCSoundPlayer p; p.ParentThing = &t;
    p.FrameUpdate();
    t.m_tcBits28 = 0x4000000;
    p.FrameUpdate();
    if (g_updates != 1) { std::printf("0066bbf0_TEST FAIL\n"); return 1; }
    std::printf("0066bbf0_TEST PASS\n");
    return 0;
}
