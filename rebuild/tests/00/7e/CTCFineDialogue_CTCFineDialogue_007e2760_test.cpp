#include <cstdio>
// CTCFineDialogue::~CTCFineDialogue(): stores its vtable, leaves the global TC list, then runs ~CTCBase

static int g_member = 0;
static int g_base = 0;
static int g_order_ok = 0;
static void* g_vt_at_member = 0;

class CTCBase {
public:
    virtual ~CTCBase();
    void RemoveFromGlobalTCs();
};

class CTCFineDialogue : public CTCBase {
public:
    virtual ~CTCFineDialogue();
};

void CTCBase::RemoveFromGlobalTCs()
{
    g_member++;
    g_vt_at_member = *(void**)this;
}

CTCBase::~CTCBase()
{
    g_base++;
    if (g_member == 1) g_order_ok = 1;
}

int main(){
    void* buf[4] = {0, 0, 0, 0};
    CTCFineDialogue* o = (CTCFineDialogue*)buf;
    o->CTCFineDialogue::~CTCFineDialogue();
    if (g_vt_at_member == 0) { printf("BAD vtbl\n"); return 1; }
    if (g_member != 1) { printf("BAD member\n"); return 1; }
    if (g_base != 1 || !g_order_ok) { printf("BAD base\n"); return 1; }
    printf("OK_007e2760\n");
    return 0;
}
