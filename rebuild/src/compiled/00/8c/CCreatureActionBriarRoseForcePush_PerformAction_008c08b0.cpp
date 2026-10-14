// CCreatureAction_BriarRoseForcePush::PerformAction() -- add a CTCForcePushPower component to the creature and arm it (level 1, active)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C08B0, length 69 bytes. Vtable slot 3 (PerformAction) of
// CCreatureAction_BriarRoseForcePush (0x01294CD4).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal "CTCForcePushPower" @ 0x0123C454, -1),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
// @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the component name is an implicit conversion temporary (constructed first in the one-dword frame, re-addressed
// with `lea` for the argument and the destructor: GOTCHAS wave 11), the arguments 0 and true pushed after it, the
// creature fetched last (object expression after the arguments); the returned component rides in esi across the
// temporary's destructor and its level (+0x10) and active flag (+0x24) are then set.
struct CTCParamsBase;

struct CCharString {                        // retail CCharString is one pointer (fable_string.h)
    char* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CTCBase;

struct CThing {
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};

struct CTCForcePushPower {                  // the component just added
    unsigned char _pad[0x10];
    long          m_level;                  // +0x10
    unsigned char _pad14[0x10];
    unsigned char m_active;                 // +0x24
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCreatureAction_BriarRoseForcePush {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
};

void CCreatureAction_BriarRoseForcePush::PerformAction()
{
    CTCForcePushPower* pPower = (CTCForcePushPower*)PCreature.GetPItem()->AddTC("CTCForcePushPower", true, 0);
    pPower->m_level = 1;
    pPower->m_active = 1;
}