#ifndef FABLE_ENGINE_CTCTARGETINGSPIRIT_H
#define FABLE_ENGINE_CTCTARGETINGSPIRIT_H
// CTCTargetingSpirit (new header, repair of row 0x007F9960): RTTI single base CTCTargetingBase at +0 (engine/CTCTargetingBase.h,
// 56 bytes). Ego_r.pdb members (struct_layouts_egor.tsv, PDB 88 bytes with the 16-byte debug STLport vector):
// MaxTargetingDistance float +0x38, Targets vector<CIntelligentPointer<CThing> > +0x3C, TargetedPos C3DVector (PDB +0x4C).
// Retail (12-byte STLport vector): the ctor 0x004DEBD4 zeroes +0x3C/+0x40/+0x44 (Targets), Alloc 0x004E1006 news 0x54 bytes,
// so TargetedPos sits at +0x48 and sizeof is 0x54. OnInitialActivate 0x007F98E0 stores MaxTargetingDistance at +0x38.
// Name set: ?UpdateTargetList@CTCTargetingSpirit@@IAEXXZ (protected, 0x007F9960), FrameUpdate 0x007F9B90,
// OnInitialActivate 0x007F98E0 (both virtual overrides of CTCBase/CTCTargetingBase slots).
#include "engine/CTCTargetingBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
class CThing;
class CTCTargetingSpirit : public CTCTargetingBase {
public:
    virtual void OnInitialActivate();                                    // 0x007F98E0
    virtual void FrameUpdate();                                          // 0x007F9B90
    float MaxTargetingDistance;                                          // +0x38
    std::vector<CIntelligentPointer<CThing> > Targets;                   // +0x3C
    C3DVector TargetedPos;                                               // +0x48
protected:
    void UpdateTargetList();                                             // 0x007F9960
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingSpirit) == 0x54);
#endif
