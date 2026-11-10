// CCameraManagerDef::CCameraManagerDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CCameraManagerDef vtable, then the members' constructors
// Leaf: ??0CCameraManagerDef@@QAE@XZ
// Retail: Fable.exe @ 0x00456795. Identity: the vtable it stores, 0x01233BE4, is RTTI .?AVCCameraManagerDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CCameraManagerDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CCameraManagerDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CCameraManagerDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;

class CCameraManagerDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CCameraManagerDef();                                   // 0x00456795
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<long> CameraList;
    std::vector<long> SpecialityCameraList;
    long InitialMode;
    long ZTargetMode;
    long ProjectileWeaponMode;
    float BaseHeightToKeepAboveFlatLandscape;
    float BaseHeightToKeepAboveWater;
    float MaxHeightRelativeToGround;
    float MaxHeightStepForPointAcceptance;
    float AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat;
};

CCameraManagerDef::CCameraManagerDef()
{
}
