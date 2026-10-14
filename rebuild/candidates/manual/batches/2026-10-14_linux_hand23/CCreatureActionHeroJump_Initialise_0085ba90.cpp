// CCreatureActionHeroJump::Initialise() -- set the "ST_JUMP" animation, touch the creature, then the base initialisation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085BA90, length 59 bytes. Vtable slot 12 of CCreatureActionHeroJump (0x012766A4).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal "ST_JUMP" @ 0x0127654C, length -1),
// CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8; the result is unused -- a GetCreature() call whose
// value the retail source dropped, kept because the accessor is out of line), CCombatActionBase::Initialise @ 0x00857520
// (the base virtual, called non-virtually; slot 12 as well).
// Shape: a scoped named CCharString in the one-dword frame (re-addressed with `lea` for the setter and the destructor);
// `this` in esi across the three calls; no tail jump because the frame slot must be released after the last call.
struct CThingCreatureBase;

struct CCharString {                        // retail CCharString is one pointer (fable_string.h)
    char* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;            // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12 (+0x30), 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureActionHeroJump : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureActionHeroJump::Initialise()
{
    {
        CCharString name("ST_JUMP");
        SetAnimationName(name);
    }
    PCreature.GetPItem();
    CCombatActionBase::Initialise();
}
