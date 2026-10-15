// Behaviour test for 0x006F2F60 (see src/006f2f60.cpp).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("006f2f60_TEST FAIL\n"); return 1; } } while (0)
struct CSoundPair { int FirstSound; int LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
// Combat-action base model: records what the derived constructor handed it.
struct CCombatActionBase {
    void* owner; void* thing; int actionCode; CSoundPair sound; char animName[48];
    CCombatActionBase(void* o, int code, const CSoundPair& s) : owner(o), thing(0), actionCode(code), sound(s) { animName[0] = 0; }
    CCombatActionBase(void* o, void* t, int code, const CSoundPair& s) : owner(o), thing(t), actionCode(code), sound(s) { animName[0] = 0; }
    void SetAnimationName(const char* n) { std::strcpy(animName, n); }
};
static CSoundPair GetSoundPair(void* owner, const char* name)
{
    CSoundPair p; p.FirstSound = owner ? 1 : 0; p.LastSound = name ? (int)std::strlen(name) : -1; return p;
}

// ~CombatDialogueManager model: release the talker, release the def (destroy on last), clear the tree and free its header.
static int g_destroyed = 0, g_erased = 0, g_freed = 0;
struct CDef { int refCount; void Destroy() { ++g_destroyed; } };
struct SNode { unsigned char color; SNode* parent; SNode* left; SNode* right; };
struct CMap { SNode* header; unsigned int count;
    void EraseSubtree(SNode* root) { if (root) ++g_erased; }
    void Clear() { if (count != 0) { EraseSubtree(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    ~CMap() { Clear(); if (header) { ++g_freed; delete header; } } };
struct CDefPtr { CDef* p; ~CDefPtr() { CDef* d = p; if (d) { if (--d->refCount == 0) d->Destroy(); p = 0; } } };
struct CManager { CMap map; CDefPtr def; };
int main()
{
    CDef d; d.refCount = 1; SNode root;
    {
        CManager* m = new CManager; m->map.header = new SNode; m->map.header->parent = &root; m->map.count = 1; m->def.p = &d;
        delete m;
    }
    CHECK(g_destroyed == 1 && d.refCount == 0 && g_erased == 1 && g_freed == 1);
    d.refCount = 2;
    { CManager* m = new CManager; m->map.header = 0; m->map.count = 0; m->def.p = &d; delete m; }
    CHECK(g_destroyed == 1 && d.refCount == 1 && g_freed == 1 && g_erased == 1);
    std::printf("006f2f60_TEST PASS\n");
    return 0;
}