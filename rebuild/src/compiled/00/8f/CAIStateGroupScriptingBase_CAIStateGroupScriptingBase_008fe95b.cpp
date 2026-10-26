// CAIStateGroup_ScriptingBase::CAIStateGroup_ScriptingBase() -- the CAIStateGroupBase constructor (0x008FD255), the word at +0x14 cleared (before the vtable store in this size-optimised module), this class's vtable (0x0128B044)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FE95B, length 22 bytes.
// The chunk label "CInterestingToVillagersDef::CInterestingToVillagersDef" is propagated; the vtable names the class. Shape: `and dword ptr [esi+14h],0`, `mov eax,esi`.
#pragma optimize("s", on)
class CAIStateGroupBase { public:
    virtual void v00();                            // (a non-virtual destructor keeps the deleting destructor out of the TU)
    unsigned char _pad[0x10];
    CAIStateGroupBase();                           // 0x008FD255
};
struct CAIStateGroup_ScriptingBase : CAIStateGroupBase {                    // vtable 0x0128B044
    long m_14;                                     // +0x14
    CAIStateGroup_ScriptingBase();
};

CAIStateGroup_ScriptingBase::CAIStateGroup_ScriptingBase()
    : CAIStateGroupBase(), m_14(0)
{
}