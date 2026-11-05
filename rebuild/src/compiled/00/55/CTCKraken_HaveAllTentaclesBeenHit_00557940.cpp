// Leaf: ?CTCKraken_HaveAllTentaclesBeenHit@@YIDPAVCTCKraken@@@Z

class CTCKraken { public:
    char pad[0x1a8];
    int  m_phase;      // +0x1a8
    int  m_remaining;  // +0x1ac
};

char __fastcall CTCKraken_HaveAllTentaclesBeenHit(CTCKraken *kraken)
{
    if (kraken->m_phase != 2)
        return 0;
    return kraken->m_remaining == 0;
}