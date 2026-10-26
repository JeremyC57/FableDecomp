// CHitParameters::CHitParameters(CHitLocation*, CThing*, CThing*) -- initialize default hit flags and empty containers, bind attacker and weapon.
// Retail 0087FF60, 184 bytes. No engine/CHitParameters.h exists; real member names from struct_layouts_egor.tsv.
// Native STLport map is 8 bytes versus donor 16; vector remains 16; PHitLocation is +48, pointers +50/+58.
// malloc import BFEA0E -> IAT1440158 is mandatory, not operator new. Base intelligent ctor A01B10; SetPItem A01B90;
// CIntelligentPointer<CThing> vtable1238C6C. Composite vector positions are uninitialized packed float triplets.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t); // 0x00BFEA0E
class CThing;
struct CHitLocation;
struct DamageNode {
    unsigned char Colour;
    DamageNode* Parent;
    DamageNode* Left;
    DamageNode* Right;
    int Attribute;
    float Damage;
};
struct DamageMap {
    DamageNode* Header;
    unsigned long Count;
    DamageMap() : Header(0) {
        Header = (DamageNode*)malloc(sizeof(DamageNode));
        Count = 0;
        Header->Colour = 0;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
};
struct AugmentationVector {
    unsigned long AllocatorState;
    int* Start;
    int* Finish;
    int* EndOfStorage;
    AugmentationVector() : Start(0), Finish(0), EndOfStorage(0) {}
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* p) { SetPItem(p); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CHitParameters { public:
    bool Localised, Directional;
    unsigned char Position[12], Direction[12];
    bool KnockDown, Decapitate, Blockable, Flourish, EpicSpell, BlockCounter;
    bool PlayHitResponse, PlayHitResponseOverrideSet, MoveBack, CreateParticleEffectOnHit, CreateDustParticleEffectOnHit;
    bool GuaranteeHit, Blocked, HitNegated, CauseRecoil;
    float ShotBuildupTime;
    DamageMap Damage;
    AugmentationVector Augmentations;
    CHitLocation* PHitLocation;
    void* PHitMaterialDef;
    CIntelligentPointer<CThing> PThingHitting, PWeapon;
    CHitParameters(CHitLocation*, CThing*, CThing*);
};
CHitParameters::CHitParameters(CHitLocation* location, CThing* attacker, CThing* weapon)
: Localised(false), Directional(false), KnockDown(false), Decapitate(false), Blockable(false), Flourish(false), EpicSpell(false), BlockCounter(false),
  PlayHitResponse(true), PlayHitResponseOverrideSet(false), MoveBack(true), CreateParticleEffectOnHit(true), CreateDustParticleEffectOnHit(true),
  GuaranteeHit(false), Blocked(false), HitNegated(false), CauseRecoil(false), ShotBuildupTime(0),
  PHitLocation(location), PHitMaterialDef(0), PThingHitting(attacker), PWeapon(weapon) {}