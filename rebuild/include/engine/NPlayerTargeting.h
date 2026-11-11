// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_NPLAYERTARGETING_H
#define FABLE_ENGINE_NPLAYERTARGETING_H
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
namespace NPlayerTargeting {
// Original PDB class, size28: intelligent pointer0, index8, vector12, frame24.
class CTargetPoint {
public:
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 CTargetPoint():Index(-1),FrameRejected(-1){}
 // Original copy constructor is compiler-generated (__that in donor module262).
 bool operator==(const CTargetPoint& other){return PThing==other.PThing && Index==other.Index;}
#else
 CTargetPoint();
 bool operator==(const CTargetPoint&);
#endif
 CIntelligentPointer<CThing> PThing;
 long Index;
 C3DVector TargetingPosition;
 long FrameRejected;
};
}
#endif
