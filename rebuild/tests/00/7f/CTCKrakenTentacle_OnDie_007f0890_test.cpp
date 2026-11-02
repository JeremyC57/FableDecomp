#include <stdio.h>
// Behaviour test for 0x007F0890 CTCKrakenTentacle::OnDie: the three callees in order on the same object.
static int g_calls = 0;
static int g_order[3];
static void* g_last[3];

class CTCBase { public:
    virtual void OnDie();
};
class CTCKrakenTentacle : public CTCBase {
public:
    virtual void OnDie();
    void RemoveObstruction();
private:
    void KillRippleFX();
};
void CTCBase::OnDie() { g_order[g_calls] = 3; g_last[g_calls++] = this; }
void CTCKrakenTentacle::KillRippleFX() { g_order[g_calls] = 1; g_last[g_calls++] = this; }
void CTCKrakenTentacle::RemoveObstruction() { g_order[g_calls] = 2; g_last[g_calls++] = this; }

int main(){
    CTCKrakenTentacle t;
    t.CTCKrakenTentacle::OnDie();
    if (g_calls == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3
        && g_last[0] == (void*)&t && g_last[1] == (void*)&t && g_last[2] == (void*)&t)
        printf("OK_007f0890\n");
    else
        printf("FAIL calls=%d\n", g_calls);
    return 0;
}
