// Behaviour test for CTCForcePushed::OnRemove (0x00798540): links the reconstructed object; the creature call is a stub.
#include <cstdio>
static int g_calls = 0; static bool g_suspend = true; static void* g_suspender = (void*)1;
struct CThing {
    unsigned char _pad_0x00[0x10];
    unsigned char m_type;
    unsigned char _pad_0x11[0x80];
    unsigned char Dead : 1;
    unsigned char _flags_0x91 : 7;
    void SetSuspendAnimation(bool suspend, void* suspender);
};
void CThing::SetSuspendAnimation(bool suspend, void* suspender) { ++g_calls; g_suspend = suspend; g_suspender = suspender; }
class CTCForcePushed {
public:
    CTCForcePushed(CThing* t) : ParentThing(t) {}
    virtual ~CTCForcePushed();
    virtual void OnRemove();
    virtual void OnKill();
    void SetParent(CThing* t) { ParentThing = t; }
private:
    CThing* ParentThing;
};
CTCForcePushed::~CTCForcePushed() {}
void CTCForcePushed::OnKill() {}
static int fail(int n) { std::printf("00798540_TEST FAIL %d\n", n); return 1; }
int main()
{
    CThing t; t.m_type = 1; t.Dead = 0; t._flags_0x91 = 0;
    CTCForcePushed c(0);
    c.OnRemove();
    if (g_calls != 0) return fail(1);
    c.SetParent(&t); t.Dead = 1; c.OnRemove();
    if (g_calls != 0) return fail(2);
    t.Dead = 0; t.m_type = 2; c.OnRemove();
    if (g_calls != 0) return fail(3);
    t.m_type = 1;
    CTCForcePushed* p = &c; p->OnRemove();              // through the vtable
    if (g_calls != 1 || g_suspend || g_suspender != 0) return fail(4);
    std::printf("00798540_TEST PASS\n");
    return 0;
}
