// CAIStateGroupBase::IsStopping() -- true when the owning brain is stopping or this state group has terminated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD010, length 26 bytes. No callees.
// Layout: PBrain at +0x04 and Terminated at +0x10 come from engine/CAIStateGroupBase.h; the
// brain's byte at +0x05 is the brain's own stopping flag (inside the PDB header's unnamed
// +0x04 dword). Result shape `xor eax,eax` / `mov eax,1` is a full-register `||` (long), not
// the byte-only form a `bool` return gives.
class CAIBrain { public:
    void*         __vftable;                // +0x00
    unsigned char _pad[1];                  // +0x04
    bool          m_bStopping;              // +0x05
};

class CAIStateGroupBase { public:
    void*         __vftable;                // +0x00
    CAIBrain*     PBrain;                   // +0x04
    unsigned char ScriptAIPriority[4];      // +0x08
    void*         Name_PStringData;         // +0x0c
    bool          Terminated;               // +0x10
    unsigned char Priority;                 // +0x11
    bool          Enabled;                  // +0x12
    long IsStopping() const;
};

long CAIStateGroupBase::IsStopping() const
{
    return PBrain->m_bStopping || Terminated;
}