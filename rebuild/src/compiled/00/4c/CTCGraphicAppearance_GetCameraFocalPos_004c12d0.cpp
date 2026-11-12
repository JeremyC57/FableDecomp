// CTCGraphicAppearance::GetCameraFocalPos(float time) const -- the camera-focal helper point, else the focal helper point (flags 4), else GetFocalPos(time)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C12D0, length 116 bytes, `ret 8` (hidden result + float).
// FableWin.pdb: ?GetCameraFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector&, float) const @ 0x004C0560
// (helper-name globals 0x013BAC4C then 0x013BAC48, flags 4), CTCGraphicAppearance::GetFocalPos(float) const @ 0x004C1060
// (the hidden result pointer is passed straight through, so the fallback is `return GetFocalPos(time);`).
// Shape: the found arm returns the local through C3DVector's user memberwise copy ctor (`fld/fstp` X, integer Y/Z); each
// lookup has its own `return pos;` (VC cross-jumps the two copies into one block) -- a `found` flag or `||` gives the
// second out-pointer lea in ecx and swaps the Y/Z copy registers.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAC4C CAMERA_FOCAL_POSITION_NAME (global, const; initializer 0x0121E6E0), was g_helperCameraFocal_013bac4c.
//   0x013BAC48 MAIN_FOCAL_POSITION_NAME (global, const; initializer 0x0121E6C0), was g_helperFocal_013bac48.
#include "engine/C3DVector.h"
class CCharString { public: void* PStringData; };
class CThing;
extern const CCharString CAMERA_FOCAL_POSITION_NAME;   // 0x013BAC4C
extern const CCharString MAIN_FOCAL_POSITION_NAME;         // 0x013BAC48

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& out, float time) const;   // 0x004C0560
    C3DVector GetFocalPos(float time) const;                // 0x004C1060
    C3DVector GetCameraFocalPos(float time) const;
};

C3DVector CTCGraphicAppearance::GetCameraFocalPos(float time) const
{
    C3DVector pos;
    if (GetHelperPoint(CAMERA_FOCAL_POSITION_NAME, 0, 4, pos, time))
        return pos;
    if (GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 4, pos, time))
        return pos;
    return GetFocalPos(time);
}
}