// Behaviour test for CShaderRenderManager::InitialiseLight (0x00989830): a model of the row -- slot `index` gets the light,
// is enabled and flagged for update; slots >= 1 publish their attenuation pair at index-1 of the packed table; constants dirty.
#include <stdio.h>
struct Light { float A0, A1; };
struct Manager { Light Slots[4]; bool Enabled[4], Update[4], Dirty; float Atten[3][2]; };
static void InitialiseLight(Manager& m, unsigned long index, const Light& l)
{
    m.Slots[index] = l; m.Enabled[index] = true; m.Update[index] = true;
    if (index >= 1) { m.Atten[index - 1][0] = l.A0; m.Atten[index - 1][1] = l.A1; }
    m.Dirty = true;
}
int main()
{
    Manager m = {}; Light l = { 0.5f, 2.0f };
    InitialiseLight(m, 0, l); int ok = m.Enabled[0] && m.Update[0] && m.Dirty && m.Atten[0][0] == 0;
    InitialiseLight(m, 2, l); ok = ok && m.Atten[1][0] == 0.5f && m.Atten[1][1] == 2.0f;
    printf(ok ? "00989830_TEST PASS\n" : "00989830_TEST FAIL\n");
    return ok ? 0 : 1;
}
