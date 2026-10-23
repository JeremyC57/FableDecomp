// CTCSoundPlayer::PlayCollisionSound(const C3DVector&, const CDefPointer<const CMaterialDef>&, const CDefPointer<const CMaterialDef>&) -- play "SE_COLLISION;<A>;<B>", falling back to "SE_COLLISION;<A>" then "SE_COLLISION;<B>" (or the one material that is set)
// Leaf: PlayCollisionSound
// Grade: DIFFER(626v626) -- code shape identical; frame 0x1c v 0x14: retail gives the hidden CDefString returns their own frame slots (+0x10/+0x14/+0x18) where ours packs them into the dead material argument slots, so every frame offset shifts
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A9E0, 624 bytes, `ret 0Ch`.
// Callees: the thing's sound-bank getter 0x004C7A80 (catalog "CAnimateThing::GetMeshBank", as in the landed 0x0066B1D0),
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, the material def's name getter (virtual slot 10, +0x28, a
//   CDefString by hidden pointer), CDefStringTable::GetString(long) 0x009D49B0 on the global table 0x013CA828,
//   CDefString::operator CCharString 0x00415DD0, operator+(const CCharString&, const CCharString&) 0x0099F570,
//   operator+(const CCharString&, const char*) 0x0099F600, the positional sound-event player 0x0066A920 (catalog
//   "GetAIStateGroupTypeConditional"; bank, position, name, 0, 0).
// Globals: the collision-sounds switch 0x0137E974, the invalid sound handle 0x0129D25C.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

struct CDefString {
    long Index;
    CDefString(const CDefString& other) : Index(other.Index) { }
    operator CCharString() const;                           // 0x00415DD0
};
struct CDefStringTable {
    CCharString GetString(long index) const;                // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828

struct CMaterialDef {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetName() const;                     // slot 10 (+0x28)
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};

struct CASoundBank;
struct CThing {
    CASoundBank* GetSoundBank_004c7a80();                   // 0x004C7A80
};
extern bool g_bPlayCollisionSounds;                         // 0x0137E974
extern const unsigned long g_InvalidSoundHandle;            // 0x0129D25C

struct CTCSoundPlayer {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned long PlaySoundEventAtPos_0066a920(CASoundBank* bank, const C3DVector& pos, const CCharString& name,
                                               long a, long b);   // 0x0066A920
    void PlayCollisionSound(const C3DVector& pos, const CDefPointer<const CMaterialDef>& materialA,
                            const CDefPointer<const CMaterialDef>& materialB);
};

void CTCSoundPlayer::PlayCollisionSound(const C3DVector& pos, const CDefPointer<const CMaterialDef>& materialA,
                                        const CDefPointer<const CMaterialDef>& materialB)
{
    if (!g_bPlayCollisionSounds)
        return;
    CASoundBank* bank = ParentThing->GetSoundBank_004c7a80();
    CCharString prefix("SE_COLLISION;");
    if (!materialA.Object) {
        if (!materialB.Object)
            return;
        CCharString nameB = g_DefStringTable.GetString(materialB->GetName().Index);
        PlaySoundEventAtPos_0066a920(bank, pos, prefix + nameB, 0, 0);
    } else if (!materialB.Object) {
        CCharString nameA = g_DefStringTable.GetString(materialA->GetName().Index);
        PlaySoundEventAtPos_0066a920(bank, pos, prefix + nameA, 0, 0);
    } else {
        bool failed;
        {
            CCharString nameB = g_DefStringTable.GetString(materialB->GetName().Index);
            CCharString nameA = g_DefStringTable.GetString(materialA->GetName().Index);
            failed = PlaySoundEventAtPos_0066a920(bank, pos, prefix + nameA + ";" + nameB, 0, 0) == g_InvalidSoundHandle;
        }
        if (failed) {
            bool failedA;
            {
                CCharString nameA = g_DefStringTable.GetString(materialA->GetName().Index);
                failedA = PlaySoundEventAtPos_0066a920(bank, pos, prefix + nameA, 0, 0) == g_InvalidSoundHandle;
            }
            if (failedA)
                PlaySoundEventAtPos_0066a920(bank, pos, prefix + materialB->GetName(), 0, 0);
        }
    }
}
