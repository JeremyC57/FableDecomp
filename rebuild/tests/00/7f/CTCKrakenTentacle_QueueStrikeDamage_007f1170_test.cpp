#include <cstdio>
class CThing { public: unsigned char Dead : 1; };
struct CIntelligentPointer { CThing* m_pItem; int calls; CThing* Get() { ++calls; return m_pItem; } };
class CKrakenTentacleDef { public: float StrikeDelaySeconds; };
static long g_framesPerSecond = 15;
static long g_frame = 1000;
static long GetCurrentFrame() { return g_frame; }
class CTCKrakenTentacle { public:
    const CKrakenTentacleDef* PTentacleDef; CIntelligentPointer PKraken; long QueuedStrikeTick;
    void QueueStrikeDamage()
    {
        if (PKraken.Get() && !PKraken.Get()->Dead) {
            CThing* kraken = PKraken.Get(); (void)kraken;
            long framesPerSecond = g_framesPerSecond;
            const CKrakenTentacleDef* def = PTentacleDef;
            QueuedStrikeTick = GetCurrentFrame() + (long)(framesPerSecond * def->StrikeDelaySeconds);
        }
    }
};
int main()
{
    CKrakenTentacleDef d; d.StrikeDelaySeconds = 2.5f;
    CThing kraken; kraken.Dead = 0;
    CTCKrakenTentacle t; t.PTentacleDef = &d; t.PKraken.m_pItem = 0; t.PKraken.calls = 0; t.QueuedStrikeTick = -1;
    t.QueueStrikeDamage();
    if (t.QueuedStrikeTick != -1 || t.PKraken.calls != 1) { std::printf("007f1170_TEST FAIL\n"); return 1; }
    t.PKraken.m_pItem = &kraken; kraken.Dead = 1; t.QueueStrikeDamage();
    if (t.QueuedStrikeTick != -1) { std::printf("007f1170_TEST FAIL\n"); return 1; }
    kraken.Dead = 0; t.QueueStrikeDamage();
    if (t.QueuedStrikeTick != 1037) { std::printf("007f1170_TEST FAIL\n"); return 1; }
    std::printf("007f1170_TEST PASS\n");
    return 0;
}