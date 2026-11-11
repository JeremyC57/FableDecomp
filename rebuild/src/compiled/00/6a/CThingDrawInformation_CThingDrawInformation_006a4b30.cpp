// CThingDrawInformation::CThingDrawInformation() -- default constructor: construct DrawString, clear BarOn and StringOn
// Leaf: ??0CThingDrawInformation@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A4B30, 20 bytes, thiscall, returns this, plain `ret`.
// Identity (proven, agentFAM07): its only caller CThingPhysical::DrawPreparePrimitivesForRendering 0x006A64D0 constructs it as
//   the stack local at esp+0x38 (call at 0x006A6561), passes its address to virtual slot 34 of the thing (0x88: CThing vtable
//   0x01244E84 slot 34 = CThing::DrawGetThingDisplayInformation 0x004CD220, PDB ?DrawGetThingDisplayInformation@CThing@@UBE_N
//   PAVCThingDrawInformation@@@Z) and then tests the byte at +0x11 (StringOn) and copies the CCharString at +0 (DrawString).
//   Ego_r.pdb: ??0CThingDrawInformation@@QAE@XZ, layout DrawString +0 (CCharString), RelativeSize +4, BarPercentage +8,
//   Colour +0xC (CRGBColour, trivial default ctor), BarOn +0x10, StringOn +0x11: the two bytes this body clears.
//   Was the placeholder CUnprovenRecord_006a4b30.
// Callee: CCharString::CCharString() 0x0099E4B0 (??0CCharString@@QAE@XZ).
// Layout: no engine header for CThingDrawInformation; Ego_r.pdb member names and offsets (no containers, so they hold).
#include "fable_string.h"
#include "engine/CRGBColour.h"

class CThingDrawInformation {
public:
    CThingDrawInformation();
    ~CThingDrawInformation();
    CCharString DrawString;                               // +0x00
    float RelativeSize;                                   // +0x04
    float BarPercentage;                                  // +0x08
    CRGBColour Colour;                                    // +0x0C
    bool BarOn;                                           // +0x10
    bool StringOn;                                        // +0x11
};

CThingDrawInformation::CThingDrawInformation()
    : BarOn(false), StringOn(false)
{
}
