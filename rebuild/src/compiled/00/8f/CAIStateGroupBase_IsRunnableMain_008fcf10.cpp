// CAIStateGroupBase::IsRunnableMain() -- true when the state group is enabled and its virtual IsRunnable() agrees
// Leaf: ?IsRunnableMain@
// Retail 0x008FCF10, 0x16 bytes (+ int3 padding). Manifest label "Script_GetCurrentStateGroupPriority" (_global) is a
// propagated descriptive name: the body reads CAIStateGroupBase::Enabled (+0x12, engine/CAIStateGroupBase.h) and calls
// vtable slot 11 (+0x2C), which rtti-vtables.tsv gives as CAIStateGroupBase::IsRunnable (0x008FD282); the PDB name set
// (Ego_r / FableWin) holds `?IsRunnableMain@CAIStateGroupBase@@QAE_NXZ`, the public wrapper of the protected virtual.
// Callers: 0x0088B891 and 0x0088BB1D (the brain's state-group scheduler).
// Shape: two byte tests with one shared `xor al,al` tail and an explicit `mov al,1` exit = `bool` result written as
// `if (a && b) return true; return false;`. A local model (not engine/CAIStateGroupBase.h) because the header holds the
// vptr as a data member and the body needs the real virtual slot.
class CAIBrain;

class CAIStateGroupBase { public:
    CAIBrain*     PBrain;                   // +0x04
    unsigned char ScriptAIPriority[4];      // +0x08 EScriptAIPriority
    void*         Name_PStringData;         // +0x0c CCharString
    bool          Terminated;               // +0x10
    unsigned char Priority;                 // +0x11
    bool          Enabled;                  // +0x12
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();              // slot 11 (+0x2c), 0x008FD282
    bool IsRunnableMain();
};

bool CAIStateGroupBase::IsRunnableMain()
{
    if (Enabled && IsRunnable())
        return true;
    return false;
}