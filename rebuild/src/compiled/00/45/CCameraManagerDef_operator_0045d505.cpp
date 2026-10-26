// CCameraManagerDef::operator=(const CCameraManagerDef&) -- memberwise copy: CParentDefClassBase part, the two camera-list vectors, three modes and five floats; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D505, length 93 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886 (twice).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CCameraManagerDef's vtable 0x01233BE4 is the 5-byte `jmp 0x0045D505` right before this body, and
// the PDB layout of CCameraManagerDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x40.. -> retail +0x3c..; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
// float members copy as plain dword moves (no x87) in a memberwise assignment.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally as a packed polymorphic base
// whose operator= is out of line; the derived members follow at their natural alignment.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

class CCameraManagerDef : public CParentDefClassBase { public: // vtable 0x01233BE4
    CLongVector          CameraList;                  // +0x3c
    CLongVector          SpecialityCameraList;        // +0x48
    long                 InitialMode;                 // +0x54
    long                 ZTargetMode;                 // +0x58
    long                 ProjectileWeaponMode;        // +0x5c
    float                BaseHeightToKeepAboveFlatLandscape; // +0x60
    float                BaseHeightToKeepAboveWater;  // +0x64
    float                MaxHeightRelativeToGround;   // +0x68
    float                MaxHeightStepForPointAcceptance; // +0x6c
    float                AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat; // +0x70
    CCameraManagerDef& operator=(const CCameraManagerDef& other);
};

CCameraManagerDef& CCameraManagerDef::operator=(const CCameraManagerDef& other)
{
    CParentDefClassBase::operator=(other);
    CameraList = other.CameraList;
    SpecialityCameraList = other.SpecialityCameraList;
    InitialMode = other.InitialMode;
    ZTargetMode = other.ZTargetMode;
    ProjectileWeaponMode = other.ProjectileWeaponMode;
    BaseHeightToKeepAboveFlatLandscape = other.BaseHeightToKeepAboveFlatLandscape;
    BaseHeightToKeepAboveWater = other.BaseHeightToKeepAboveWater;
    MaxHeightRelativeToGround = other.MaxHeightRelativeToGround;
    MaxHeightStepForPointAcceptance = other.MaxHeightStepForPointAcceptance;
    AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat = other.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat;
    return *this;
}