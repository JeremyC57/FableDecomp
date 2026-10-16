// Behaviour test for CTCCreatureGroup row 00813100 (model of the group member map).
#include <cstdio>
struct Thing { unsigned long long UniqueID; bool dead; bool killed; bool IsDead() const { return dead; } };
struct Data { bool IsActive; unsigned int DeactivatedFrame; };
struct Node { unsigned long long key; Data second; bool used; };
static unsigned int g_frame = 77;
struct Map {
    Node nodes[4]; Node header; unsigned int count;
    Node* find(unsigned long long k) { for (int i = 0; i < 4; ++i) if (nodes[i].used && nodes[i].key == k) return &nodes[i]; return &header; }
    unsigned int erase(unsigned long long k) { Node* n = find(k); if (n == &header) return 0; n->used = false; --count; return 1; }
};
struct Group {
    unsigned int MemberCount, ActiveMemberCount; bool ShutdownOnEmpty; Map Members; Thing* parent;
    bool RemoveMember(Thing* m)
    {
        if (!m) return false;
        unsigned long long uid = m->UniqueID;
        if (Members.find(uid) != &Members.header) {
            Members.erase(uid); --MemberCount;
            if (ShutdownOnEmpty && Members.count == 0) parent->killed = true;
        }
        return true;
    }
    bool DeactivateMember(Thing* m)
    {
        if (!m) return false;
        if (m->IsDead()) { RemoveMember(m); return true; }
        Node* it = Members.find(m->UniqueID);
        if (it != &Members.header) {
            if (it->second.IsActive) { it->second.DeactivatedFrame = g_frame; if (it->second.IsActive) --ActiveMemberCount; }
            it->second.IsActive = false;
            return true;
        }
        return false;
    }
    bool ActivateMember(Thing* m)
    {
        if (m && !m->IsDead()) {
            Node* it = Members.find(m->UniqueID);
            if (it != &Members.header) {
                it->second.DeactivatedFrame = 0;
                if (!it->second.IsActive) ++ActiveMemberCount;
                it->second.IsActive = true;
                return true;
            }
        }
        return false;
    }
};
static void Setup(Group& g, Thing& owner, Thing& a, Thing& b)
{
    owner.killed = false; a.UniqueID = 5; a.dead = false; b.UniqueID = 9; b.dead = false;
    g.parent = &owner; g.ShutdownOnEmpty = true; g.MemberCount = 1; g.ActiveMemberCount = 1;
    for (int i = 0; i < 4; ++i) g.Members.nodes[i].used = false;
    g.Members.nodes[0].used = true; g.Members.nodes[0].key = 5; g.Members.nodes[0].second.IsActive = true;
    g.Members.nodes[0].second.DeactivatedFrame = 0; g.Members.count = 1;
}
#define FAIL { std::printf("00813100_TEST FAIL\n"); return 1; }
int main()
{
    Group g; Thing owner, a, b; Setup(g, owner, a, b);
    if (g.RemoveMember(0)) FAIL; if (!g.RemoveMember(&b) || g.MemberCount != 1 || owner.killed) FAIL; if (!g.RemoveMember(&a) || g.MemberCount != 0 || !owner.killed) FAIL;
    std::printf("00813100_TEST PASS\n");
    return 0;
}
