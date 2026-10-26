#include <stdio.h>

struct Pair;

static int g_calls = 0;
static void* g_last = 0;

extern "C" void __fastcall Dtor_first(Pair* tentacle){ g_calls++; g_last = (void*)tentacle; }
class CTCKrakenTentacle {
public:
    void RemoveObstruction();
};
void CTCKrakenTentacle::RemoveObstruction()
{
    Pair* tentacle = (Pair*)this; (void)tentacle; g_calls++; g_last = (void*)tentacle;
}
extern "C" void __fastcall Dtor_node(Pair* tentacle){ g_calls++; g_last = (void*)tentacle; }

void __fastcall Dest_val(Pair* tentacle);

int main(){
    int dummy = 0;
    Pair* p = (Pair*)&dummy;
    Dest_val(p);
    if (g_calls == 3 && g_last == (void*)p)
        printf("OK_007f0890\n");
    else
        printf("FAIL calls=%d\n", g_calls);
    return 0;
}