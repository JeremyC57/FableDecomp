#ifndef FABLE_ENGINE_CGETBESTTURNINGANIM_H
#define FABLE_ENGINE_CGETBESTTURNINGANIM_H
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CAnimationEntry;
// Complete original anonymous type0x409e0, both PDBs; donor025af460 GetResult.
namespace {class CGetBestTurningAnim {
public:
 CGetBestTurningAnim(const C3DVector& facing,const C3DVector& target);
 void operator()(const std::pair<float,const CAnimationEntry*>& value);
 void GetResult(const CAnimationEntry** panim_entry,float& fraction)const {fraction=RotationAngle/BestMatchAngle;*panim_entry=PBestMatchAnim;}
private:float RotationAngle,BestMatchAngle;const CAnimationEntry* PBestMatchAnim;
};}
#endif
