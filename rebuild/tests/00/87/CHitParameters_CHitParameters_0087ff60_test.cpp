#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#endif
static int allocations;
static size_t allocation_size;
extern "C" void* __cdecl observed_malloc(size_t n) { ++allocations; allocation_size=n; return malloc(n); }
#define malloc observed_malloc
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
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer(); // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void* Data;
    void SetPItem(CThing*); // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* p) { SetPItem(p); }
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



#undef malloc
class CThing { public: int ID; };
struct CHitLocation { int ID; };
static int base_ctors, bindings, dtors;
static CThing* bound[2];
CBaseIntelligentPointer::CBaseIntelligentPointer() : Data(0) { ++base_ctors; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++dtors; }
void CBaseIntelligentPointer::SetPItem(CThing* p) { bound[bindings++]=p; Data=p; }
#ifdef _MSC_VER
typedef char assert_node_size[sizeof(DamageNode)==24?1:-1];
typedef char assert_hit_size[sizeof(CHitParameters)==0x60?1:-1];
typedef char assert_hit_location[offsetof(CHitParameters,PHitLocation)==0x48?1:-1];
typedef char assert_hit_attacker[offsetof(CHitParameters,PThingHitting)==0x50?1:-1];
typedef char assert_hit_weapon[offsetof(CHitParameters,PWeapon)==0x58?1:-1];
#endif
static int check(CThing* attacker,CThing* weapon) {
    CHitLocation location;
    base_ctors=bindings=dtors=allocations=0;
    union Storage { void* align; double align2; unsigned char bytes[sizeof(CHitParameters)]; } storage;
    memset(storage.bytes,0xA5,sizeof(storage.bytes));
    CHitParameters* p=new(storage.bytes) CHitParameters(&location,attacker,weapon);
    if(p->Localised || p->Directional || p->KnockDown || p->Decapitate || p->Blockable || p->Flourish || p->EpicSpell || p->BlockCounter ||
       !p->PlayHitResponse || p->PlayHitResponseOverrideSet || !p->MoveBack || !p->CreateParticleEffectOnHit || !p->CreateDustParticleEffectOnHit ||
       p->GuaranteeHit || p->Blocked || p->HitNegated || p->CauseRecoil || p->ShotBuildupTime!=0) return 1;
// Uninitialized position/direction preservation is a native ABI check; their values are indeterminate in portable C++98.
#ifdef _MSC_VER
    for(int i=0;i<12;++i) if(p->Position[i]!=0xA5 || p->Direction[i]!=0xA5) return 2;
#endif
    if(allocations!=1 || allocation_size!=sizeof(DamageNode) || p->Damage.Count || !p->Damage.Header) return 3;
    DamageNode* head=p->Damage.Header;
    if(head->Colour || head->Parent || head->Left!=head || head->Right!=head) return 4;
    if(p->Augmentations.Start || p->Augmentations.Finish || p->Augmentations.EndOfStorage || p->PHitMaterialDef || p->PHitLocation!=&location) return 5;
    if(base_ctors!=2 || bindings!=2 || bound[0]!=attacker || bound[1]!=weapon || p->PThingHitting.Data!=attacker || p->PWeapon.Data!=weapon) return 6;
    p->~CHitParameters();
    if(dtors!=2) return 7;
    free(head);
    return 0;
}
int main() {
    CThing attacker,weapon;
    int r=check(&attacker,&weapon); if(!r) r=check(0,&weapon); if(!r) r=check(&attacker,0); if(!r) r=check(0,0);
    if(r) { printf("0087ff60_TEST FAIL %d\n",r); return 1; }
    printf("0087ff60_TEST PASS\n"); return 0;
}