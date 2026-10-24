// Behaviour test for 0x004C60F0 CBulletTimeManager::CBulletTimeManager(world, playerManager): an empty map (header node red,
// no root, leftmost/rightmost = header, count 0), Override off, UseDefaultMultiplier on, tick 0, both references kept.
#include <cstdio>
#include <cstdlib>

struct Node { bool color; Node* parent; Node* left; Node* right; void* key; float m, p; };
struct CWorld { int x; }; struct CPlayerManager { int y; };
struct Manager {
    Node* header; unsigned count; unsigned cmp;
    bool Override; bool UseDefault; long Tick; CWorld* World; CPlayerManager* PM;
    Manager(CWorld& w, CPlayerManager& pm) : header(0)
    {
        header = static_cast<Node*>(std::malloc(sizeof(Node))); count = 0;
        header->color = false; header->parent = 0; header->left = header; header->right = header;
        Override = false; UseDefault = true; Tick = 0; World = &w; PM = &pm;
    }
};

int main()
{
    CWorld w; CPlayerManager pm;
    Manager m(w, pm);
    if (m.header == 0 || m.count != 0 || m.header->color || m.header->parent || m.header->left != m.header ||
        m.header->right != m.header || m.Override || !m.UseDefault || m.Tick != 0 || m.World != &w || m.PM != &pm) {
        std::printf("004c60f0_TEST FAIL\n"); return 1;
    }
    std::free(m.header);
    std::printf("004c60f0_TEST PASS\n");
    return 0;
}