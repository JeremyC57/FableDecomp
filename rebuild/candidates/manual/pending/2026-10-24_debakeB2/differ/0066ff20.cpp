// Grade: DIFFER(175v175) -- unchanged best (debakeB), register allocation only (eax vs ecx/edx in three spots). debakeB2 tried 6 more, all the identical diff: direct m_pPhysics, a const position, a hero local, a parent local for the last copy, an assigned (default-constructed) position, a memberwise copy from a const reference.
// CTCCoopSpirit::SwapToHero() -- hand the co-op spirit's player back to the hero: swap, restore the spirit def, move the hero to the spirit
// Retail: Fable.exe @ 0x0066FF20, 175 bytes, `ret`. Callees: CTCBase::GetPlayerManager (0x00686D40, fetched afresh for
// every call), CPlayerManager accessors: the main player number (0x00449700, `mov eax,[ecx+1Ch]`), SwapToHero(long)
// (0x00449CB0), GetSpiritDefName(long) (0x00449730, a CCharString by value; dtor 0x0099EAE0), the player def setter
// (0x00449B20: GetPlayer(n) then 0x0048A070 with the name), GetPlayer(long) (0x004498C0); CPlayer::GetHeroCreature
// (0x00487DC0); CThing::GetPosition (0x004C73D0, const C3DVector&); the physics component's SetPosition (virtual
// slot 31, +0x7C, on thing+0x60). The spirit's position is copied before the swap (C3DVector with a memberwise user
// copy ctor: three dword moves into the frame).
// Layout: CTCBase::ParentThing +4 (Ego_r.pdb CTCCoopSpirit); CThing::PlayerNumber is the signed char at retail +0x90
// (PDB +0xA0); the parent thing's dwords +0xB0 / +0xB4 (past CThing) are unnamed in the PDB view, descriptive names.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                         // 0x0099EAE0
};
struct CPhysicsComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void SetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
struct CThing {
    unsigned char      _pad00[0x60];
    CPhysicsComponent* m_pPhysics;                          // +0x60
    unsigned char      _pad64[0x2c];
    signed char        PlayerNumber;                        // +0x90
    unsigned char      _pad91[0x1f];
    long               m_SpiritLinkState;                   // +0xb0 (descriptive)
    long               m_PreviousSpiritLinkState;           // +0xb4 (descriptive)
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    CPhysicsComponent* GetPhysics() const { return m_pPhysics; }
};
struct CPlayer {
    CThing* GetHeroCreature();                              // 0x00487DC0
};
struct CPlayerManager {
    long GetMainPlayerNumber() const;                       // 0x00449700
    void SwapToHero(long playerNumber);                     // 0x00449CB0
    CCharString GetSpiritDefName(long playerNumber);        // 0x00449730
    void SetPlayerDefName(long playerNumber, const CCharString& defName);   // 0x00449B20
    CPlayer* GetPlayer(long playerNumber);                  // 0x004498C0
};
struct CTCCoopSpirit {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    CPlayerManager* GetPlayerManager() const;               // 0x00686D40 (CTCBase)
    void SwapToHero();
};
void CTCCoopSpirit::SwapToHero()
{
    long mainPlayer = GetPlayerManager()->GetMainPlayerNumber();
    C3DVector position = ParentThing->GetPosition();
    GetPlayerManager()->SwapToHero(ParentThing->PlayerNumber);
    GetPlayerManager()->SetPlayerDefName(mainPlayer, GetPlayerManager()->GetSpiritDefName(mainPlayer));
    GetPlayerManager()->GetPlayer(mainPlayer)->GetHeroCreature()->GetPhysics()->SetPosition(position);
    ParentThing->m_PreviousSpiritLinkState = ParentThing->m_SpiritLinkState;
}
