// Behaviour test for CScrollingComponent::ChangeState (0x00452B4D): links the row object (declared here, not defined), with the
// row's callees defined under the row's own spellings, and asserts what it does: the scope marker gets retail's text "" (the pooled
// empty string at 0x0122D70E) before the guard is set up, and the state switch calls visitor slot 1 (state 2, field24) or slot 2
// (state 3, field28) and nothing for any other state. The component is the reconstruction's own local layout (state +0x18, field24
// +0x24, field28 +0x28), as in the row.
#include <cstdio>

static int g_flags = 0;
static int g_order = 0;
static int g_slot1 = 0, g_slot2 = 0;
static void* g_seen = 0;
static const void* g_markerArg = (const void*)1;
static int g_markerCalls = 0;

struct IVisitor {
    virtual void f00() {}
    virtual void f01(void* a, void* b) { g_slot1++; g_seen = a; }
    virtual void f02(void* a, void* b) { g_slot2++; g_seen = a; }
};

struct CScrollingComponent {
    char pad[0x18];
    int  state;    /* +0x18 */
    char pad2[0x08];
    void* field24; /* +0x24 */
    void* field28; /* +0x28 */
};

struct ScopeGuard { char buf[0x18]; void init(int flag); };
void ScopeGuard::init(int flag) { g_flags |= 2; if (flag != 0) g_flags |= 8; if (g_order == 1) g_order = 2; }

void __stdcall SomeGlobalFunc(const void* arg) { g_flags |= 1; g_markerArg = arg; ++g_markerCalls; if (g_order == 0) g_order = 1; }

void __fastcall CScrollingComponent_ChangeState(CScrollingComponent* component, void* edx, IVisitor* p);   // linked from the row object

static int markerIsEmptyText()
{
    const char* s = (const char*)g_markerArg;
    return s != 0 && s != (const char*)1 && s[0] == '\0';
}

int main() {
    IVisitor v;
    CScrollingComponent c;
    c.field24 = (void*)0x1111;
    c.field28 = (void*)0x2222;

    c.state = 2;
    CScrollingComponent_ChangeState(&c, 0, &v);
    if (g_slot1 != 1 || g_slot2 != 0 || g_seen != (void*)0x1111) {
        std::printf("FAIL state2\n"); return 1;
    }
    if (g_markerCalls != 1 || !markerIsEmptyText()) { std::printf("FAIL marker text\n"); return 1; }
    if (g_order != 2) { std::printf("FAIL order\n"); return 1; }

    c.state = 3;
    CScrollingComponent_ChangeState(&c, 0, &v);
    if (g_slot2 != 1 || g_seen != (void*)0x2222) {
        std::printf("FAIL state3\n"); return 1;
    }

    c.state = 7;
    CScrollingComponent_ChangeState(&c, 0, &v);
    if (g_slot1 != 1 || g_slot2 != 1) {
        std::printf("FAIL other\n"); return 1;
    }

    if ((g_flags & 11) != 3 || g_markerCalls != 3 || !markerIsEmptyText()) { std::printf("FAIL flags\n"); return 1; }

    std::printf("CScrollingComponent_00452b4d_TEST PASS\n");
    return 0;
}
