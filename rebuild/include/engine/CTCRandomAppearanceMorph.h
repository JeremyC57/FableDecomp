#ifndef FABLE_ENGINE_CTCRANDOMAPPEARANCEMORPH_H
#define FABLE_ENGINE_CTCRANDOMAPPEARANCEMORPH_H
#include "engine/CTCAppearanceMorphBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/CCountedPointer.h"
namespace NGraphicAppearance {class CPrimitive;}
class CTCSkeletalMorph;
// Both original complete records; retail arrays shrink16->12 and native fields10/18/1c/28.
class CTCRandomAppearanceMorph : public CTCAppearanceMorphBase, public NGraphicAppearance::CAGraphicAppearanceComponent {
#ifdef FABLE_RETAIL_DECAPITATE_PASS29
public:
 static CCharString FABLE_FASTCALL StatGetClassTypeName(){return CCharString("CTCRandomAppearanceMorph");}
 void UpdateDecapitationStatus();
#endif
private:
 void UpdateMorphSet();
 CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet;
 unsigned long Seed;
 std::vector<NGraphicAppearance::CPrimitive*> PrimHandles;
 CTCSkeletalMorph* PTCSkeletalMorph;
};
FABLE_STATIC_ASSERT_AT(17, sizeof(CTCRandomAppearanceMorph)==0x2c);
#endif
