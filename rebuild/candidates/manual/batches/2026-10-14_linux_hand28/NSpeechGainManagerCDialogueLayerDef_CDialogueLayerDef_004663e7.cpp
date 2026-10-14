#pragma optimize("s", on)
// NSpeechGainManager::CDialogueLayerDef::CDialogueLayerDef(const CDialogueLayerDef&) -- copy constructor: CDefClassBase copy, own vtable, six float parameters copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004663E7, length 63 bytes. Size-optimised TU (odd address in the /Os region).
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C. Vtable 0x0123543C = NSpeechGainManager::CDialogueLayerDef
// (rtti-vtables.tsv).
// PDB CDialogueLayerDef (64 bytes): ResponseTimeSecsAttack +0x28 .. FocusShiftDistanceThreshold +0x3c after the 0x25-byte
// CDefClassBase (padded to 0x28). The floats are copied with plain dword moves, so they are modelled as dwords.
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 .. +0x28: CResource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase(const CDefClassBase& other);         // 0x0046644C
};

namespace NSpeechGainManager {

struct CDialogueLayerDef : CDefClassBase {             // vtable 0x0123543C
    virtual void v00();
    long ResponseTimeSecsAttack;                       // +0x28 (float)
    long ResponseTimeSecsRelease;                      // +0x2c (float)
    long GainLevelHigh;                                // +0x30 (float)
    long GainLevelMid;                                 // +0x34 (float)
    long GainLevelLow;                                 // +0x38 (float)
    long FocusShiftDistanceThreshold;                  // +0x3c (float)
    CDialogueLayerDef(const CDialogueLayerDef& other);
};

CDialogueLayerDef::CDialogueLayerDef(const CDialogueLayerDef& other)
    : CDefClassBase(other),
      ResponseTimeSecsAttack(other.ResponseTimeSecsAttack), ResponseTimeSecsRelease(other.ResponseTimeSecsRelease),
      GainLevelHigh(other.GainLevelHigh), GainLevelMid(other.GainLevelMid), GainLevelLow(other.GainLevelLow),
      FocusShiftDistanceThreshold(other.FocusShiftDistanceThreshold)
{
}

}
