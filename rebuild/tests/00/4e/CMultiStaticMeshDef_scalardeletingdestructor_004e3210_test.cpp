#include <cstdio>

struct CMember { };
static int g_vtbl = 0;
static int member_calls = 0;
static int base_calls = 0;
static int delete_calls = 0;

void __fastcall CMember_dtor_004dfa66(CMember* def) { (void)def; member_calls++; }
void __fastcall Base_dtor(void* def) { (void)def; base_calls++; }
void __cdecl op_delete(void* p) { (void)p; delete_calls++; }

struct CMultiStaticMeshDef {
    int f0;
    int Parent;
    void* f8;
    char pad[0x28 - 0xc];
    CMember m28;
};

void* __fastcall CMultiStaticMeshDef_scalar_deleting_destructor(CMultiStaticMeshDef* def, int edx, unsigned char flags)
{
    CMember_dtor_004dfa66(&def->m28);
    *(const int**)def = &g_vtbl;
    Base_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main()
{
    CMultiStaticMeshDef obj;
    obj.f0 = 123;

    // flags bit0 clear: no delete
    member_calls = base_calls = delete_calls = 0;
    void* r = CMultiStaticMeshDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (member_calls != 1) { std::printf("FAIL member\n"); return 1; }
    if (base_calls != 1) { std::printf("FAIL base\n"); return 1; }
    if (delete_calls != 0) { std::printf("FAIL delete-noflag\n"); return 1; }
    if (*(int*)&obj != (int)(long)&g_vtbl) { std::printf("FAIL vtbl\n"); return 1; }

    // flags bit0 set: delete
    member_calls = base_calls = delete_calls = 0;
    r = CMultiStaticMeshDef_scalar_deleting_destructor(&obj, 0, 1);
    if (delete_calls != 1) { std::printf("FAIL delete-flag\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00454647_TEST PASS\n");
    return 0;
}