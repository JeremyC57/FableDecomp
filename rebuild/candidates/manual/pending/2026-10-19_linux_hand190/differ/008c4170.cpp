// CCreatureAction_ScorpionKingDeath::Initialise() -- animation FALL_TO_DEATH, base Initialise, then look the "DEATH" effect up in the owner's CSpecialEffectsDef and create that effect thing at the owner's position
// Grade: DIFFER(218v218) -- frame 0xc v 8: retail shares the first block-scoped string, the GetSubDef out slot and the DEATH string in one word (RTM sibling-slot sharing); body otherwise identical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4170, length 218 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580,
// CCRC-style CalcAndLookup(const CCharString&, long*) @ 0x00669D30 (on the special-effects def),
// CThing::GetPosition @ 0x004C73D0, Create(long defIndex, const C3DVector&, long) @ 0x006E0880 (__fastcall).
// Shape: the owner's def (+0x70 of the thing behind the CIntelligentPointer at +8) is copied into a by-value holder
// (null-tested add-ref, null-tested release through slot 1) only around the GetSubDef call; the out CDefPointer and
// the looked-up index are zeroed in the frame first; the sub-def is copied out of the out slot into a holder kept in
// esi (its null test merges with the `if`, its release is the trailing `dec [esi+4] ; jne`), so the dead out slot is
// reused by the block-scoped "DEATH" string; the bool result survives the string's destructor in bl. Create's third
// argument (0) is pushed before the owner's position is fetched.
struct C3DVector { float X, Y, Z; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CBaseObject;

struct CBaseIntelligentPointer {
    void*        m_vtable;
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                     // 0x00A01B50
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

template <class T> struct CDefCopy {                   // by-value CDefPointer copy: add-ref on entry, release on exit
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefCopy()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};

struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                               // +0x70
    const C3DVector& GetPosition() const;              // 0x004C73D0
};

CThing* __fastcall Create(long defIndex, const C3DVector& position, long flags);   // 0x006E0880

struct CCombatActionBase {                             // retail sizeof 0x114
    virtual void v00();
    long                    _pad04;
    CBaseIntelligentPointer POwner;                    // +0x08
    unsigned char           _body[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);    // 0x008574D0
    void Initialise();                                 // 0x00857520
    CThing* GetOwner() const { return (CThing*)POwner.GetPItem(); }
    __forceinline CSpecialEffectsDef* GetSpecialEffectsDef() const
    {
        CDefPointer<CSpecialEffectsDef> out;
        CDefCopy<CDefClassBase> def(GetOwner()->PDef);
        def->GetSubDef(&out);
        return out.Object;
    }
};

struct CCreatureAction_ScorpionKingDeath : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_ScorpionKingDeath::Initialise()
{
    SetAnimationName("FALL_TO_DEATH");
    CCombatActionBase::Initialise();
    CDefPointer<CSpecialEffectsDef> out;
    long effectIndex = 0;
    {
        CDefCopy<CDefClassBase> def(GetOwner()->PDef);
        def->GetSubDef(&out);
    }
    CDefHolder<CSpecialEffectsDef> effects(out.Object);
    if (effects.Object) {
        bool found;
        {
            CCharString name("DEATH", -1);
            found = effects->CalcAndLookup(name, &effectIndex);
        }
        if (found)
            Create(effectIndex, GetOwner()->GetPosition(), 0);
    }
}
