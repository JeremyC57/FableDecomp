// CCreatureAction_UnblockableMidAttack::Initialise() -- animation STRIKE_MEDIUM_FRONT_UNBLOCKABLE, base Initialise, then turn the attacker's physics to face a live target (other than itself)
// Grade: DIFFER(178v178) -- register swap only: retail keeps &PTarget in ebx and the owner/physics in edi, ours the reverse (instruction order now identical)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BF1C0, length 178 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0, AngleBetweenPositions(const C3DVector&, const C3DVector&) @ 0x00661540
// (`__fastcall`, float result), CTCPhysicsBase::SetFacingAngleXY (virtual slot 66, +0x108).
// Shape: the target is tested present / not dead (thing byte +0x91 bit 0) and different from the owner; the physics component
// (the thing's cached pointer at +0x60) and its vtable are fetched before the float argument (VC evaluates the object of a
// virtual call first); the angle comes from an inline CThing member (the owner fetched as its `this` before the target's
// position) and is pushed through `push ecx ; fstp [esp]`; the self test compares the target with the owner.
// Layout: owner CIntelligentPointer +8, target +0xa8.
struct C3DVector { float X, Y, Z; };

struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void SetFacingAngleXY(float angle); // slot 66 (+0x108)
};

struct CThing {
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* m_pPhysics;                 // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                  // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline float GetAngleXYTo(const struct CBaseIntelligentPointer& other) const;
};

float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00661540



struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};

__forceinline float CThing::GetAngleXYTo(const CBaseIntelligentPointer& other) const   // inline: `this` is fetched before the other thing
{
    return AngleBetweenPositions(GetPosition(), other.GetPItem()->GetPosition());
}

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_UnblockableMidAttack : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCreatureAction_UnblockableMidAttack::Initialise()
{
    {
        CCharString name("STRIKE_MEDIUM_FRONT_UNBLOCKABLE", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
        CThing* owner = PCreature.GetPItem();
        if (PTarget.GetPItem() != owner)
            PCreature.GetPItem()->m_pPhysics->SetFacingAngleXY(PCreature.GetPItem()->GetAngleXYTo(PTarget));
    }
}
