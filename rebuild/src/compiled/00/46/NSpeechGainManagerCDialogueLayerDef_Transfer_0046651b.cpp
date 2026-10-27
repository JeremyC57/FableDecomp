// NSpeechGainManager::CDialogueLayerDef::Transfer(CPersistContext& context) -- persist every member without member names
// Leaf: ?Transfer@CDialogueLayerDef@NSpeechGainManager@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046651B, 79 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): slot 18 of vtable 0x0123543C (RTTI .?AVCDialogueLayerDef@NSpeechGainManager@@; Ego_r.pdb ?Transfer@CDialogueLayerDef@NSpeechGainManager@@UAEXAAVCPersistContext@@@Z);
//   the 6 Transfer calls take, in call order, the members at +0x28, +0x2c, +0x30, +0x34, +0x38, +0x3c with the per-type callees 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, 0x00431061, which are
//   Ego_r.pdb NSpeechGainManager::CDialogueLayerDef's members ResponseTimeSecsAttack, ResponseTimeSecsRelease, GainLevelHigh, GainLevelMid, GainLevelLow, FocusShiftDistanceThreshold.
// Shape (family M050, representative 0x0046651B): `context.Transfer(Member);` per member; `optimize("s", on)` (each member
//   address formed with `lea eax,[esi+N]`, the last as `add esi,N`).
// Header debt: the def base is modelled by its retail size (pack(1), vptr + members) with the virtual Transfer only.
#pragma optimize("s", on)
class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};
#pragma pack(push, 1)
class CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x25 - 4];
};
#pragma pack(pop)
namespace NSpeechGainManager {
class CDialogueLayerDef : public CDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);
    float                    ResponseTimeSecsAttack;        // +0x28
    float                    ResponseTimeSecsRelease;       // +0x2c
    float                    GainLevelHigh;                 // +0x30
    float                    GainLevelMid;                  // +0x34
    float                    GainLevelLow;                  // +0x38
    float                    FocusShiftDistanceThreshold;   // +0x3c
};
void CDialogueLayerDef::Transfer(CPersistContext& context)
{
    context.Transfer(ResponseTimeSecsAttack);
    context.Transfer(ResponseTimeSecsRelease);
    context.Transfer(GainLevelHigh);
    context.Transfer(GainLevelMid);
    context.Transfer(GainLevelLow);
    context.Transfer(FocusShiftDistanceThreshold);
}
}