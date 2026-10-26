#include <cstdio>
#if !defined(_MSC_VER)
#  define __fastcall
#  define __stdcall
#endif

class CUserProfileManager { public:
    char pad0[0x0c];
    char UseNewProfileValues;
    char pad1[0x13];
    float val20;            // +0x20 HUDAlpha (PDB)
    char pad2[0xbc];
    float vale0;            // +0xe0
};

int g_apply = 0;
int g_refresh = 0;

void __fastcall CUserProfileManager_Apply(CUserProfileManager* self) { (void)self; g_apply++; }
void __fastcall CUserProfileManager_Refresh(CUserProfileManager* self) { (void)self; g_refresh++; }

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, int edx, float val, char flag)
{
    if (self->UseNewProfileValues)
        self->vale0 = val;
    self->val20 = val;
    CUserProfileManager_Apply(self);
    if (flag)
        CUserProfileManager_Refresh(self);
}

int main()
{
    CUserProfileManager m;
    for (int i = 0; i < (int)sizeof(m); ++i) ((char*)&m)[i] = 0;

    // Case 1: flag0c set, flag arg set
    m.UseNewProfileValues = 1;
    CUserProfileManager_SetAntialiasing(&m, 0, 0.75f, 1);
    if (m.vale0 != 0.75f) { std::printf("FAIL vale0\n"); return 1; }
    if (m.val20 != 0.75f) { std::printf("FAIL val20\n"); return 1; }
    if (g_apply != 1) { std::printf("FAIL apply\n"); return 1; }
    if (g_refresh != 1) { std::printf("FAIL refresh\n"); return 1; }

    // Case 2: flag0c clear, flag arg clear
    m.UseNewProfileValues = 0;
    m.vale0 = 9.0f;
    g_apply = 0; g_refresh = 0;
    CUserProfileManager_SetAntialiasing(&m, 0, 0.25f, 0);
    if (m.vale0 != 9.0f) { std::printf("FAIL vale0 2\n"); return 1; }
    if (m.val20 != 0.25f) { std::printf("FAIL val20 2\n"); return 1; }
    if (g_apply != 1) { std::printf("FAIL apply 2\n"); return 1; }
    if (g_refresh != 0) { std::printf("FAIL refresh 2\n"); return 1; }

    std::printf("CUserProfileManager_0040b310_TEST PASS\n");
    return 0;
}