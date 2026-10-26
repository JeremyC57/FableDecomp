#include <cstdio>
struct CMember { };
class CTargetingDef { public:
    void* vtbl;
    int Parent;
    void* f8;
    unsigned char pad[0x74 - 0xc];
    CMember member;
    void* CTargetingDef_scalar_deleting_destructor(unsigned char flags);
};

static int g_member_dtor = 0;
static int g_base_dtor = 0;
static int g_deleted = 0;
void* g_CLandscapeBackgroundPatch_vftable;

void __fastcall CMember_dtor_00451940(CMember* def) { (void)def; g_member_dtor++; }
void __fastcall CTargetingDef_base_dtor(CTargetingDef* def) { (void)def; g_base_dtor++; }
void __cdecl operator_delete(void* p) { (void)p; g_deleted++; }

void* CTargetingDef::CTargetingDef_scalar_deleting_destructor(unsigned char flags)
{
    CMember_dtor_00451940(&this->member);
    this->vtbl = &g_CLandscapeBackgroundPatch_vftable;
    CTargetingDef_base_dtor(this);
    if (flags & 1) {
        operator_delete(this);
    }
    return this;
}

int main() {
    CTargetingDef obj;
    obj.vtbl = 0;
    void* r = obj.CTargetingDef_scalar_deleting_destructor(0);
    if (r != &obj || g_member_dtor != 1 || g_base_dtor != 1 || g_deleted != 0 || obj.vtbl != &g_CLandscapeBackgroundPatch_vftable) {
        std::printf("FAIL noflag\n"); return 1;
    }
    r = obj.CTargetingDef_scalar_deleting_destructor(1);
    if (r != &obj || g_member_dtor != 2 || g_base_dtor != 2 || g_deleted != 1) {
        std::printf("FAIL flag\n"); return 1;
    }
    std::printf("CLandscapeBackgroundPatch_0045470e_TEST PASS\n");
    return 0;
}