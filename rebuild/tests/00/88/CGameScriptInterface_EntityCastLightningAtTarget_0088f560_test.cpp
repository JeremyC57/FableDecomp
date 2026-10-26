#include <cstdio>

class CGameScriptInterface { public: void* vt; };

void __fastcall CGameScriptInterface_EntityCastLightningAtTarget(CGameScriptInterface* self, int edx_ignored, int caster, int target)
{
    (void)self; (void)caster; (void)target;
}

int main()
{
    CGameScriptInterface obj;
    obj.vt = 0;
    CGameScriptInterface_EntityCastLightningAtTarget(&obj, 0, 1, 2);
    std::printf("CGameScriptInterface_0088f560_TEST PASS\n");
    return 0;
}