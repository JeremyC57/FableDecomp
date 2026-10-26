// CHitParameters::CHitParameters(const CHitParameters&) -- actual copy constructor, PDB00807C70
// Leaf: CHitParameters
#include "engine/CBaseObject.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/_quarantine/CMaterialDef.h"
// No CHitParameters/C3DVector/intelligent-pointer class headers exist. Native map/vector sizes12 vs donor16 shift the tail by8.
#pragma pack(push,1)
class C3DVector { public:C3DVector();C3DVector(const C3DVector&o):X(o.X),Y(o.Y),Z(o.Z){} float X,Y,Z;};
#pragma pack(pop)
struct DamageMap {DamageMap();DamageMap(const DamageMap&);~DamageMap();void*Head;unsigned long Size;unsigned long Allocator;}; //004F97B3 copies native tree,12 bytes
struct AugmentationVector {AugmentationVector();AugmentationVector(const AugmentationVector&);~AugmentationVector();long*First,*Last,*End;}; //00811D40, enum vector copy12 bytes
struct CHitLocation;
struct CBaseIntelligentPointer {CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObject* GetPItem()const;void SetPItem(CBaseObject*);CBaseObjectPointer*PData;}; //A01B10/A01C10/A01B50/A01B90
struct ThingPointer:CBaseIntelligentPointer {ThingPointer();ThingPointer(const ThingPointer&o):CBaseIntelligentPointer(){SetPItem(o.GetPItem());}virtual ~ThingPointer(){}}; // native1238C6C sole slot deleting dtor004A9710 -> A01C10
// CDefPointer<CMaterialDef const> copy PDB01D83F90; unsigned inherited refcount+4.
enum ENullDef {NullDef=0};
struct MaterialPointer {MaterialPointer(ENullDef);~MaterialPointer();MaterialPointer(const MaterialPointer&o):P(o.P){if(P)++reinterpret_cast<CIVCountedPointeeBase*>(const_cast<CMaterialDef*>(P))->IVRefCount;}const CMaterialDef*P;};
class CHitParameters { public:
 CHitParameters();CHitParameters(const CHitParameters&);
 bool Localised,Directional;C3DVector Position,Direction;
 bool KnockDown,Decapitate,Blockable,Flourish,EpicSpell,BlockCounter,PlayHitResponse,PlayHitResponseOverrideSet,MoveBack,CreateParticleEffectOnHit,CreateDustParticleEffectOnHit,GuaranteeHit,Blocked,HitNegated,CauseRecoil;
 float ShotBuildupTime;DamageMap Damage;AugmentationVector Augmentations;CHitLocation*PHitLocation;MaterialPointer PHitMaterialDef;ThingPointer PThingHitting,PWeapon;
};
CHitParameters::CHitParameters(const CHitParameters&o):Localised(o.Localised),Directional(o.Directional),Position(o.Position),Direction(o.Direction),KnockDown(o.KnockDown),Decapitate(o.Decapitate),Blockable(o.Blockable),Flourish(o.Flourish),EpicSpell(o.EpicSpell),BlockCounter(o.BlockCounter),PlayHitResponse(o.PlayHitResponse),PlayHitResponseOverrideSet(o.PlayHitResponseOverrideSet),MoveBack(o.MoveBack),CreateParticleEffectOnHit(o.CreateParticleEffectOnHit),CreateDustParticleEffectOnHit(o.CreateDustParticleEffectOnHit),GuaranteeHit(o.GuaranteeHit),Blocked(o.Blocked),HitNegated(o.HitNegated),CauseRecoil(o.CauseRecoil),ShotBuildupTime(o.ShotBuildupTime),Damage(o.Damage),Augmentations(o.Augmentations),PHitLocation(o.PHitLocation),PHitMaterialDef(o.PHitMaterialDef),PThingHitting(o.PThingHitting),PWeapon(o.PWeapon){}