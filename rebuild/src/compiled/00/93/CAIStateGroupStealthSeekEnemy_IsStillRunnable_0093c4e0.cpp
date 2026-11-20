// CAIStateGroup_StealthSeekEnemy::IsStillRunnable(CTCPerceiveThing*) -- require perceived live attackable enemy and willingness to attack
// Naming (NF173, 2026-11-20): q4718/q5091: 0x007446A0 is Ego_r.pdb / FableWin.pdb ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ, taken from engine/CTCPerceiveThing.h (FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETPERCEIVEDTHING); the CTCPerceiveThingMethods declaration is dropped.
// NF167 (2026-11-20, q4949 / q4988): callee 0x0076D2F0 respelt from the free stand-in AreThingsEnemies(CThing*, CThing*) to the PDB static
//   ?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z (Ego_r 0x0077B430 / FableWin 0x02606310) through the shared engine/CTCEnemy.h (the call
//   dereferences both thing pointers; same fastcall registers).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail0x0093C4E0,275bytes. GetPItemA01B50;IsStopping8FD010;GetPerceivedThing7446A0;
// LowerBound4365B0;IsThingAttackable7F6170;AreThingsEnemies76D2F0;IsWillingToAttackTarget8648A0.
#define FABLE_RETAIL_CTCPERCEIVETHING_FLAT_GETPERCEIVEDTHING   // NF173 q4718: the out-of-line 0x007446A0 getter
#include "fable_compiler.h"
#include "engine/CAIStateGroup_StealthSeekEnemy.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPerceiveThing.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "stlport/fable_containers.h"   // std::pair (STLport 4.6 <utility>)
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: typedef std::pair<ETCInterfaceType, CTCBase*> Entry; Entry *first,*last; const Entry* LowerBound(const ETCInterfaceType& key) const; };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead;
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out,long type) {
        if(bits[type>>5]&(1UL<<(type&31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type); CInterfaceMap* map=&interfaces; const CInterfaceMap::Entry* it=map->LowerBound(key);
            if(it==map->last || it->first>type) it=map->last;
            *out=(T*)((void*)it->second); return true;
        } return false;
    }
};


class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CTCPerceiveThingMethods : CTCPerceiveThing { };   // NF173: GetPerceivedThing is the header's (FLAT_GETPERCEIVEDTHING)
class CTCTargetingAI { public: bool IsThingAttackable(const CThing*) const; };
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#include "engine/CTCEnemy.h"   // CTCEnemy::AreThingsEnemies 0x0076D2F0 (NF167 opt-in)
bool __fastcall IsWillingToAttackTarget(CThing*,CThing*);
struct CAIStateGroup_StealthSeekEnemy_Adapter : CAIStateGroup_StealthSeekEnemy {
 CAIBrain* Brain() { return *(CAIBrain**)&_pad_0x04[0]; }
 CBaseIntelligentPointer& Enemy() { return *(CBaseIntelligentPointer*)PEnemy; }
 FABLE_FORCEINLINE bool AreEnemies() { CThing* c=(CThing*)Brain()->Creature; return CTCEnemy::AreThingsEnemies(*((CThing*)Enemy().GetPItem()),*c); }
 FABLE_FORCEINLINE bool WillAttack() { CThing* c=(CThing*)Brain()->Creature; return IsWillingToAttackTarget(c,((CThing*)Enemy().GetPItem())); }
 bool IsStopping(); bool IsStillRunnable(CTCPerceiveThingMethods*);
};
bool CAIStateGroup_StealthSeekEnemy_Adapter::IsStillRunnable(CTCPerceiveThingMethods* perception) {
 if(Enemy().GetPItem() && !(((CThing*)Enemy().GetPItem())->dead&1) && !IsStopping()) {
  if(perception->ExtendedLineOfSight || perception->AwarenessTimer!=0.0f) {
   CThing* enemy=((CThing*)Enemy().GetPItem());
   if(perception->GetPerceivedThing()==enemy) {
    CTCTargetingAI* targeting;
    if(((CThing*)Brain()->Creature)->GetInterface(&targeting,8) && targeting->IsThingAttackable(((CThing*)Enemy().GetPItem()))) {
     return AreEnemies() && WillAttack();
    }
   }
  }
 }
 return false;
}