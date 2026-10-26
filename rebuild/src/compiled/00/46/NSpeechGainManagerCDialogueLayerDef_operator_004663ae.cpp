#pragma optimize("s", on)
// NSpeechGainManager::CDialogueLayerDef::operator=(const CDialogueLayerDef&) -- CDefClassBase assignment, then the six float parameters copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004663AE, length 57 bytes. Size-optimised TU (odd address in the /Os region).
// Callees: CDefClassBase::operator=(const CDefClassBase&) @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is
// propagated across the byte-identical base assignment bodies).
// PDB CDialogueLayerDef (64 bytes): ResponseTimeSecsAttack +0x28, ResponseTimeSecsRelease +0x2c, GainLevelHigh +0x30,
// GainLevelMid +0x34, GainLevelLow +0x38, FocusShiftDistanceThreshold +0x3c after the 0x25-byte CDefClassBase (padded
// to 0x28). The floats are copied with plain dword moves, so they are modelled as dwords; the member-wise copy is
// written out because the DDK out-lines the implicit assignment of a record whose base has its own operator=.
class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 .. +0x28: CResource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

namespace NSpeechGainManager {

class CDialogueLayerDef : public CDefClassBase { public:             // vtable 0x0123543C
    virtual void v00();
    long ResponseTimeSecsAttack;                       // +0x28 (float)
    long ResponseTimeSecsRelease;                      // +0x2c (float)
    long GainLevelHigh;                                // +0x30 (float)
    long GainLevelMid;                                 // +0x34 (float)
    long GainLevelLow;                                 // +0x38 (float)
    long FocusShiftDistanceThreshold;                  // +0x3c (float)
    CDialogueLayerDef& operator=(const CDialogueLayerDef& other);
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