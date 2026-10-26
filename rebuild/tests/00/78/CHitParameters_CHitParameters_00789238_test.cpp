// Behaviour test for CHitParameters::CHitParameters (0x00789238): every argument lands in its member and the
// defaults are set, built over garbage-filled storage.
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>
static int g_maps = 0;
class CThing;
struct CHitLocation;
class CMaterialDef;

#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)

struct CDamageMap {                                // map<EDamageAttribute, float> (12 bytes in retail)
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CDamageMap() : _M_header(0), _M_node_count(0), _M_key_compare(0) { ++g_maps; }
};

template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
};

struct CDefPointer {                               // CDefPointer<const CMaterialDef>
    const CMaterialDef* Object;
    CDefPointer() : Object(0) { }
};

struct CIntelligentPointer {                       // CIntelligentPointer<CThing>: vptr + item
    void* __vftable;
    void* m_pItem;
    CIntelligentPointer(CThing* thing) : __vftable(0), m_pItem(thing) { }
};

#pragma pack(push, 1)
class CHitParameters { public:
    bool          Localised;                       // +0x00
    bool          Directional;                     // +0x01
    C3DVector     Position;                        // +0x02
    C3DVector     Direction;                       // +0x0e
    bool          KnockDown;                       // +0x1a
    bool          Decapitate;                      // +0x1b
    bool          Blockable;                       // +0x1c
    bool          Flourish;                        // +0x1d
    bool          EpicSpell;                       // +0x1e
    bool          BlockCounter;                    // +0x1f
    bool          PlayHitResponse;                 // +0x20
    bool          PlayHitResponseOverrideSet;      // +0x21
    bool          MoveBack;                        // +0x22
    bool          CreateParticleEffectOnHit;       // +0x23
    bool          CreateDustParticleEffectOnHit;   // +0x24
    bool          GuaranteeHit;                    // +0x25
    bool          Blocked;                         // +0x26
    bool          HitNegated;                      // +0x27
    bool          CauseRecoil;                     // +0x28
    unsigned char _pad29[3];
    float         ShotBuildupTime;                 // +0x2c
    CDamageMap    DamageMap;                       // +0x30
    CRetailVector<long> Augmentations;             // +0x3c vector<EObjectAugmentationType>
    CHitLocation* PHitLocation;                    // +0x48
    CDefPointer   PHitMaterialDef;                 // +0x4c
    CIntelligentPointer PThingHitting;             // +0x50
    CIntelligentPointer PWeapon;                   // +0x58
    CHitParameters(CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter);
};
#pragma pack(pop)

CHitParameters::CHitParameters(CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon, const C3DVector& position,
                               const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter)
    : Localised(true)
    , Directional(true)
    , Position(position)
    , Direction(direction)
    , KnockDown(knockDown)
    , Decapitate(false)
    , Blockable(blockable)
    , Flourish(flourish)
    , EpicSpell(false)
    , BlockCounter(blockCounter)
    , PlayHitResponse(true)
    , PlayHitResponseOverrideSet(false)
    , MoveBack(true)
    , CreateParticleEffectOnHit(true)
    , CreateDustParticleEffectOnHit(true)
    , GuaranteeHit(false)
    , Blocked(false)
    , HitNegated(false)
    , CauseRecoil(false)
    , ShotBuildupTime(0.0f)
    , PHitLocation(hitLocation)
    , PThingHitting(thingHitting)
    , PWeapon(weapon)
{
}

int main()
{
    void* mem = malloc(sizeof(CHitParameters)); memset(mem, 0xcd, sizeof(CHitParameters));
    float pv[3] = { 1.0f, 2.0f, 3.0f }, dv[3] = { -4.0f, 5.5f, 0.25f };
    int loc = 0, a = 0, b = 0;
    CHitParameters* h = new (mem) CHitParameters((CHitLocation*)&loc, (CThing*)&a, (CThing*)&b, *(C3DVector*)pv, *(C3DVector*)dv,
                                                 true, false, true, false);
    int bad = 0;
    if (!h->Localised || !h->Directional || h->Position.X != 1.0f || h->Position.Z != 3.0f || h->Direction.Y != 5.5f || h->Direction.Z != 0.25f) bad = 1;
    if (!h->KnockDown || h->Blockable || !h->Flourish || h->BlockCounter || h->Decapitate || h->EpicSpell) bad = 2;
    if (!h->PlayHitResponse || h->PlayHitResponseOverrideSet || !h->MoveBack || !h->CreateParticleEffectOnHit || !h->CreateDustParticleEffectOnHit) bad = 3;
    if (h->GuaranteeHit || h->Blocked || h->HitNegated || h->CauseRecoil || h->ShotBuildupTime != 0.0f) bad = 4;
    if (g_maps != 1 || h->Augmentations._M_start || h->Augmentations._M_finish || h->Augmentations._M_end_of_storage._M_data) bad = 5;
    if (h->PHitLocation != (CHitLocation*)&loc || h->PHitMaterialDef.Object || h->PThingHitting.m_pItem != (void*)&a || h->PWeapon.m_pItem != (void*)&b) bad = 6;
    CHitParameters* h2 = new (mem) CHitParameters(0, 0, 0, *(C3DVector*)pv, *(C3DVector*)dv, false, true, false, true);
    if (h2->KnockDown || !h2->Blockable || h2->Flourish || !h2->BlockCounter || h2->PWeapon.m_pItem) bad = 7;
    if (bad) { std::printf("00789238_TEST FAIL\n"); return 1; }
    std::printf("00789238_TEST PASS\n");
    return 0;
}