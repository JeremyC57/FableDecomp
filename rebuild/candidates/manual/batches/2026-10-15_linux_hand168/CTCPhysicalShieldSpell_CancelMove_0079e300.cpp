// CTCPhysicalShieldSpell::CancelMove() -- play the def's cancel sound (player-flagged for a player owner), play the "END" effect on the owner, stop being pausable and remove the shield component (0x93) from the owner
// Leaf: CancelMove
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079E300, 155 bytes.
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable 0x013CA828, CTCPhysicalShieldSpell::PlaySound
//   (const CCharString&, bool) @ 0x0079DE00 (landed under the propagated label "CTCBulletTime::PlaySoundW"), CCharString::~CCharString
//   @ 0x0099EAE0, CCharString::CCharString(const char*, long) @ 0x0099EBF0, CTCPhysicalShieldSpell::PlayEffect @ 0x0079DEB0,
//   CThing::RemoveTC(long) @ 0x004C9840 (catalog "CBaseObject::OnPropertyChanged").
// Layout (PDB CTCPhysicalShieldSpell, as the landed ResumeForCutscene 0x0079E520): ParentThing +0x04, PDef +0x0c, Pausable +0x44;
//   CSpecialAbilitiesPhysicalShieldDef cancel sound (CDefString id) at +0x60; CThing +0x10 type byte (2 = player).
// Shape: the owner is a named local read first (edi); the sound string is a named local in its own block; the two effect strings
//   ("" @ 0x0122D70E and "END" @ 0x01239B74) are named locals in a block, built before the argument pushes and destroyed in
//   reverse order before the Pausable store.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char Type;                         // +0x10 (2 = player)
    void RemoveTC(long type);                   // 0x004C9840
};
struct CSpecialAbilitiesPhysicalShieldDef {
    unsigned char _pad00[0x60];
    long CancelSound;                           // +0x60 CDefString id
};
struct CTCPhysicalShieldSpell {
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x4];
    const CSpecialAbilitiesPhysicalShieldDef* PDef;   // +0x0c CDefPointer
    unsigned char _pad10[0x34];
    bool    Pausable;                           // +0x44
    unsigned long PlaySound(const CCharString& sound, bool player);   // 0x0079DE00
    void PlayEffect(long effect, const CCharString& boneName, const CCharString& type, CThing* target, long flags);   // 0x0079DEB0
    void CancelMove();
};

void CTCPhysicalShieldSpell::CancelMove()
{
    CThing* owner = ParentThing;
    {
        CCharString sound = g_DefStringTable.GetString(PDef->CancelSound);
        PlaySound(sound, owner->Type == 2);
    }
    {
        CCharString type("", -1);
        CCharString boneName("END", -1);
        PlayEffect(0, boneName, type, owner, 8);
    }
    Pausable = false;
    owner->RemoveTC(0x93);
}
