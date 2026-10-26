// Behaviour test for 00869020: final.cpp copied verbatim (malloc/free renamed to fable_malloc/fable_free wrappers so the
// test can include <cstdlib>), the extern callees stubbed below so calls and arguments are observable.
// CHitParameters::SetAsModifiedByArmour(const CThing&, CDefPointer<const CArmourDef>&)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_mallocs = 0, g_frees = 0;
extern "C" void* __cdecl fable_malloc(unsigned int size) { ++g_mallocs; return malloc(size); }
extern "C" void __cdecl fable_free(void* block) { ++g_frees; free(block); }
// ---- source (verbatim, allocator names aside) ----
// CHitParameters::SetAsModifiedByArmour(const CThing& thing, CDefPointer<const CArmourDef>& armour) -- collect the thing's armour defs (or the given armour at weight 1 when it has none), raise the recoil / negate / knock-down flags the armour asks for, then run every damage attribute through the armour tree (a ranged hit above the shot build-up time negates the hit), buff it by the hit's augmentations, scale it by the effective damage multiplier and, for absorbing things, clamp the total to the armour absorb value; no damage left and no guaranteed hit negates the hit
// Leaf: SetAsModifiedByArmour
// Retail: Fable.exe (TLC, VC7.1) @ 0x00869020, 955 bytes, `ret 8`.
// Shape notes (what the lane needs to reproduce retail's register/schedule choices):
//  * same-TU `__declspec(noinline)` stand-ins under `#pragma auto_inline(off)` for three callees whose retail bodies sat
//    in the same TU: the multimap _M_insert (0x00869CA0; only READS the pair, so the converted pair's key stays in esi
//    through the insert and both CDefPointer releases), GetArmourDefsForThing (0x00868E50; writes the multiplier by
//    reference without letting its address escape, which lets the `it->second = 0` store and the map-ctor stores sink
//    below the argument pushes as retail's do) and AccumulateMeleeBuffFromTree (0x00868A80; writes only its float&, so
//    the augmentation loop's `Augmentations._M_finish` re-read is hoisted into edi after the entry test's eax load).
//    They are not inlined and are not part of the graded leaf.
//  * the augmentation loop runs on its own `float current = damage;` copy declared inside `if (numAugmentations > 0)`
//    (retail's `fstp st(0) ; fld [esp+0x18]` reload at the loop entry) and tests `aug != Augmentations._M_finish`.
//  * the absorb scale loop stores through a reference bound first (`float& entry = ...; entry = entry * scale;`) so the
//    Increment load stays below the fmul/fstp pair.
extern "C" void* __cdecl fable_malloc(unsigned int size);      // 0x00BFEA0E malloc
extern "C" void __cdecl fable_free(void* block);               // 0x00BFEA14 free

struct CRbNode {
    unsigned char _M_color;
    CRbNode* _M_parent;
    CRbNode* _M_left;
    CRbNode* _M_right;
};

class CArmourDef {
public:
    virtual void v00();
    virtual void DeleteThis();
    long          RefCount;                                    // +0x04
    unsigned char _pad08[0x60 - 0x08];
    bool          CausesKnockDown;                             // +0x60 (descriptive)
    bool          NegatesHit;                                  // +0x61 (descriptive)
    bool          CausesRecoil;                                // +0x62 (descriptive)
};

struct CArmourDefPointer {                                     // CDefPointer<const CArmourDef>
    CArmourDef* Ptr;
    __forceinline CArmourDefPointer(const CArmourDefPointer& other) : Ptr(other.Ptr) { if (Ptr) ++Ptr->RefCount; }
    __forceinline ~CArmourDefPointer() { if (Ptr) { if (--Ptr->RefCount == 0) Ptr->DeleteThis(); } }
    CArmourDef* operator->() const { return Ptr; }
};

struct CArmourPairTemp {                                       // pair<CDefPointer<const CArmourDef>, float>
    CArmourDefPointer first;
    float second;
    CArmourPairTemp(const CArmourDefPointer& a, const float& b) : first(a), second(b) { }
};
struct CArmourPair {                                           // pair<const CDefPointer<const CArmourDef>, float>
    const CArmourDefPointer first;
    float second;
    CArmourPair(const CArmourPairTemp& p) : first(p.first), second(p.second) { }
};
inline CArmourPairTemp MakeArmourPair(const CArmourDefPointer& a, const float& b) { return CArmourPairTemp(a, b); }

struct CArmourNode : CRbNode { CArmourPair _M_value_field; };

struct CArmourIterator {
    CRbNode* _M_node;
    CArmourIterator() { }
    CArmourIterator(CRbNode* x) : _M_node(x) { }
    CArmourIterator(const CArmourIterator& other) : _M_node(other._M_node) { }
};

struct CArmourMap {                                            // multimap<CDefPointer<const CArmourDef>, float> (STLport, 12 bytes)
    CRbNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    CArmourMap() : _M_header(0)
    {
        _M_header = (CRbNode*)fable_malloc(sizeof(CArmourNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    ~CArmourMap()
    {
        clear();
        if (_M_header)
            fable_free(_M_header);
    }
    void _M_erase(CRbNode* x);                                 // 0x00869BB0
    CArmourIterator _M_insert(CRbNode* x, CRbNode* y, const CArmourPair& v, CRbNode* w);   // 0x00869CA0
    bool empty() const { return _M_node_count == 0; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    CArmourIterator insert(const CArmourPair& v)
    {
        CRbNode* y = _M_header;
        CRbNode* x = _M_header->_M_parent;
        while (x != 0) {
            y = x;
            x = v.first.Ptr < ((CArmourNode*)x)->_M_value_field.first.Ptr ? x->_M_left : x->_M_right;
        }
        return _M_insert(x, y, v, 0);
    }
};

typedef long EDamageAttribute;
typedef long EObjectAugmentationType;

struct CDamageNode : CRbNode { EDamageAttribute first; float second; };

__forceinline CRbNode* Increment(CRbNode* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    } else {
        CRbNode* y = node->_M_parent;
        while (node == y->_M_right) {
            node = y;
            y = y->_M_parent;
        }
        if (node->_M_right != y)
            node = y;
    }
    return node;
}

struct CDamageMap {                                            // map<EDamageAttribute, float> (STLport, 12 bytes)
    CRbNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_key_compare;
    CRbNode* end() const { return _M_header; }
    CRbNode* find(const EDamageAttribute& k) const
    {
        CRbNode* y = _M_header;
        CRbNode* x = _M_header->_M_parent;
        while (x != 0) {
            if (!(((CDamageNode*)x)->first < k)) {
                y = x;
                x = x->_M_left;
            } else
                x = x->_M_right;
        }
        if (y == _M_header || k < ((CDamageNode*)y)->first)
            y = _M_header;
        return y;
    }
};

struct CAugmentationArray {                                    // vector<EObjectAugmentationType> (12 bytes retail)
    EObjectAugmentationType* _M_start;
    EObjectAugmentationType* _M_finish;
    EObjectAugmentationType* _M_end_of_storage;
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;          // 0x004365B0
};
class CTCWeapon { public:                                             // interface 0x77 (descriptive)
    bool IsRanged() const;                                     // 0x007BED60 (descriptive)
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                                   // +0x10 (descriptive)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                            // +0x20
    CInterfaceMap m_interfaces;                                // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6c;                                     // +0x6c (descriptive)
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char Dead : 1;                                    // +0x91 bit 0
    unsigned char _pad92[0xb4 - 0x92];
    float         ArmourAbsorb;                                // +0xb4 (descriptive)
    unsigned char _padb8[0x1c8 - 0xb8];
    unsigned char Flags1c8;                                    // +0x1c8 (descriptive)
    __forceinline bool IsRangedWeapon() const
    {
        if (m_presentBits[3] & 0x800000) {
            long type = 0x77;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            return ((CTCWeapon*)(it == map->last || 0x77 < it->type ? map->last : it)->component)->IsRanged();
        }
        return false;
    }
};
inline bool IsAlive(const CThing* thing) { return thing && !thing->Dead; }

struct CBaseIntelligentPointer {
    void* Ptr;
    long  Index;
    CThing* GetPItem() const;                                  // 0x00A01B50
    CThing* operator->() const { return GetPItem(); }
};

void __fastcall GetArmourDefsForThing(const CThing& thing, CArmourMap& armours, float& multiplier);   // 0x00868E50
float __fastcall CalculateEffectiveDamageMultiplier(const CThing& thing);                             // 0x00868B40
float __fastcall CalculateDamageFromArmourTree(EDamageAttribute attribute, const CArmourMap& armours, float multiplier);   // 0x008686E0
bool __fastcall AccumulateDamageModifierFromTree(EDamageAttribute attribute, const CArmourMap& armours, float multiplier, float& damage);   // 0x008687A0
void __fastcall AccumulateMeleeBuffFromTree(EObjectAugmentationType type, const CArmourMap& armours, float multiplier, float& damage);    // 0x00868A80

// Layout: retail CHitParameters keeps the PDB bool/float offsets up to +0x2c, then 12-byte STLport containers:
// DamageMap +0x30, Augmentations +0x3c, PHitLocation +0x48, PHitMaterialDef +0x4c, PThingHitting +0x50, PWeapon +0x58.
class CHitParameters {
public:
    unsigned char _pad00[0x1a];
    bool          KnockDown;                                   // +0x1a
    unsigned char _pad1b[0x25 - 0x1b];
    bool          GuaranteeHit;                                // +0x25
    bool          Blocked;                                     // +0x26
    bool          HitNegated;                                  // +0x27
    bool          CauseRecoil;                                 // +0x28
    float         ShotBuildupTime;                             // +0x2c
    CDamageMap    DamageMap;                                   // +0x30
    CAugmentationArray Augmentations;                          // +0x3c
    void*         PHitLocation;                                // +0x48
    void*         PHitMaterialDef;                             // +0x4c
    CBaseIntelligentPointer PThingHitting;                     // +0x50
    CBaseIntelligentPointer PWeapon;                           // +0x58
    void SetAsModifiedByArmour(const CThing& thing, CArmourDefPointer& armour);
};

// Same-TU read-mostly stand-ins for the __fastcall armour helpers (retail bodies are elsewhere; not inlined, not part of the leaf).
#pragma auto_inline(off)
__declspec(noinline) void __fastcall GetArmourDefsForThing(const CThing& thing, CArmourMap& armours, float& multiplier)
{
    multiplier = thing.ArmourAbsorb;
    if (thing.Flags6c & 1)
        armours.clear();
}
__declspec(noinline) void __fastcall AccumulateMeleeBuffFromTree(EObjectAugmentationType type, const CArmourMap& armours, float multiplier, float& damage)
{
    if (type > 0)
        damage += multiplier * (float)armours._M_node_count;
}
#pragma auto_inline(on)

// Same-TU stand-in for the LTCG _M_insert (0x00869CA0): it only READS `v`, so VC7.1 keeps the converted pair's key in
// a register across the call instead of re-reading the pair's frame slot for the releases. Not inlined, not part of the leaf.
#pragma auto_inline(off)
__declspec(noinline) CArmourIterator CArmourMap::_M_insert(CRbNode* x, CRbNode* y, const CArmourPair& v, CRbNode* w)
{
    CArmourNode* z = (CArmourNode*)fable_malloc(sizeof(CArmourNode));
    CArmourDef* def = v.first.Ptr;
    ((CArmourDefPointer*)&z->_M_value_field.first)->Ptr = def;
    if (def)
        ++def->RefCount;
    z->_M_value_field.second = v.second;
    if (y == _M_header || x != 0 || def < ((CArmourNode*)y)->_M_value_field.first.Ptr) {
        y->_M_left = z;
        if (y == _M_header) {
            _M_header->_M_parent = z;
            _M_header->_M_right = z;
        } else if (y == _M_header->_M_left)
            _M_header->_M_left = z;
    } else {
        y->_M_right = z;
        if (y == _M_header->_M_right)
            _M_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    z->_M_color = 0;
    ++_M_node_count;
    return CArmourIterator(z);
}
#pragma auto_inline(on)

void CHitParameters::SetAsModifiedByArmour(const CThing& thing, CArmourDefPointer& armour)
{
    CArmourMap armours;
    float multiplier = 0.0f;
    GetArmourDefsForThing(thing, armours, multiplier);
    if (armours.empty()) {
        armours.insert(MakeArmourPair(armour, 1.0f));
        multiplier = 1.0f;
    }
    if (armour->CausesRecoil)
        CauseRecoil = true;
    if (armour->NegatesHit)
        HitNegated = true;
    if (armour->CausesKnockDown)
        KnockDown = true;
    if (thing.ThingType == 1 && (thing.Flags1c8 & 0x40))
        HitNegated = true;

    float damageMultiplier = CalculateEffectiveDamageMultiplier(thing);
    long numAugmentations = Augmentations._M_finish - Augmentations._M_start;
    bool anyDamage = false;
    float total = 0.0f;
    for (CRbNode* it = DamageMap._M_header->_M_left; it != DamageMap._M_header; it = Increment(it)) {
        float damage = ((CDamageNode*)it)->second;
        CRbNode* end = DamageMap._M_header;
        EDamageAttribute attribute = ((CDamageNode*)it)->first;
        if (DamageMap.find(4) != end && IsAlive(PWeapon.GetPItem()) && PWeapon->IsRangedWeapon()) {
            if (CalculateDamageFromArmourTree(attribute, armours, multiplier) > ShotBuildupTime)
                HitNegated = true;
        }
        ((CDamageNode*)it)->second = 0.0f;
        if (AccumulateDamageModifierFromTree(attribute, armours, multiplier, damage)) {
            float value = damage;
            if (numAugmentations > 0) {
                float current = damage;
                EObjectAugmentationType* aug = Augmentations._M_start;
                for (; aug != Augmentations._M_finish; ++aug)
                {
                    float buff = current;
                    AccumulateMeleeBuffFromTree(*aug, armours, multiplier, buff);
                    current = buff;
                }
                value = current;
            }
            if (value > 0.0001f)
                anyDamage = true;
            ((CDamageNode*)it)->second = value * damageMultiplier;
            total += ((CDamageNode*)it)->second;
        }
    }
    if ((thing.Flags6c & 2) && total > 0.0001f) {
        float scale = thing.ArmourAbsorb / total;
        if (scale < 1.0f) {
            for (CRbNode* it = DamageMap._M_header->_M_left; it != DamageMap._M_header; it = Increment(it)) {
                float& entry = ((CDamageNode*)it)->second;
                entry = entry * scale;
            }
        }
    }
    if (!anyDamage && !GuaranteeHit)
        HitNegated = true;
}

// ---- stubs ----
static int g_deleted = 0;
void CArmourDef::v00() { }
void CArmourDef::DeleteThis() { ++g_deleted; }
static float g_effMult = 1.0f, g_treeDamage = 0.0f, g_modScale = 1.0f; static bool g_modOk = true, g_ranged = false;
static int g_modCalls = 0; static float g_modMultiplier = -1.0f; static long g_modAttrs[8]; static unsigned long g_modNodes = 99;
float __fastcall CalculateEffectiveDamageMultiplier(const CThing&) { return g_effMult; }
float __fastcall CalculateDamageFromArmourTree(EDamageAttribute, const CArmourMap&, float) { return g_treeDamage; }
bool __fastcall AccumulateDamageModifierFromTree(EDamageAttribute attribute, const CArmourMap& armours, float multiplier, float& damage)
{
    if (g_modCalls < 8) g_modAttrs[g_modCalls] = attribute;
    ++g_modCalls; g_modMultiplier = multiplier; g_modNodes = armours._M_node_count;
    damage = damage * g_modScale;
    return g_modOk;
}
static void ReleaseDef(CArmourDef* def) { if (def && --def->RefCount == 0) def->DeleteThis(); }
void CArmourMap::_M_erase(CRbNode* x)
{
    while (x) { _M_erase(x->_M_right); CRbNode* left = x->_M_left; ReleaseDef(((CArmourNode*)x)->_M_value_field.first.Ptr); fable_free(x); x = left; }
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first; while (it != last && it->type < *type) ++it; return it;
}
bool CTCWeapon::IsRanged() const { return g_ranged; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)Ptr; }

// ---- a hand-built map<EDamageAttribute, float>: header + up to three nodes linked as a right-leaning chain ----
struct DamageTree {
    CRbNode header; CDamageNode node[3]; int count;
    void Build(const long* keys, const float* values, int n)
    {
        count = n; memset(&header, 0, sizeof(header)); memset(node, 0, sizeof(node));
        header._M_left = &header; header._M_right = &header;
        for (int i = 0; i < n; ++i) {
            node[i].first = keys[i]; node[i].second = values[i];
            node[i]._M_parent = i ? (CRbNode*)&node[i - 1] : &header;
            if (i) node[i - 1]._M_right = &node[i];
        }
        if (n) { header._M_parent = &node[0]; header._M_left = &node[0]; header._M_right = &node[n - 1]; }
    }
};
static float Near(float a, float b) { float d = a - b; return (d < 0 ? -d : d) < 1e-4f; }

struct Fixture {
    CThing thing; CHitParameters hit; DamageTree tree; CArmourDef def; CArmourDefPointer armour; CThing weapon; CTCWeapon tc;
    CInterfaceMap::SEntry entry; EObjectAugmentationType augs[3];
    Fixture() : armour(MakeArmourPointer())
    {
        memset(&thing, 0, sizeof(thing)); memset(&hit, 0, sizeof(hit)); memset(&def, 0, sizeof(def)); memset(&weapon, 0, sizeof(weapon));
        def.RefCount = 1; armour.Ptr = &def;
        tree.Build(0, 0, 0); hit.DamageMap._M_header = &tree.header; hit.DamageMap._M_node_count = 0;
        hit.Augmentations._M_start = augs; hit.Augmentations._M_finish = augs; hit.Augmentations._M_end_of_storage = augs + 3;
        hit.PWeapon.Ptr = 0; hit.ShotBuildupTime = 2.0f;
        entry.type = 0x77; entry.component = (CTCBase*)&tc;
        weapon.m_interfaces.first = &entry; weapon.m_interfaces.last = &entry + 1;
        g_effMult = 1.0f; g_treeDamage = 0.0f; g_modScale = 1.0f; g_modOk = true; g_ranged = false; g_modCalls = 0; g_modMultiplier = -1.0f; g_modNodes = 99; g_deleted = 0;
    }
    static CArmourDefPointer MakeArmourPointer() { CArmourDefPointer* p = (CArmourDefPointer*)calloc(1, sizeof(CArmourDefPointer)); return *p; }
    void Damage(long k0, float v0, long k1, float v1) { long keys[2] = { k0, k1 }; float values[2] = { v0, v1 }; tree.Build(keys, values, 2); hit.DamageMap._M_node_count = 2; }
    void Damage3(long k0, float v0, long k1, float v1, long k2, float v2) { long keys[3] = { k0, k1, k2 }; float values[3] = { v0, v1, v2 }; tree.Build(keys, values, 3); hit.DamageMap._M_node_count = 3; }
    void Run() { hit.SetAsModifiedByArmour(thing, armour); }
    ~Fixture() { armour.Ptr = 0; }
};

int main()
{
    {   // plain hit: two attributes, modifier halves, effective multiplier doubles -> values unchanged, total 15, hit stands
        Fixture f; f.Damage(1, 10.0f, 2, 5.0f); g_modScale = 0.5f; g_effMult = 2.0f;
        int mallocs = g_mallocs, frees = g_frees;
        f.Run();
        CHECK(g_modCalls == 2 && g_modAttrs[0] == 1 && g_modAttrs[1] == 2);
        CHECK(g_modMultiplier == 1.0f && g_modNodes == 1);                 // the thing had no armour: the given armour at weight 1
        CHECK(Near(f.tree.node[0].second, 10.0f) && Near(f.tree.node[1].second, 5.0f));
        CHECK(!f.hit.HitNegated && !f.hit.CauseRecoil && !f.hit.KnockDown);
        CHECK(f.def.RefCount == 1 && g_deleted == 0);                      // temporaries and the erased node all released
        CHECK(g_mallocs - mallocs == 2 && g_frees - frees == 2);           // header + one node, both freed by the map dtor
    }
    {   // armour flags propagate
        Fixture f; f.Damage(1, 1.0f, 2, 1.0f); f.def.CausesRecoil = true; f.def.NegatesHit = true; f.def.CausesKnockDown = true;
        f.Run(); CHECK(f.hit.CauseRecoil && f.hit.HitNegated && f.hit.KnockDown);
    }
    {   // creature (type 1) with flag 0x40 negates
        Fixture f; f.Damage(1, 1.0f, 2, 1.0f); f.thing.ThingType = 1; f.thing.Flags1c8 = 0x40; f.Run(); CHECK(f.hit.HitNegated);
        Fixture g; g.Damage(1, 1.0f, 2, 1.0f); g.thing.ThingType = 2; g.thing.Flags1c8 = 0x40; g.Run(); CHECK(!g.hit.HitNegated);
    }
    {   // ranged weapon with attribute 4 present: tree damage above the shot build-up time negates
        Fixture f; f.Damage3(1, 1.0f, 2, 1.0f, 4, 1.0f); f.hit.PWeapon.Ptr = &f.weapon; f.weapon.m_presentBits[3] = 0x800000; g_ranged = true; g_treeDamage = 3.0f;
        f.Run(); CHECK(f.hit.HitNegated);
        Fixture g; g.Damage3(1, 1.0f, 2, 1.0f, 4, 1.0f); g.hit.PWeapon.Ptr = &g.weapon; g.weapon.m_presentBits[3] = 0x800000; g_ranged = true; g_treeDamage = 1.0f;
        g.Run(); CHECK(!g.hit.HitNegated);
        Fixture h; h.Damage3(1, 1.0f, 2, 1.0f, 4, 1.0f); h.hit.PWeapon.Ptr = &h.weapon; h.weapon.m_presentBits[3] = 0x800000; h.weapon.Dead = 1; g_ranged = true; g_treeDamage = 3.0f;
        h.Run(); CHECK(!h.hit.HitNegated);                                  // dead weapon thing: no ranged check
        Fixture i; i.Damage(1, 1.0f, 2, 1.0f); i.hit.PWeapon.Ptr = &i.weapon; i.weapon.m_presentBits[3] = 0x800000; g_ranged = true; g_treeDamage = 3.0f;
        i.Run(); CHECK(!i.hit.HitNegated);                                  // no attribute 4: no ranged check
    }
    {   // augmentations buff each attribute (stand-in: +multiplier*nodes per positive augmentation type)
        Fixture f; f.Damage(1, 2.0f, 2, 3.0f); f.augs[0] = 1; f.augs[1] = 0; f.augs[2] = 5; f.hit.Augmentations._M_finish = f.augs + 3;
        f.Run(); CHECK(Near(f.tree.node[0].second, 4.0f) && Near(f.tree.node[1].second, 5.0f));
    }
    {   // modifier rejects every attribute: damage zeroed, no damage left -> negated unless guaranteed
        Fixture f; f.Damage(1, 2.0f, 2, 3.0f); g_modOk = false; f.Run();
        CHECK(f.tree.node[0].second == 0.0f && f.tree.node[1].second == 0.0f && f.hit.HitNegated);
        Fixture g; g.Damage(1, 2.0f, 2, 3.0f); g_modOk = false; g.hit.GuaranteeHit = true; g.Run(); CHECK(!g.hit.HitNegated);
    }
    {   // absorbing thing (flag 2): total 15 clamped to absorb 3 -> every entry scaled by 0.2; absorb above the total leaves it
        Fixture f; f.Damage(1, 10.0f, 2, 5.0f); f.thing.Flags6c = 2; f.thing.ArmourAbsorb = 3.0f; f.Run();
        CHECK(Near(f.tree.node[0].second, 2.0f) && Near(f.tree.node[1].second, 1.0f) && !f.hit.HitNegated);
        Fixture g; g.Damage(1, 10.0f, 2, 5.0f); g.thing.Flags6c = 2; g.thing.ArmourAbsorb = 30.0f; g.Run();
        CHECK(Near(g.tree.node[0].second, 10.0f) && Near(g.tree.node[1].second, 5.0f));
        Fixture h; h.Damage(1, 10.0f, 2, 5.0f); h.thing.Flags6c = 0; h.thing.ArmourAbsorb = 3.0f; h.Run();
        CHECK(Near(h.tree.node[0].second, 10.0f));
    }
    {   // empty damage map: nothing to modify -> negated (no guaranteed hit)
        Fixture f; f.Run(); CHECK(g_modCalls == 0 && f.hit.HitNegated);
    }
    if (fails) { printf("00869020_TEST FAIL (%d)\n", fails); return 1; }
    printf("00869020_TEST PASS\n");
    return 0;
}