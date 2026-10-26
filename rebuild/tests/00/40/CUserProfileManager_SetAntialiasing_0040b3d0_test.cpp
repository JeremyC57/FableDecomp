#include <cstdio>
#if !defined(_MSC_VER)
#  define __fastcall
#  define __stdcall
#endif

class CUserProfileManager { public:
    char pad0[0xc];
    char flag;
    char pad1[0x2c - 0xd];
    float m2c;              // +0x2c DialogueVolume (PDB)
    char pad2[0xec - 0x30];
    float mec;              // +0xec
};

static int g_h1 = 0;
static int g_h2 = 0;

void __fastcall CUPM_Helper1(CUserProfileManager* self) { (void)self; ++g_h1; }
void __fastcall CUPM_Helper2(CUserProfileManager* self) { (void)self; ++g_h2; }

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, void* edx, float value, char apply)
{
    (void)edx;
    if (self->flag)
        self->mec = value;
    self->m2c = value;
    CUPM_Helper1(self);
    if (apply)
        CUPM_Helper2(self);
}

int main() {
    CUserProfileManager o;
    o.flag = 1; o.m2c = 0; o.mec = 0;
    CUserProfileManager_SetAntialiasing(&o, 0, 0.5f, 1);
    if (o.mec != 0.5f) { std::printf("FAIL mec\n"); return 1; }
    if (o.m2c != 0.5f) { std::printf("FAIL m2c\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1\n"); return 1; }
    if (g_h2 != 1) { std::printf("FAIL h2\n"); return 1; }

    g_h1 = g_h2 = 0;
    o.flag = 0; o.m2c = 0; o.mec = 7;
    CUserProfileManager_SetAntialiasing(&o, 0, 0.75f, 0);
    if (o.mec != 7) { std::printf("FAIL mec2\n"); return 1; }
    if (o.m2c != 0.75f) { std::printf("FAIL m2c2\n"); return 1; }
    if (g_h1 != 1) { std::printf("FAIL h1b\n"); return 1; }
    if (g_h2 != 0) { std::printf("FAIL h2b\n"); return 1; }

    std::printf("CUserProfileManager_0040b3d0_TEST PASS\n");
    return 0;
}