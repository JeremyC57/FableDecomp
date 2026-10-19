// CCreatureAction_TeleportIn::Initialise() -- base Initialise, then (CreateEffects) look the TELEPORT_IN effect up in the owner's CSpecialEffectsDef and create that effect thing at the owner's position
// Grade: DIFFER(205v205) -- register swap only: retail turns this into &PCreature in place (add esi,8) and keeps the sub-def holder in edi; ours lea edi,[esi+8] with the holder in esi (epilogue pops move accordingly)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B9F20, length 205 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CalcAndLookup(const CCharString&, long*) @ 0x00669D30 (on the special-effects def), CCharString::~CCharString @ 0x0099EAE0,
// CThing::GetPosition @ 0x004C73D0, Create(long defIndex, const C3DVector&, long) @ 0x006E0880 (__fastcall).
// Shape: `found = GetSubDef(&out) && def->CalcAndLookup(CCharString("TELEPORT_IN"), &index)` -- the string is a
// conditional temporary of the `&&`, so VC keeps a construction-flag word (zeroed at entry, set to 1 after the string is
// built, tested before its destructor); the sub-def is copied out of the out slot into a holder (edi) right after
// GetSubDef and released at the end (`dec [edi+4] ; jne`, null-tested); the owner's def (+0x70) is used raw.
// Layout: owner CIntelligentPointer +8; Ego_r.pdb CreateEffects +0x12c, retail +0x114 (CCombatActionBase 0x114).
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

class CCreatureAction_TeleportIn : public CCombatActionBase {
public:
    bool CreateEffects;                                // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_TeleportIn::Initialise()
{
    CCombatActionBase::Initialise();
    if (CreateEffects) {
        CDefPointer<CSpecialEffectsDef> out;
        long effectIndex = 0;
        CDefClassBase* thingDef = PCreature.GetPItem()->PDef;
        bool haveDef = thingDef->GetSubDef(&out);
        CDefHolder<CSpecialEffectsDef> effects(out.Object);
        bool found = haveDef && effects->CalcAndLookup("TELEPORT_IN", &effectIndex);
        if (found)
            Create(effectIndex, PCreature.GetPItem()->GetPosition(), 0);
    }
}
