// CGroupMemberData::CGroupMemberData(const CThingAICreature*, bool isActive, unsigned long joinedFrame, unsigned long deactivatedFrame) -- constructor: CIntelligentPointer PMember at +0 built from the pointer, then IsActive +8, JoinedFrame +0xc, DeactivatedFrame +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812890, length 53 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// The manifest label (CActiveDialogueInfo) is propagated; the layout written (intelligent pointer
// at +0, bool at +8, two ulongs at +0xc/+0x10, 0x14 bytes, ret 0x10) is the PDB's CGroupMemberData
// {PMember, IsActive, JoinedFrame, DeactivatedFrame}. The vtable stored at +0 (0x01264BAC) is
// CIntelligentPointer's own, so the pointer is a MEMBER at +0 and the class itself is not
// polymorphic. CIntelligentPointer(p) is the inlined family-44 shape: base ctor, vtable, SetPItem(p).
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                                          // +0x04 (the pointer is 8 bytes)
    CBaseIntelligentPointer();                                   // 0x00A01B10
    void SetPItem(CBaseObject* item);                            // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {           // vtable 0x01264BAC
    virtual void v00();
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

class CGroupMemberData { public:
    CIntelligentPointer PMember;                                 // +0x00 CIntelligentPointer<const CThingAICreature>
    bool                IsActive;                                // +0x08
    unsigned long       JoinedFrame;                             // +0x0c
    unsigned long       DeactivatedFrame;                        // +0x10
    CGroupMemberData(CBaseObject* member, bool isActive, unsigned long joinedFrame, unsigned long deactivatedFrame);
};

CGroupMemberData::CGroupMemberData(CBaseObject* member, bool isActive, unsigned long joinedFrame, unsigned long deactivatedFrame)
    : PMember(member)
    , IsActive(isActive)
    , JoinedFrame(joinedFrame)
    , DeactivatedFrame(deactivatedFrame)
{
}