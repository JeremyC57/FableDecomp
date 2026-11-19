// Behaviour test for 0x00834530 (anonymous namespace)::CombatStressTestDebug: the row's defines, includes and body verbatim. Things are
// raw zeroed images at retail offsets (ThingTypeFlags +0x6C, state bits +0x91: IncludeInThingScans 0x04, Activated 0x08, Dead 0x01,
// InLimbo 0x20; AISuspended bit 1 of +0x1C8, Suspenders set +0x1CC); doubles for PeekClassList (the world's thing list), the two
// set<..>::insert_unique (plain BST inserts), GFROR13, IsInMeleeAsAttacker, GetCombatManager and the combat manager's register calls.
// Checks: the creature's AI is suspended (null suspender in its set, flag set); it is deregistered only when in melee as an attacker; the
// AI list holds only scan-visible AI creatures (non-AI types, unscanned, inactive, dead and limbo things skipped, the type mask is the
// full 0x1F); the draws follow GFRandom on the world seed (size/2 draws of range size, duplicates merged) and the creature registers
// against each drawn creature in ascending index order except itself; an empty world draws nothing.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_SUSPENDERS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CCombatManager.h"
#include "engine/GFRandom.h"
#include "stlport/fable_map.h"

namespace {

void __fastcall CombatStressTestDebug(CThingAICreature& ai_creature, const CThingSearchTools& search_tools, unsigned long* pworld_seed)
{
    ai_creature.SuspendAI(true, 0);
    std::vector<CThingAICreature*> ais;
    search_tools.GetAllThings<CThingAICreature>(CThingFilter_None(), ais);
    if (ai_creature.IsInMeleeAsAttacker())
        ai_creature.GetCombatManager().DeregisterCombatant(ai_creature);
    if (!ais.empty()) {
        long rnd_num = GFRandom(ais.size() / 2, pworld_seed);
        std::set<long> random_indices;
        for (long c1 = 0; c1 != rnd_num; ++c1)
            random_indices.insert(GFRandom(ais.size(), pworld_seed));
        for (std::set<long>::iterator it = random_indices.begin(); it != random_indices.end(); ++it) {
            CThingAICreature* ptarget = ais[*it];
            if (ptarget != &ai_creature)
                ai_creature.GetCombatManager().RegisterCombatant(ai_creature, *ptarget, true);
        }
    }
}

}

// Fidelity: the anonymous-namespace function's only caller is WorldUpdate (another row); the pointer keeps this TU's copy emitted.
void (__fastcall* g_pCombatStressTestDebug)(CThingAICreature&, const CThingSearchTools&, unsigned long*) = &CombatStressTestDebug;


// ---- test doubles -----------------------------------------------------------------------------------------------------------------
static std::list<CClass*>* g_world_list = 0;
const std::list<CClass*>& CThingSearchTools::PeekClassList() const { return *g_world_list; }
long FABLE_FASTCALL GFROR13(unsigned long value) { return (long)((value >> 13) | (value << 19)); }

static int g_melee = 0, g_deregs = 0, g_regs = 0;
static CThingCreatureBase* g_dereg_who = 0;
static CThingCreatureBase* g_reg_a[64]; static CThingCreatureBase* g_reg_b[64]; static bool g_reg_flag[64];
static unsigned char g_combat_manager[0x100];
bool CThingCreatureBase::IsInMeleeAsAttacker() const { return g_melee != 0; }
CCombatManager& CThingCreatureBase::GetCombatManager() { return *(CCombatManager*)g_combat_manager; }
void CCombatManager::DeregisterCombatant(CThingCreatureBase& c) { ++g_deregs; g_dereg_who = &c; }
void CCombatManager::RegisterCombatant(CThingCreatureBase& a, CThingCreatureBase& b, bool f)
{ if (g_regs < 64) { g_reg_a[g_regs] = &a; g_reg_b[g_regs] = &b; g_reg_flag[g_regs] = f; } ++g_regs; }

#define FAKE_INSERT_UNIQUE(K) \
template<> std::pair<std::_Rb_tree<K,K,std::_Identity<K>,std::less<K>,std::allocator<K> >::iterator,bool> \
std::_Rb_tree<K,K,std::_Identity<K>,std::less<K>,std::allocator<K> >::insert_unique(const K& v) \
{ \
    std::_Rb_tree_node_base* header = this->_M_header._M_data; \
    std::_Rb_tree_node_base* parent = header; std::_Rb_tree_node_base* x = header->_M_parent; bool left = true; \
    while (x != 0) { \
        K k = static_cast<std::_Rb_tree_node<K>*>(x)->_M_value_field; \
        if (!(v < k) && !(k < v)) return std::pair<iterator,bool>(iterator(static_cast<std::_Rb_tree_node<K>*>(x)), false); \
        parent = x; left = v < k; x = left ? x->_M_left : x->_M_right; \
    } \
    std::_Rb_tree_node<K>* n = this->_M_header.allocate(1); \
    n->_M_value_field = v; n->_M_left = 0; n->_M_right = 0; n->_M_parent = parent; n->_M_color = true; \
    if (parent == header) { header->_M_parent = n; header->_M_left = n; header->_M_right = n; } \
    else if (left) { parent->_M_left = n; if (parent == header->_M_left) header->_M_left = n; } \
    else { parent->_M_right = n; if (parent == header->_M_right) header->_M_right = n; } \
    ++_M_node_count; \
    return std::pair<iterator,bool>(iterator(n), true); \
}
FAKE_INSERT_UNIQUE(long)
typedef void* VoidPtr;
static int g_tree_erase_calls = 0;     // SuspendAI(false, ..) only: never reached from this row
template<> void std::_Rb_tree<VoidPtr,VoidPtr,std::_Identity<VoidPtr>,std::less<VoidPtr>,std::allocator<VoidPtr> >::erase(iterator) { ++g_tree_erase_calls; }
FAKE_INSERT_UNIQUE(VoidPtr)

// ---- harness ------------------------------------------------------------------------------------------------------------------------
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static unsigned char images[16][0x200];
static unsigned char search_image[0x40];
static CThing* Make(int i, unsigned long type_flags, unsigned char state)
{
    memset(images[i], 0, sizeof(images[i]));
    *(unsigned long*)(images[i] + 0x6C) = type_flags;
    images[i][0x91] = state;
    return (CThing*)images[i];
}
static unsigned long Rand(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = (*seed >> 13) | (*seed << 19);
    return range ? *seed % range : 0;
}
static void SetUpCreature(CThingAICreature* c)
{
    // an empty STLport set<void*> at +0x1CC: header node (red, no root, left/right = header), count 0
    std::_Rb_tree_node_base* h = (std::_Rb_tree_node_base*)malloc(0x14);
    h->_M_color = false; h->_M_parent = 0; h->_M_left = h; h->_M_right = h;
    *(void**)((char*)c + 0x1CC) = h; *(unsigned long*)((char*)c + 0x1D0) = 0;
}
static void Run(std::list<CClass*>& world, int nais, CThingAICreature** ais, CThingAICreature* me, unsigned long seed0, int melee, const char* tag)
{
    g_world_list = &world; g_melee = melee; g_deregs = 0; g_regs = 0; g_dereg_who = 0;
    unsigned long seed = seed0;
    CombatStressTestDebug(*me, *(const CThingSearchTools*)search_image, &seed);
    // suspension
    std::_Rb_tree_node_base* h = *(std::_Rb_tree_node_base**)((char*)me + 0x1CC);
    check(*(unsigned long*)((char*)me + 0x1D0) == 1 && h->_M_parent != 0 && static_cast<std::_Rb_tree_node<void*>*>(h->_M_parent)->_M_value_field == 0, tag);
    check((((unsigned char*)me)[0x1C8] & 2) != 0, "AISuspended set");
    check(g_deregs == (melee ? 1 : 0) && (!melee || g_dereg_who == (CThingCreatureBase*)me), "deregister only in melee as attacker");
    // expected draws
    unsigned long eseed = seed0; int expected = 0;
    if (nais) {
        unsigned long n = Rand(nais / 2, &eseed);
        bool picked[64]; memset(picked, 0, sizeof(picked));
        for (unsigned long c = 0; c < n; ++c) picked[Rand(nais, &eseed)] = true;
        for (int i = 0; i < nais; ++i) {
            if (!picked[i] || ais[i] == me) continue;
            check(expected < g_regs && g_reg_a[expected] == (CThingCreatureBase*)me && g_reg_b[expected] == (CThingCreatureBase*)ais[i] && g_reg_flag[expected], "registration order / args");
            ++expected;
        }
    }
    check(g_regs == expected, "registration count");
    check(seed == eseed, "world seed advanced by the draws");
}

int main()
{
    std::list<CClass*> world;
    CThingAICreature* ais[16]; int nais = 0;
    // AI creatures (type flags 0x1F, scanned + activated); interleaved with things that must be skipped
    for (int i = 0; i < 8; ++i) { CThing* t = Make(i, 0x1F, 0x0C); world.push_back((CClass*)t); ais[nais++] = (CThingAICreature*)t; }
    world.push_back((CClass*)Make(8, 0x0F, 0x0C));          // creature, not AI
    world.push_back((CClass*)Make(9, 0x1F, 0x08));          // not included in scans
    world.push_back((CClass*)Make(10, 0x1F, 0x04));         // not activated
    world.push_back((CClass*)Make(11, 0x1F, 0x0D));         // dead
    world.push_back((CClass*)Make(12, 0x1F, 0x2C));         // in limbo
    world.push_back((CClass*)Make(13, 0x3F, 0x0C)); ais[nais++] = (CThingAICreature*)images[13];   // extra type bits still match
    CThingAICreature* me = ais[3];
    for (int i = 0; i < 16; ++i) SetUpCreature((CThingAICreature*)images[i]);
    unsigned long seeds[] = { 1, 12345, 0xdeadbeef, 77, 4096, 999999 };
    for (int s = 0; s < 6; ++s) {
        SetUpCreature(me);
        Run(world, nais, ais, me, seeds[s], s & 1, "suspended with a null suspender");
    }
    // empty world: no draws, no registrations, seed unchanged
    std::list<CClass*> empty;
    SetUpCreature(me);
    Run(empty, 0, ais, me, 555, 1, "suspended (empty world)");
    // a world of just the creature itself: draws happen (size 1: 0 draws), nothing registered
    std::list<CClass*> alone; alone.push_back((CClass*)me);
    CThingAICreature* only[1] = { me };
    SetUpCreature(me);
    Run(alone, 1, only, me, 31337, 0, "suspended (alone)");
    check(g_tree_erase_calls == 0, "no suspender erased");
    if (!g_fail) printf("00834530_TEST PASS\n");
    return g_fail;
}