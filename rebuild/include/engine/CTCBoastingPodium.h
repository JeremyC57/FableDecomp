#ifndef FABLE_ENGINE_CTCBOASTINGPODIUM_H
#define FABLE_ENGINE_CTCBOASTINGPODIUM_H
// Both original complete PDB classes and native slot9/field accesses; STLport map size0x10.
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
class CBoastingPodiumDef;
class CTCBoastingPodium:public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCBOASTINGPODIUM_CTOR_PASS23
 // Both original PDB PUBLICs: ??0CTCBoastingPodium@@QAE@AAVCThing@@@Z.
 CTCBoastingPodium(CThing&);
#endif
#ifdef FABLE_RETAIL_BOAST_AREA_PASS27
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_BOASTING_PODIUM;}
 void OnHeroEnterBoastArea(long,bool,class CThingCreatureBase&);
 void OnHeroLeaveBoastArea(long,bool,class CThingCreatureBase&);
#endif
 virtual void FrameUpdate();
private:
 long GetNumberOfActiveBoastingPositions()const;
 CVectorMap<long,CIntelligentPointer<CThing>,CKeyPairCompareLess<long,CIntelligentPointer<CThing> > > BoastingPositions;
 long LastPositionsWithBoastCount;
 unsigned long FrameBoastListFrom;
 bool BoastingActive;
 CDefPointer<const CBoastingPodiumDef> PDef;
};
FABLE_STATIC_ASSERT_AT(20, sizeof(CTCBoastingPodium)==0x2c);
#endif
