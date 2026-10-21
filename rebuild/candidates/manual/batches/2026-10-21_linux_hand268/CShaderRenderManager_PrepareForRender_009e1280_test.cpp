#include <cstdio>
// Behaviour model of CShaderRenderManager::PrepareForRender (0x009E1280): records which updaters run for a given
// dirty-flag word and render-flag word.
struct Mgr {
    unsigned int DirtyFlags;
    bool WorldTransformIsIdentity;
    unsigned int constants[4];
    unsigned int constantCount;
    char log[32]; int n; unsigned int lastCount; const unsigned int* lastData; unsigned int lightingFlags;
    void Hit(char c) { log[n++] = c; log[n] = 0; }
    unsigned int* data() { return constantCount > 0 ? constants : 0; }
    void SetVSConstants(const unsigned int* d, unsigned int count) { lastData = d; lastCount = count; Hit('V'); }
    void PrepareForRender(unsigned int renderFlags)
    {
        if (DirtyFlags & 4) SetVSConstants(data(), constantCount);
        if (DirtyFlags & 1) Hit('P');
        if (DirtyFlags & 2) Hit('p');
        if ((renderFlags & 0x400) && (DirtyFlags & 0x20)) Hit('a');
        if (renderFlags & 0x40) { if (DirtyFlags & 0x1000) Hit('T'); }
        else if ((renderFlags & 0x80) && (DirtyFlags & 0x800)) Hit('t');
        if (renderFlags & 0x200) { if (DirtyFlags & 0x2000) Hit('F'); }
        else if ((renderFlags & 0x100) && (DirtyFlags & 0x4000)) Hit('f');
        if (DirtyFlags & 4) Hit('A');
        if (DirtyFlags & 0x20000) Hit('C');
        if (DirtyFlags & 0x400) Hit('G');
        if (renderFlags & 8) { if (DirtyFlags & 0x100) { lightingFlags = renderFlags; Hit('L'); } }
        else if ((renderFlags & 0x10) && (DirtyFlags & 0x200)) Hit('l');
        if ((renderFlags & 0x20) && (DirtyFlags & 0x8000)) Hit('S');
        if (!(renderFlags & 1) && !WorldTransformIsIdentity) Hit('W');
        if (DirtyFlags & 0x80) Hit('X');
        if ((renderFlags & 4) && (DirtyFlags & 8)) Hit('8');
    }
};
static bool Same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
int main()
{
    Mgr m; m.n = 0; m.log[0] = 0; m.constantCount = 3; m.WorldTransformIsIdentity = false; m.lastCount = 99; m.lastData = 0;
    m.DirtyFlags = 0xffffffffu;
    m.PrepareForRender(0x40 | 0x200 | 8 | 0x20 | 4 | 0x400);
    bool ok = Same(m.log, "VPpaTFACGLSWX8") && m.lastCount == 3 && m.lastData == m.constants && m.lightingFlags == (0x40u | 0x200 | 8 | 0x20 | 4 | 0x400);
    m.n = 0; m.log[0] = 0;
    m.PrepareForRender(0x80 | 0x100 | 0x10 | 1);
    ok = ok && Same(m.log, "VPptfACGlX");
    m.n = 0; m.log[0] = 0; m.DirtyFlags = 0; m.WorldTransformIsIdentity = true;
    m.PrepareForRender(0xffff);
    ok = ok && m.n == 0;
    m.DirtyFlags = 4; m.constantCount = 0;
    m.PrepareForRender(1);
    ok = ok && Same(m.log, "VA") && m.lastCount == 0 && m.lastData == 0;
    if (!ok) { std::printf("009e1280_TEST FAIL\n"); return 1; }
    std::printf("009e1280_TEST PASS\n");
    return 0;
}
