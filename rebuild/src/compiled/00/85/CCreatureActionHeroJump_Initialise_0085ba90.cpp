// CCreatureActionHeroJump::Initialise() -- set the "ST_JUMP" animation, touch the creature, then the base initialisation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085BA90, length 59 bytes. Vtable slot 12 of CCreatureActionHeroJump (0x012766A4).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal "ST_JUMP" @ 0x0127654C, length -1),
// CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8; the result is unused -- a GetCreature() call whose
// value the retail source dropped, kept because the accessor is out of line), CCombatActionBase::Initialise @ 0x00857520
// (the base virtual, called non-virtually; slot 12 as well).
// Shape: a scoped named CCharString in the one-dword frame (re-addressed with `lea` for the setter and the destructor);
// `this` in esi across the three calls; no tail jump because the frame slot must be released after the last call.
class CThingCreatureBase;

class CCharString { public:                        // retail CCharString is one pointer (fable_string.h)
    char* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12 (+0x30), 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureActionHeroJump : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureActionHeroJump::Initialise()
{
    {
        CCharString name("ST_JUMP");
        SetCombatAnimation(name);
    }
    PCreature.GetPItem();
    CCombatActionBase::Initialise();
}