// CShaderRenderManager::OnPostViewportChanged @ 0x00c3d460
// Leaf: ?CShaderRenderManager_OnPostViewportChanged@@YIXPAVCShaderRenderManager@@@Z
// or dword ptr [ecx+0xa4], 0x80000000 ; ret

class CShaderRenderManager { public:
    char pad[0xa4];
    unsigned int flags;   // +0xa4
};

void __fastcall CShaderRenderManager_OnPostViewportChanged(CShaderRenderManager* self)
{
    self->flags |= 0x80000000u;
}