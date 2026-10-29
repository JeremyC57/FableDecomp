struct MidObj {
    char pad0[0xc];
    char flagC;            // +0xc
    unsigned char flagD;   // +0xd
    char pad_e[0xbf];
    char flagCD;           // +0xcd  (signed)
};
class CUserProfileManager : public MidObj { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ

extern void __fastcall PlayMovie(void* target, int id,
    int a3, int a4, int a5, int a6, int a7, unsigned char b,
    float f, int a10, int a11, int a12);

void __fastcall CGameScriptInterface_PlayAVIMovie(void* self, void* edx_, void* target)
{
    (void)self; (void)edx_;
    MidObj* m = &CUserProfileManager::Get();
    PlayMovie(target, 0x1b, 0, 0, 0, 0, 0,
              m->flagC ? (unsigned char)m->flagCD : m->flagD, -1.0f, 0, 0, 0);
}