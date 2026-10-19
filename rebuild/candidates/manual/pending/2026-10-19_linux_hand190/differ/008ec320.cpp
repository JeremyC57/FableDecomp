// CCreatureAction_UndeadReturnToEarth::Initialise() -- look the SINK effect up in the owner's CSpecialEffectsDef and create it at the owner's position, suspend the owner's animation, then the base Initialise
// Grade: DIFFER(206v206) -- register swap only (ours 205): retail this in edi / sub-def holder in ebp, ours this in ebp / holder in edi (shifts one push); instruction order identical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC320, length 206 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CalcAndLookup(const CCharString&, long*) @ 0x00669D30 (on the
// special-effects def), CCharString::~CCharString @ 0x0099EAE0, CThing::GetPosition @ 0x004C73D0, Create(long defIndex,
// const C3DVector&, long) @ 0x006E0880 (__fastcall), CThingAnimatable::SetSuspendAnimation(bool, bool) @ 0x00833A10,
// CCombatActionBase::Initialise @ 0x00857520.
// Shape: the TeleportIn (0x008B9F20) lookup with the `&&` result kept in a frame bool across the conditional string
// temporary's destructor (its construction flag lives in ebx, the zero register), and the sub-def holder in ebp released
// after the base Initialise at the very end.
// Layout: owner CIntelligentPointer +8 (CCombatActionBase is 0x114 in retail).
struct C3DVector { float X, Y, Z; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefPointeeBase {
    virtual void Destroy();                            // slot 0
    virtual void Delete();                             // slot 1 (+0x04)
    long m_refCount;                                   // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};

struct CSpecialEffectsDef : CDefPointeeBase {
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);       // 0x00669580
};

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                               // +0x70
    const C3DVector& GetPosition() const;              // 0x004C73D0
    void SetSuspendAnimation(bool suspend, bool b);    // 0x00833A10 (CThingAnimatable)
};

CThing* __fastcall Create(long defIndex, const C3DVector& position, long flags);   // 0x006E0880

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                          // 0x00A01B50
};

class CCombatActionBase {
public:
    virtual void Initialise();                         // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _body[0x114 - 0x10];
};

class CCreatureAction_UndeadReturnToEarth : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCreatureAction_UndeadReturnToEarth::Initialise()
{
    CDefPointer<CSpecialEffectsDef> out;
    long effectIndex = 0;
    CDefClassBase* thingDef = PCreature.GetPItem()->PDef;
    bool haveDef = thingDef->GetSubDef(&out);
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    bool found = haveDef && effects->CalcAndLookup("SINK", &effectIndex);
    if (found)
        Create(effectIndex, PCreature.GetPItem()->GetPosition(), 0);
    CThing* owner = PCreature.GetPItem();
    owner->SetSuspendAnimation(true, false);
    CCombatActionBase::Initialise();
}
