#ifndef FABLE_ENGINE_CWOUNDMORPHS_H
#define FABLE_ENGINE_CWOUNDMORPHS_H
#include "rebuild_abi.h"
#include "engine/CVectorMap.h"
// Original enum values independently recovered from Ego_r and FableWin raw TPI.
enum EBodyLocation {
    BODY_LOCATION_NULL = 0,
    BODY_LOCATION_FACE = 1,
    BODY_LOCATION_LEFT_LEG = 2,
    BODY_LOCATION_RIGHT_LEG = 3,
    BODY_LOCATION_LEFT_ARM = 4,
    BODY_LOCATION_RIGHT_ARM = 5,
    BODY_LOCATION_TORSO = 6,
    MAX_NO_BODY_LOCATIONS = 7
};
class CMemoryDataInputStream;
class CDataOutputStream;
class CWoundMorphs {
public:
    class CEntry {
    public:
        CEntry();
        bool IsPermanent() const;
        bool operator==(const CEntry&) const;
        EBodyLocation Location;
        long ReplacingTextureIndex,ReplacingBumpMapIndex,WoundTextureIndex,WoundBumpMapIndex;
        long ScarTextureIndex,ScarBumpMapIndex,FadeToScarDuration,TotalDuration;
        float PermanentScarStrength;
    };
    void TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryOut(CDataOutputStream&) const;
#ifdef FABLE_RETAIL_CPERSIST_ORIGINAL_CLASS_TYPES_PASS22
protected:
#else
private:
#endif
    CVectorMap<EBodyLocation,CEntry> Morphs;
};
FABLE_STATIC_ASSERT_AT(35, sizeof(CWoundMorphs)==0x10);
#endif
