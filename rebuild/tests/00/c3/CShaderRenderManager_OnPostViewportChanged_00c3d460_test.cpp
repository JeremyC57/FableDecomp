#include <cstdio>

class CShaderRenderManager { public:
    char pad[0xa4];
    unsigned int flags;   // +0xa4
};

void __fastcall CShaderRenderManager_OnPostViewportChanged(CShaderRenderManager* self)
{
    self->flags |= 0x80000000u;
}

int main()
{
    CShaderRenderManager m;
    m.flags = 0;
    CShaderRenderManager_OnPostViewportChanged(&m);
    bool ok = (m.flags == 0x80000000u);

    m.flags = 0x1;
    CShaderRenderManager_OnPostViewportChanged(&m);
    ok = ok && (m.flags == 0x80000001u);

    // idempotent OR
    CShaderRenderManager_OnPostViewportChanged(&m);
    ok = ok && (m.flags == 0x80000001u);

    if (ok) printf("PARITY_OK\n");
    else printf("PARITY_FAIL\n");
    return 0;
}