#include <cstdio>

// Model of CCombatActionBase::GetTotalTimeSeconds @ 0x00856DD0
struct CAnimation { float t; float GetTotalTimeSeconds() const { return t; } };
static long g_indexSeen = -1;
struct CAnimationBank { CAnimation anims[4]; CAnimation* GetPAnimation(long i) { g_indexSeen = i; return &anims[i]; } };
struct CAnimationOwner { unsigned char _pad[0x3c]; CAnimationBank* m_pBank; };
class CAnimationEntry { public: long AnimationIndex; };
static CAnimationOwner g_owner;
class CCombatActionBase { public:
    virtual void v00() {} unsigned char _pad04[0x74 - 4]; CAnimationEntry* PAnimEntry; unsigned char _pad78[0x114 - 0x78];
    CAnimationOwner* GetPCreatureAnimationOwner() const { return &g_owner; }
    float GetTotalTimeSeconds() const { long index = PAnimEntry->AnimationIndex; CAnimationBank* bank = GetPCreatureAnimationOwner()->m_pBank; float t = bank->GetPAnimation(index)->GetTotalTimeSeconds(); return t; }
};
int main()
{
    CAnimationBank bank; bank.anims[0].t = 1.0f; bank.anims[1].t = 2.0f; bank.anims[2].t = 2.5f; bank.anims[3].t = 4.0f; g_owner.m_pBank = &bank;
    CAnimationEntry e; e.AnimationIndex = 2; CCombatActionBase a; a.PAnimEntry = &e;
    if (a.GetTotalTimeSeconds() != 2.5f || g_indexSeen != 2 || (sizeof(void*) == 4 && (char*)&a.PAnimEntry - (char*)&a != 0x74)) { std::printf("00856dd0_TEST FAIL\n"); return 1; }
    std::printf("00856dd0_TEST PASS\n"); return 0;
}