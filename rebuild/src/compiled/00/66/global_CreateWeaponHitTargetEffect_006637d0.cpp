// 006637d0 CreateWeaponHitTargetEffect
// Flags: /O2 /Oy /W3 
// Leaf: CreateWeaponHitTargetEffect
#include "fable_compiler.h"
#include <math.h>
// NF49 (round 7): the def classes come from the shared headers (were local CDefPointeeBase / CDefPointer / CSubDefInfo /
// CSpecialEffectsDef models and a CDefClassBaseMethods / CDefinitionManagerMethods adapter whose same-TU GetSubDef body bound
// ?GetSubDef@CDefClassBaseMethods@@..): the call at 0x00669580 is the member template
// ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z (Ego_r.pdb / FableWin.pdb),
// its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom; the stand-in
// replicated that body). The local CCharString model is replaced by fable_string.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCPhysicsBase.h"
#pragma intrinsic(sqrt)
class C3DVector {public:float X,Y,Z;C3DVector(){} FABLE_FORCEINLINE C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){} C3DVector(float a,float b,float c):X(a),Y(b),Z(c){} FABLE_FORCEINLINE C3DVector& operator=(const C3DVector&r){X=r.X;Y=r.Y;Z=r.Z;return *this;} FABLE_FORCEINLINE C3DVector&operator-=(const C3DVector&r){X-=r.X;Y-=r.Y;Z-=r.Z;return *this;} C3DVector& AccurateNormalise(); FABLE_FORCEINLINE float DotProduct(const C3DVector&other)const{return Z*other.Z+Y*other.Y+X*other.X;} FABLE_FORCEINLINE C3DVector operator*(float k)const{return C3DVector(X*k,Y*k,Z*k);} FABLE_FORCEINLINE C3DVector operator-(const C3DVector&r)const{return C3DVector(X-r.X,Y-r.Y,Z-r.Z);} };class CRightHandedSet {public:C3DVector Up,Forward;FABLE_FORCEINLINE const C3DVector&GetUp()const{return Up;}};
class CTCPhysicsBaseVirtualSlots {public:virtual void V0()=0;virtual void V1()=0;virtual void V2()=0;virtual void V3()=0;virtual void V4()=0;virtual void V5()=0;virtual void V6()=0;virtual void V7()=0;virtual void V8()=0;virtual void V9()=0;virtual void V10()=0;virtual void V11()=0;virtual void V12()=0;virtual void V13()=0;virtual void V14()=0;virtual void V15()=0;virtual void V16()=0;virtual void V17()=0;virtual void V18()=0;virtual void V19()=0;virtual void V20()=0;virtual void V21()=0;virtual void V22()=0;virtual void V23()=0;virtual void V24()=0;virtual void V25()=0;virtual void V26()=0;virtual void V27()=0;virtual void V28()=0;virtual void V29()=0;virtual void V30()=0;virtual void V31()=0;virtual void V32()=0;virtual void V33()=0;virtual void V34()=0;virtual void V35()=0;virtual void V36()=0;virtual void V37()=0;virtual void V38()=0;virtual void V39()=0;virtual void V40()=0;virtual void V41()=0;virtual void V42()=0;virtual void V43()=0;virtual void V44()=0;virtual void V45()=0;virtual void V46()=0;virtual void V47()=0;virtual void V48()=0;virtual void V49()=0;virtual void V50()=0;virtual void V51()=0;virtual void V52()=0;virtual void V53()=0;virtual void V54()=0;virtual void V55()=0;virtual void V56()=0;virtual void V57()=0;virtual void V58()=0;virtual void V59()=0;virtual void V60()=0;virtual void V61()=0;virtual void V62()=0;virtual void V63()=0;virtual void V64()=0;virtual void V65()=0;virtual void V66()=0;virtual void V67()=0;virtual void V68()=0;virtual void V69()=0;virtual void V70()=0;virtual void V71()=0;virtual const CRightHandedSet&GetRHSet()const=0;};

class CThing;class CTCDParticleEmitter {public:static CThing* __fastcall Create(long,const C3DVector&,bool);void SetEmitterPosition(const C3DVector&);void SetEmitterOrientation(const CRightHandedSet&);};class CThing;

// Layout: active CDefClassBase and CTCPhysicsBase headers own storage. Methods/virtual-slots adapters add no fields; CThing and CSpecialEffectsDef headers are quarantined.
// Layout: exact oracle physics+60 resource+70 dead+91; effect flags+28 component+5c.
// Layout: physics vtable0125d2c4 slot72 is006b1290, returning the static RH set0137f9b4 (Up={0,0,1},Forward={0,1,0}); PDB identifies GetRHSet() const and GetUp() const.
class CThing {public:char pad[0x20];unsigned UsingComponents[9];char mapAndLists[24];CTCDParticleEmitter*component;::CTCPhysicsBase*physics;char gap[12];::CDefClassBase*resource;char gap2[0x1d];unsigned char dead;const C3DVector&GetPos() const; /* 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb) */};

void __fastcall CreateWeaponHitTargetEffect(const CThing*thing,const C3DVector&position,const C3DVector&direction,bool flourish,bool knockout,bool counter){if(!thing||(thing->dead&1))return;CDefPointer<const CSpecialEffectsDef> def;long id=-1;const char*key=flourish?"LATERAL_SLASH_FLOURISH":knockout?"LATERAL_SLASH_KNOCKOUT":counter?"LATERAL_SLASH_COUNTER_BLOCK":"LATERAL_SLASH_NORMAL";{CCharString name(key,-1);bool acquired=thing->resource->GetSubDef(&def);if(acquired&&def->GetEffectFromString(name,id)){CThing*effect=CTCDParticleEmitter::Create(id,thing->GetPos(),false);if(effect){CTCDParticleEmitter*c=0;if(effect->UsingComponents[2]&0x800)c=effect->component;c->SetEmitterPosition(position);C3DVector up;up=((CTCPhysicsBaseVirtualSlots*)thing->physics)->GetRHSet().GetUp();up.AccurateNormalise();C3DVector forward=direction;float dot=forward.DotProduct(up);forward-=up*dot;forward.AccurateNormalise();CRightHandedSet b;b.Up=up;b.Forward=forward;c->SetEmitterOrientation(b);}}}}