// CEngineInternalPrimitiveBase::$E_CurrentInterpolationInfo() -- dynamic initializer of the static CEngineInternalPrimitiveBase::CurrentInterpolationInfo: run CEngineInterpolationInfo's default constructor on it (no atexit: the class has a trivial destructor)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x01224900, 10 bytes, in the `.text$yc` initializer run, reached from the .CRT$XCU table
//   (data reference 0x013748E4): `mov ecx,0x0143B8E0 ; jmp 0x0042FA30` (the constructor's `return this` is the tail call).
// Identity (two sources): Ego_r.pdb (0x0101A290) and FableWin.pdb (0x04009040) both list `dynamic initializer for
//   'CEngineInternalPrimitiveBase::CurrentInterpolationInfo''; the object is a 112-byte CEngineInterpolationInfo (Ego_r.pdb:
//   six 12-byte sets, WFServerCurrentTime, InterpolationSetList[3]) built by ??0CEngineInterpolationInfo@@QAE@XZ 0x0042FA30
//   (landed), and the 0x70 bytes at 0x0143B8E0 are what CEnginePrimitiveRenderer::FrameStart 0x00B32660 copies the frame's
//   CEngineComponentParams::InterpolationInfo into each frame (0x00B31D10 is the out-of-line copy). The landed rows bind the global
//   as ?g_EngineInterpolationInfo@@3VCEngineInterpolationInfo@@A / ?g_block_0143b8e0@@3USBlock112@@A (NAME_FIXES.tsv).
// Label: the manifest's "sub_01224900" is a gap-scan placeholder.
// Header: engine/CEngineInternalPrimitiveBase.h with FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS and the new nested opt-in
//   FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO (the static member; hdr/README.md). CEngineInterpolationInfo
//   has no shared header (engine/CIEngine.h forward-declares it): the PDB layout below, its constructor out of line as in retail.
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO
#include "engine/CEngineInternalPrimitiveBase.h"

class CEngineInterpolationInfoSet {                 // Ego_r.pdb, 12 bytes
public:
    float GTPredictedRenderAt;                      // +0x00
    float GTPredictedTimeSinceLastRenderFrame;      // +0x04
    float WFInterpolate;                            // +0x08
};

class CEngineInterpolationInfo {                    // Ego_r.pdb, 112 bytes
public:
    CEngineInterpolationInfoSet Current;            // +0x00
    CEngineInterpolationInfoSet Last;               // +0x0c
    CEngineInterpolationInfoSet PausedCurrent;      // +0x18
    CEngineInterpolationInfoSet PausedLast;         // +0x24
    CEngineInterpolationInfoSet BulletTimeCurrent;  // +0x30
    CEngineInterpolationInfoSet BulletTimeLast;     // +0x3c
    long WFServerCurrentTime;                       // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];   // +0x4c
    CEngineInterpolationInfo();                     // 0x0042FA30
};

CEngineInterpolationInfo CEngineInternalPrimitiveBase::CurrentInterpolationInfo;