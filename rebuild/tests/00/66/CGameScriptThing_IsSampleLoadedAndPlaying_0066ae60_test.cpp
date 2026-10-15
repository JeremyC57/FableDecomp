// Behaviour test for 0x0066AE60 (CGameScriptThing::IsSampleLoadedAndPlaying): false without a game state, without
// a thing, or with a dead thing; otherwise the thing's answer for the sample id.
#include <cstdio>

void* g_pGameState_013b8394 = 0;

struct CThing_Retail {
    unsigned char m_dead;
    unsigned long m_playing;
    bool IsSampleLoadedAndPlaying(unsigned long sampleID) const { return sampleID == m_playing; }
};
struct CIntelligentPointer_Retail {
    CThing_Retail* m_pItem;
    int m_fetches;
    CThing_Retail* GetPItem() { ++m_fetches; return m_pItem; }
};

struct CGameScriptThing {
    CIntelligentPointer_Retail PThing;
    bool IsSampleLoadedAndPlaying(unsigned long sampleID);
};

bool CGameScriptThing::IsSampleLoadedAndPlaying(unsigned long sampleID)
{
    if (g_pGameState_013b8394) {
        CIntelligentPointer_Retail* thing = &PThing;
        if (thing->GetPItem()) {
            if (!thing->GetPItem()->m_dead)
                return thing->GetPItem()->IsSampleLoadedAndPlaying(sampleID);
        }
    }
    return false;
}

int main()
{
    int state = 1;
    CThing_Retail t;
    t.m_dead = 0; t.m_playing = 7;
    CGameScriptThing s;
    s.PThing.m_pItem = &t; s.PThing.m_fetches = 0;
    if (s.IsSampleLoadedAndPlaying(7)) { std::printf("0066ae60_TEST FAIL\n"); return 1; }          // no game state
    g_pGameState_013b8394 = &state;
    if (!s.IsSampleLoadedAndPlaying(7) || s.PThing.m_fetches != 3) { std::printf("0066ae60_TEST FAIL\n"); return 1; }
    if (s.IsSampleLoadedAndPlaying(8)) { std::printf("0066ae60_TEST FAIL\n"); return 1; }
    t.m_dead = 1;
    if (s.IsSampleLoadedAndPlaying(7)) { std::printf("0066ae60_TEST FAIL\n"); return 1; }
    t.m_dead = 0;
    s.PThing.m_pItem = 0;
    if (s.IsSampleLoadedAndPlaying(7)) { std::printf("0066ae60_TEST FAIL\n"); return 1; }
    std::printf("0066ae60_TEST PASS\n");
    return 0;
}