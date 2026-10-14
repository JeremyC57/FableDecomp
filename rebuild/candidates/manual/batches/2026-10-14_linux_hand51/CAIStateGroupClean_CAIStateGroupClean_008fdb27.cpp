// CAIStateGroup_Clean::CAIStateGroup_Clean() -- the CAIStateGroupBase constructor (0x008FD255), the word at +0x18 cleared (before the vtable store in this size-optimised module), this class's vtable (0x01289A4C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FDB27, length 22 bytes.
// The chunk label "CInterestingToVillagersDef::CInterestingToVillagersDef" is propagated; the vtable names the class. Shape: `and dword ptr [esi+18h],0`, `mov eax,esi`.
#pragma optimize("s", on)
struct CAIStateGroupBase {
    virtual void v00();                            // (a non-virtual destructor keeps the deleting destructor out of the TU)
    unsigned char _pad[0x14];
    CAIStateGroupBase();                           // 0x008FD255
};
struct CAIStateGroup_Clean : CAIStateGroupBase {                    // vtable 0x01289A4C
    long m_18;                                     // +0x18
    CAIStateGroup_Clean();
};

CAIStateGroup_Clean::CAIStateGroup_Clean()
    : CAIStateGroupBase(), m_18(0)
{
}
