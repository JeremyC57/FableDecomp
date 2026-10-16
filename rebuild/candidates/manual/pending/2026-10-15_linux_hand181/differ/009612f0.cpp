// CAIStateGroup_BanditKingCharge::IsRunnable() -- runnable when the live hero is farther than the charge radius (squared XY distance > 49.0f @ 0x01295F5C): target the hero
// Leaf: IsRunnable@CAIStateGroup_BanditKingCharge
// Grade: DIFFER(174v174) -- register swap only: hero kept in esi with edi pushed late (retail hero in edi, esi pushed late for the creature position)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009612F0, length 174 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::GetHeroThing @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0 (creature, then hero),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10.
// Shape: `dist2 > R` gives `fcomp ; test ah,41h ; jne`; the inline distance helper keeps both differences on the FPU;
// PTargetThing = CIntelligentPointer<..>(hero) (inline ctor, vtable 0x01231C4C).
struct C3DVector { float X, Y, Z; };

struct CThing
{
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;           // 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated)
};

struct CPlayer
{
    CThing* GetHeroThing();                         // 0x00487DC0
};

struct CPlayerManager
{
    CPlayer* GetMainPlayer();                       // 0x00449970
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer    // CIntelligentPointer<CThingCreatureBase>, vtable 0x01231C4C
{
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

// squared XY distance between two positions (Y term evaluated first by VC7.1; arguments evaluate right to left,
// so `b` -- the creature -- is fetched first and kept in esi)
__forceinline float GetDistanceSquaredXY(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}

struct CAIStateGroup_BanditKingCharge
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14
    CPlayerManager* GetPPlayerManager();            // 0x008FCF90
    virtual bool IsRunnable();
};

bool CAIStateGroup_BanditKingCharge::IsRunnable()
{
    CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero && !(hero->m_flags91 & 1)
        && GetDistanceSquaredXY(hero->GetPosition(), PBrain->Creature->GetPosition()) > 49.0f)
    {
        PTargetThing.SetPItem(CIntelligentPointerThing(hero).GetPItem());
        return true;
    }
    return false;
}
