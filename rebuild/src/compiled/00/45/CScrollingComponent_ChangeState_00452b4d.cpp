#pragma optimize("gs",on)
// Leaf: ChangeState

struct IVisitor {
    virtual void f00();
    virtual void f01(void* a, void* b);   /* vtbl +0x4 */
    virtual void f02(void* a, void* b);   /* vtbl +0x8 */
};

struct CScrollingComponent {
    char pad[0x18];
    int  state;    /* +0x18 */
    char pad2[0x08];
    void* field24; /* +0x24 */
    void* field28; /* +0x28 */
};

struct ScopeGuard { char buf[0x18]; void init(int flag); };

extern void __stdcall SomeGlobalFunc(const void* arg);

void __fastcall CScrollingComponent_ChangeState(CScrollingComponent* component, void* edx, IVisitor* p)
{
    // Literal (NF414, 2026-11-25): HEAD cast the retail address (const void*)0x0122D70E (no relocation; missed by purity, NF413
    //   typedef_cast_scan); it is now the literal "", retail's pooled empty string at 0x0122D70E (.rdata, a lone NUL).
    SomeGlobalFunc("");
    ScopeGuard g;
    g.init(0);
    int s = component->state;
    switch (s) {
    case 2:
        p->f01(component->field24, p);
        break;
    case 3:
        p->f02(component->field28, p);
        break;
    }
}