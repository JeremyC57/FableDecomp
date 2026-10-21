// Grade: DIFFER(514v511) -- register allocation only: retail puts this in ebp and effectType/effect/emitter in edi, freeing ebx to hold the CIntelligentPointer vtable (0x01238C6C) for both temporaries; the lane gives this edi, effectType ebp and uses immediates
// CModeScreamerAggressive::OnActivate() -- spawn the screamer's aggressive special effect: look the effect type up in the creature's special-effects def, kill any live previous effect, create the new one at the creature and attach its particle emitter, mark the combat component ready for melee and run the stand activation
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA410, 0x1FF bytes (padded to 512), `ret`.
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::Kill 0x004C9B80, CThing::GetPosition 0x004C73D0,
//   CreateEffectThing 0x006E0880 (__fastcall), CVectorMap::LowerBound 0x0040F020 (inline lookups 0x4B and 0x30),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CModeStandBase::OnActivate 0x008621A0; def release slot 1.
// Layout: the landed CModeScreamerAggressive::OnDeactivate 0x008DA690 model (PEffect +0x74, PTarget +0x7c; combat
//   ReadyForMelee +0x80; thing life flags +0x91); stand-in GetSubDef / GetEffectFromString bodies as the landed
//   StrikeResponseScorpionKing::Initialise 0x008C4B80.
// Shape: `GetSubDef(effects) && effects->GetEffectFromString("...", effectType);` as a discarded expression statement (the
//   name is a conditional temporary: flag in bl, destroyed under `test bl,1`).
#ifndef _MSC_VER
#define __int64 long long
#define __fastcall
#define __forceinline inline
#endif
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/CModeStandBase.h"
#include "engine/CTCCombat.h"
#include "engine/CTCWeapon.h"
#include "fable_string.h"
// Generated Thing offsets differ after bitset/vector shrink. Native presence bits20..43, map44..4F, CurrentMap8E,life91.
struct ThingNative;
struct ComponentEntry{int Type;void* Component;};
struct ComponentMap{ComponentEntry*First,*Last,*Limit;ComponentEntry*LowerBound(const int*);}; //40F020
struct ThingNative {
 void Kill(bool); //4C9B80: native kill flags/deferred destruction
 void* VTable;unsigned char BaseData[0x1c];unsigned int UsingComponents[9];ComponentMap Components;
 unsigned char Unrecovered50[0x3c];unsigned short DefGlobalIndex;short CurrentMapNumber;signed char PlayerNumber;unsigned char LifeFlags;
 __forceinline void* GetInterface(int type){int key=type;ComponentMap*map=&Components;ComponentEntry*p=map->LowerBound(&key);if(p==map->Last||type<p->Type)p=map->Last;return p->Component;}
 template<class T>__forceinline bool GetInterface(T*&out,int type) {if(UsingComponents[type/32]&(1u<<(type%32))){out=(T*)GetInterface(type);return true;}return false;}
};

struct WeaponMethods:CTCWeapon{long GetWeaponClass()const;}; //5DA7F0, def+2C (PDB Class), not propagated CCategory label
struct CarryingMethods{ThingNative*GetThingInPrimarySlot();}; //6A1440: walks carry list and chooses primary entity
bool __fastcall IsThingCarryingMeleeWeapon(const ThingNative*); //6A15A0, presence70 +liveprimary +weapon72
struct NativeStringValue:CCharString {
 NativeStringValue():CCharString(){} //99E4B0
 NativeStringValue(const NativeStringValue&); //99EC30, actual header lacks narrow copy declaration
 bool NotEqual(const CCharString&)const; //99E900, audited compares storage/length/content and inverts
};
struct AnimationGroupFilter {void*VTable;NativeStringValue Group;unsigned long CRC;void SetAndHash(const CCharString&);}; //73DFF0, 99EFB0 assignment+4014A0 CRC
struct CountedAnimEntry {CAnimEntryLite*Data;CCPPointerInfo*Info;};
// Native typed projection of actual flattened CModeStandBase: inherited ParentThing and embedded string/filter remain typed for host fixtures.
struct ModeStandNative {
 void*VTable;ThingNative*ParentThing;AnimationGroupFilter WeaponBasedFilter;long FramesBeforeRemovingSelf;bool Active;unsigned char ActivePadding[3];
 long IdleCounter,FramesToKeepUpdatingMovement;bool ForceAddNewAnims;unsigned char ForcePadding[3];float MaxRunSpeed,MaxJogSpeed,MaxWalkSpeed;NativeStringValue PoseAnimName;
 CountedAnimEntry Stand,SlowWalk,Walk,Jog,Run,Sprint;
};
FABLE_STATIC_ASSERT(sizeof(ModeStandNative)==sizeof(CModeStandBase));

struct IntelligentInfo {ThingNative*Object;unsigned long RefCount;}; //CBaseObjectPointer native object0/count4; differs from counted-block ABI
struct BaseIntelligentPointer {
 IntelligentInfo*PData;
 BaseIntelligentPointer(); //A01B10, native baseVT129C95C
 virtual ~BaseIntelligentPointer(); //A01C10
 ThingNative*GetPItem()const; //A01B50
 void SetPItem(ThingNative*); //A01B90
};
struct IntelligentThingPointer:BaseIntelligentPointer {
 IntelligentThingPointer(ThingNative*p=0):BaseIntelligentPointer(){SetPItem(p);}
 //implicit virtual destructor: VT1238C6C scalar-deleting4A9710 directly calls baseA01C10
 IntelligentThingPointer&operator=(const IntelligentThingPointer&p){SetPItem(p.GetPItem());return*this;}
};
FABLE_STATIC_ASSERT(sizeof(IntelligentThingPointer)==8);
struct C3DVector { float X, Y, Z; };
struct CNameString : CCharString {             // the retail call sites convert a literal implicitly (CCharString(const char*, long = -1))
    CNameString(const char* text) : CCharString(text, -1) { }
};
struct CDefVirtual {
    virtual ~CDefVirtual();                     // slot 0
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_refCount == 0)
            m_p->Destroy();
    }
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CNameString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CNameString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct ThingWithDef : ThingNative {
    CDefClassBase* GetDef() const { return *(CDefClassBase* const*)((const unsigned char*)this + 0x70); }   // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CTCDParticleEmitter {
public:
    void AttachToThing(ThingNative* thing, long attachType, const CNameString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
ThingNative* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CombatNative {unsigned char Prefix[0x80];bool ReadyForMelee;};
struct ModeScreamer:ModeStandNative {
 AnimationGroupFilter Filter;IntelligentThingPointer PEffect,PTarget;
 void OnActivate();void StandActivate(); //8621A0
};
FABLE_STATIC_ASSERT(offsetof(ModeScreamer,PEffect)==0x74);
void ModeScreamer::OnActivate()
{
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    static_cast<ThingWithDef*>(ParentThing)->GetDef()->GetSubDef(effects)
        && effects.m_p->GetEffectFromString("SCREAMER_AGGRESSIVE", effectType);
    if (PEffect.GetPItem() && !(PEffect.GetPItem()->LifeFlags & 1)) {
        PEffect.GetPItem()->Kill(true);
        PEffect = IntelligentThingPointer(0);
    }
    if (effectType > 0) {
        PEffect = IntelligentThingPointer(CreateEffectThing(effectType, &static_cast<ThingWithDef*>(ParentThing)->GetPosition(), false));
        if (PEffect.GetPItem() && !(PEffect.GetPItem()->LifeFlags & 1)) {
            CTCDParticleEmitter* emitter = 0;
            PEffect.GetPItem()->GetInterface(emitter, 0x4b);
            emitter->AttachToThing(ParentThing, 12, "EMITTER", 0, 0.0f);
        }
    }
    CombatNative* combat;
    ParentThing->GetInterface(combat, 0x30);
    combat->ReadyForMelee = true;
    StandActivate();
}
