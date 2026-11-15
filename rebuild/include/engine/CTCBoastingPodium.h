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
 virtual void FrameUpdate();
private:
 long GetNumberOfActiveBoastingPositions()const;
 CVectorMap<long,CIntelligentPointer<CThing>,CKeyPairCompareLess<long,CIntelligentPointer<CThing> > > BoastingPositions;
 long LastPositionsWithBoastCount;
 unsigned long FrameBoastListFrom;
 bool BoastingActive;
 CDefPointer<const CBoastingPodiumDef> PDef;
};
FABLE_STATIC_ASSERT(sizeof(CTCBoastingPodium)==0x2c);
#endif
