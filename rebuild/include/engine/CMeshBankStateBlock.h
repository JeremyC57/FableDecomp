#ifndef FABLE_ENGINE_CMESHBANKSTATEBLOCK_H
#define FABLE_ENGINE_CMESHBANKSTATEBLOCK_H
#ifdef FABLE_RETAIL_CMESHBANKSTATEBLOCK
#include "engine/CBankStateBlock.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_map.h"

class C3DBoundingBox;

// Both donor type records contain the base, four scalar controls and these nested types.
// Retail uses two 12-byte STLport trees and allocates 0x34 bytes in SetupEditorAnimation.
class CMeshBankStateBlock : public CBankStateBlock {
public:
    struct CStateBlock {
        bool EnableLOD, PreserveMaterials, GenericAnimation, UnpackedPositions;
        long MaxLODs, ResourcePhysicsMeshIndex, AnimationVersion;
    };
    class CLODStateBlock {
    public:
        typedef std::map<CCharString, long> TBoneLODSet;
        TBoneLODSet BoneLODs;
        CCountedPointer<C3DBoundingBox> MinXYZRange;
    };
    class CAnimationStateBlock {
    public:
        typedef std::set<CCharString> TBoneSet;
        TBoneSet ExcludedBones;
    };
    CMeshBankStateBlock();                                                   // 0x00A2D650
    virtual ~CMeshBankStateBlock();                                           // 0x00A2D710
    virtual void GetDefault(CBankStateBlock&);
    virtual unsigned long CreateCRC();
    CStateBlock State;
    CLODStateBlock LODState;
    CAnimationStateBlock AnimationState;
protected:
    virtual void Set(const CBankStateBlock*);
};
FABLE_STATIC_ASSERT_AT(42, sizeof(CMeshBankStateBlock) == 0x34);
#endif
#endif
