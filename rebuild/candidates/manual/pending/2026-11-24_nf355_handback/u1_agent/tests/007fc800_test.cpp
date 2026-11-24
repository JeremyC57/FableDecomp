#include <stdio.h>

// Behaviour test for 0x007fc800 CTCKraken::EndAttackBeam (linked with the row object): Mode = 3, then KillBeamEffect, then KillTentacles.
class CTCKraken { public:
    char pad[0x64];
    int  Mode;                                  // +0x64
    void EndAttackBeam();
private:
    void KillBeamEffect();
    void KillTentacles();
};

static int g_pre = 0;
static int g_fin = 0;
static int g_order = 0;
static CTCKraken* g_last = 0;

void CTCKraken::KillBeamEffect(){ g_pre++; g_last = this; if (g_fin == 0) g_order = 1; }
void CTCKraken::KillTentacles(){ g_fin++; g_last = this; }

int main(){
    CTCKraken k;
    k.Mode = 0;
    k.EndAttackBeam();
    if (k.Mode == 3 && g_pre == 1 && g_fin == 1 && g_order == 1 && g_last == &k)
        printf("OK_007fc800\n");
    else
        printf("FAIL %d %d %d\n", k.Mode, g_pre, g_fin);
    return 0;
}
