// CHitParameters::SetAsModifiedByArmour(const CThing& thing, CDefPointer<const CArmourDef>& armour) -- collect the thing's armour defs (or the given armour at weight 1 when it has none), raise the recoil / negate / knock-down flags the armour asks for, then run every damage attribute through the armour tree (a ranged hit above the shot build-up time negates the hit), buff it by the hit's augmentations, scale it by the effective damage multiplier and, for absorbing things, clamp the total to the armour absorb value; no damage left and no guaranteed hit negates the hit
// Leaf: SetAsModifiedByArmour
// Retail: Fable.exe (TLC, VC7.1) @ 0x00869020, 955 bytes, `ret 8`.
// Shape notes (what the lane needs to reproduce retail's register/schedule choices):
//  * same-TU `__declspec(noinline)` stand-ins under `#pragma auto_inline(off)` for three callees whose retail bodies sat
//    in the same TU: the multimap _M_insert (0x00869CA0; only READS the pair, so the converted pair's key stays in esi
//    through the insert and both CDefPointer releases), GetArmourWeightsFromThing (0x00868E50; writes the multiplier by
//    reference without letting its address escape, which lets the `it->second = 0` store and the map-ctor stores sink
//    below the argument pushes as retail's do) and GetArmourResponseAugmentationDamageMultiplier (0x00868A80; writes only its float&, so
//    the augmentation loop's `Augmentations._M_finish` re-read is hoisted into edi after the entry test's eax load).
//    They are not inlined and are not part of the graded leaf.
//  * the augmentation loop runs on its own `float current = damage;` copy declared inside `if (numAugmentations > 0)`
//    (retail's `fstp st(0) ; fld [esp+0x18]` reload at the loop entry) and tests `aug != Augmentations._M_finish`.
//  * the absorb scale loop stores through a reference bound first (`float& entry = ...; entry = entry * scale;`) so the
//    Increment load stays below the fmul/fstp pair.
extern "C" void* __cdecl malloc(unsigned int size);            // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

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
        _M_header = (CRbNode*)malloc(sizeof(CArmourNode));
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
            free(_M_header);
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

// The armour helpers sit in this TU's anonymous namespace (Ego_r.pdb ?...@?A0x23f4dc30@@YI..., all taking the armour
// multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<const CArmourDef> >; sweep3, FINDINGS chunk 481).
namespace {
void __fastcall GetArmourWeightsFromThing(const CThing& thing, CArmourMap& armours, float& multiplier);   // 0x00868E50
float __fastcall GetToughnessDamageReductionFactorForThing(const CThing& thing);                       // 0x00868B40
float __fastcall GetArmourThresholdValueFromArmourWeights(EDamageAttribute attribute, const CArmourMap& armours, float multiplier);   // 0x008686E0
bool __fastcall GetArmourResponseDamageAttributeDamageMultiplier(EDamageAttribute attribute, const CArmourMap& armours, float multiplier, float& damage);   // 0x008687A0
bool __fastcall GetArmourResponseAugmentationDamageMultiplier(EObjectAugmentationType type, const CArmourMap& armours, float multiplier, float& damage);    // 0x00868A80
}

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
namespace {
__declspec(noinline) void __fastcall GetArmourWeightsFromThing(const CThing& thing, CArmourMap& armours, float& multiplier)
{
    multiplier = thing.ArmourAbsorb;
    if (thing.Flags6c & 1)
        armours.clear();
}
__declspec(noinline) bool __fastcall GetArmourResponseAugmentationDamageMultiplier(EObjectAugmentationType type, const CArmourMap& armours, float multiplier, float& damage)
{
    if (type > 0)
        damage += multiplier * (float)armours._M_node_count;
    return false;
}
}
#pragma auto_inline(on)

// Same-TU stand-in for the LTCG _M_insert (0x00869CA0): it only READS `v`, so VC7.1 keeps the converted pair's key in
// a register across the call instead of re-reading the pair's frame slot for the releases. Not inlined, not part of the leaf.
#pragma auto_inline(off)
__declspec(noinline) CArmourIterator CArmourMap::_M_insert(CRbNode* x, CRbNode* y, const CArmourPair& v, CRbNode* w)
{
    CArmourNode* z = (CArmourNode*)malloc(sizeof(CArmourNode));
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
    GetArmourWeightsFromThing(thing, armours, multiplier);
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

    float damageMultiplier = GetToughnessDamageReductionFactorForThing(thing);
    long numAugmentations = Augmentations._M_finish - Augmentations._M_start;
    bool anyDamage = false;
    float total = 0.0f;
    for (CRbNode* it = DamageMap._M_header->_M_left; it != DamageMap._M_header; it = Increment(it)) {
        float damage = ((CDamageNode*)it)->second;
        CRbNode* end = DamageMap._M_header;
        EDamageAttribute attribute = ((CDamageNode*)it)->first;
        if (DamageMap.find(4) != end && IsAlive(PWeapon.GetPItem()) && PWeapon->IsRangedWeapon()) {
            if (GetArmourThresholdValueFromArmourWeights(attribute, armours, multiplier) > ShotBuildupTime)
                HitNegated = true;
        }
        ((CDamageNode*)it)->second = 0.0f;
        if (GetArmourResponseDamageAttributeDamageMultiplier(attribute, armours, multiplier, damage)) {
            float value = damage;
            if (numAugmentations > 0) {
                float current = damage;
                EObjectAugmentationType* aug = Augmentations._M_start;
                for (; aug != Augmentations._M_finish; ++aug)
                {
                    float buff = current;
                    GetArmourResponseAugmentationDamageMultiplier(*aug, armours, multiplier, buff);
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