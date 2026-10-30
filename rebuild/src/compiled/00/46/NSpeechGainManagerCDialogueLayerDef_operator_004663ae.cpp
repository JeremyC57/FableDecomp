#pragma optimize("s", on)
// NSpeechGainManager::CDialogueLayerDef::operator=(const CDialogueLayerDef&) -- CDefClassBase assignment, then the six float parameters copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004663AE, length 57 bytes. Size-optimised TU (odd address in the /Os region).
// Callees: CDefClassBase::operator=(const CDefClassBase&) @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is
// propagated across the byte-identical base assignment bodies).
// PDB CDialogueLayerDef (64 bytes): ResponseTimeSecsAttack +0x28, ResponseTimeSecsRelease +0x2c, GainLevelHigh +0x30,
// GainLevelMid +0x34, GainLevelLow +0x38, FocusShiftDistanceThreshold +0x3c after the 0x25-byte CDefClassBase (padded
// to 0x28). The floats are copied with plain dword moves, so they are modelled as dwords; the member-wise copy is
// written out because the DDK out-lines the implicit assignment of a record whose base has its own operator=.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

namespace NSpeechGainManager {

// def vtable 0x0123543C: slot 0 0x0046642A (implicit inline dtor); real overrides from the def vtable scan
class CDialogueLayerDef : public CDefClassBase { public:             // vtable 0x0123543C
    virtual void v00();
    long ResponseTimeSecsAttack;                       // +0x28 (float)
    long ResponseTimeSecsRelease;                      // +0x2c (float)
    long GainLevelHigh;                                // +0x30 (float)
    long GainLevelMid;                                 // +0x34 (float)
    long GainLevelLow;                                 // +0x38 (float)
    long FocusShiftDistanceThreshold;                  // +0x3c (float)
    CDialogueLayerDef& operator=(const CDialogueLayerDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x0046651B
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x0046656A
    virtual unsigned long GetSizeofClass();                  // slot 20 0x00466426
};

CDialogueLayerDef& CDialogueLayerDef::operator=(const CDialogueLayerDef& other)
{
    CDefClassBase::operator=(other);
    ResponseTimeSecsAttack = other.ResponseTimeSecsAttack;
    ResponseTimeSecsRelease = other.ResponseTimeSecsRelease;
    GainLevelHigh = other.GainLevelHigh;
    GainLevelMid = other.GainLevelMid;
    GainLevelLow = other.GainLevelLow;
    FocusShiftDistanceThreshold = other.FocusShiftDistanceThreshold;
    return *this;
}

}