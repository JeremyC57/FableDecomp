// Behaviour test for 0x006F48A0: the defending-combatant info releases its attack pattern, its last attacker, its combat rings, then clears and frees the attackers map.
#include <cstdio>
#include <cstdlib>
static int g_log[32]; static int g_n = 0;
class CDefPointeeBase { public: virtual ~CDefPointeeBase() {} virtual void DeleteThis() { g_log[g_n++] = 50; } unsigned long RefCount; };
template<class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    ~CDefPointer() { CDefPointeeBase* object = Object; if (object) { if (--object->RefCount == 0) object->DeleteThis(); Object = 0; } }
};
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() { g_log[g_n++] = 40; } void* PItem; };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };
class C3DVector { public: float X, Y, Z; };
struct SNonscalarPtrIteratorTag { };
struct CCombatRing { long Ring; };
static void DestroyRange(CCombatRing*, CCombatRing*, const SNonscalarPtrIteratorTag&) { g_log[g_n++] = 60; }
struct CCombatRingVector {
    CCombatRing* _M_start; CCombatRing* _M_finish; CCombatRing* _M_end_of_storage;
    ~CCombatRingVector() { DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag()); if (_M_start) { g_log[g_n++] = 61; std::free(_M_start); } }
};
struct CCombatWheel { CCombatRingVector CombatRings; };
static bool g_headReset = false;
struct CAttackerTree {
    struct node { unsigned char colour; node* parent; node* left; node* right; };
    node* head; unsigned long count; long compare;
    void Erase(node* root) { g_log[g_n++] = root ? 70 : 71; if (root) std::free(root); }
    ~CAttackerTree()
    {
        if (count) { Erase(head->parent); head->left = head; head->parent = 0; head->right = head; count = 0; }
        if (head) { g_headReset = head->left == head && head->parent == 0 && head->right == head; g_log[g_n++] = 72; std::free(head); }
    }
};
class CThing; class CAttackPatternDef;
class CDefendingCombatantInfo { public:
    CAttackerTree Attackers; CCombatWheel CombatWheel; C3DVector LastVictimWheelUpdatePos; long LastWheelUpdateWorldFrame;
    CIntelligentPointer<CThing> PLastAttacker; CDefPointer<const CAttackPatternDef> PAttackPattern;
    long AttackPatternIndex; long FrameForNextInstruction;
};
static CAttackerTree::node* NewNode() { return (CAttackerTree::node*)std::malloc(sizeof(CAttackerTree::node)); }
int main()
{
    CDefPointeeBase def; def.RefCount = 2;
    {
        CDefendingCombatantInfo info;
        info.Attackers.head = NewNode(); info.Attackers.head->parent = NewNode(); info.Attackers.count = 1;
        info.Attackers.head->left = info.Attackers.head->right = info.Attackers.head->parent;
        info.CombatWheel.CombatRings._M_start = (CCombatRing*)std::malloc(sizeof(CCombatRing));
        info.CombatWheel.CombatRings._M_finish = info.CombatWheel.CombatRings._M_end_of_storage = info.CombatWheel.CombatRings._M_start + 1;
        info.PLastAttacker.PItem = 0;
        info.PAttackPattern.Object = &def;
    }
    // def survives (RefCount 1), last attacker (40), rings (60, 61), root erased (70), head reset and freed (72)
    bool ok = g_n == 5 && g_log[0] == 40 && g_log[1] == 60 && g_log[2] == 61 && g_log[3] == 70 && g_log[4] == 72
           && g_headReset && def.RefCount == 1;
    if (!ok) { std::printf("006f48a0_TEST FAIL\n"); return 1; }
    std::printf("006f48a0_TEST PASS\n");
    return 0;
}