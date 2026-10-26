#include <cstdio>

struct Member { int pad[16]; };
class COpinionReactionMaskDef { public:
    char pad[0x3c];
    Member m;
};

static int g_member_dtor = 0;
static int g_base_dtor = 0;
static int g_deleted = 0;

extern "C" void __fastcall Member_dtor(Member* def) { (void)def; g_member_dtor++; }
extern "C" void __fastcall Base_dtor_00430300(COpinionReactionMaskDef* def) { (void)def; g_base_dtor++; }
extern "C" void __cdecl op_delete(void* p) { (void)p; g_deleted++; }

void* __fastcall COpinionReactionMaskDef_scalar_deleting_destructor(COpinionReactionMaskDef* def, int, unsigned int flags)
{
    Member_dtor(&def->m);
    Base_dtor_00430300(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main()
{
    COpinionReactionMaskDef obj;
    void* r = COpinionReactionMaskDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_member_dtor != 1 || g_base_dtor != 1 || g_deleted != 0) {
        std::printf("FAIL no-delete path\n");
        return 1;
    }
    g_member_dtor = g_base_dtor = g_deleted = 0;
    r = COpinionReactionMaskDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_member_dtor != 1 || g_base_dtor != 1 || g_deleted != 1) {
        std::printf("FAIL delete path\n");
        return 1;
    }
    std::printf("CEngineSubPrimitiveText_00456cff_TEST PASS\n");
    return 0;
}