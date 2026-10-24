// CTCAtmosPlayer::SetAtmosDefIndex(long index) -- store the atmos def index; for a non-zero index create the sound emitter at the parent thing's position (when the sound system exists) and start it playing the def's name
// Leaf: ?SetAtmosDefIndex@CTCAtmosPlayer@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3610, 122 bytes (`ret 4` + int3 padding). FableWin.pdb
//   `?SetAtmosDefIndex@CTCAtmosPlayer@@QAEXJ@Z` (public thiscall); caller CTCAtmosPlayer vfunc_4 @ 0x007E3690
//   (`SetAtmosDefIndex(AtmosDefIndex)` when non-zero) and 0x006B23AD. Catalog label `Def_GetDetailsForDisplay` was
//   an autoname guess.
// Layout: CTCAtmosPlayer (Ego_r.pdb, 0x14 bytes): CTCBase {vptr, ParentThing +4, flags +8}, PSoundEmitter +0x0c
//   (CASoundEmitter*), AtmosDefIndex +0x10.
// Callees: the sound system singleton 0x013B8394 slot 4 (+0x10, creates an emitter at a position), CThing::GetPosition
//   @ 0x004C73D0, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDefNameFromGlobalIndex
//   @ 0x009ACCC0 (CDefString by hidden pointer), CDefStringTable::GetString @ 0x009D49B0 on g_DefStringTable 0x013CA828,
//   the emitter's slot 5 (+0x14, play by name), ~CCharString @ 0x0099EAE0. Module built without /EHsc (no frame).
class CCharString {
public:
    void* m_pData;
    ~CCharString();                                             // 0x0099EAE0
};

struct C3DVector { float x, y, z; };

struct CDefString {
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
};

class CDefinitionManager {
public:
    CDefString GetDefNameFromGlobalIndex(long index);           // 0x009ACCC0
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();           // 0x0044C6B0
};

class CDefStringTable {
public:
    CCharString GetString(long index);                          // 0x009D49B0 (the CDefString's index)
};
extern CDefStringTable g_DefStringTable;                        // 0x013CA828

class CASoundEmitter {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Play(const CCharString& name);                 // slot 5 (+0x14)
};

class CASoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CASoundEmitter* CreateEmitter(const C3DVector& pos);   // slot 4 (+0x10)
};
extern CASoundSystem* g_pSoundSystem;                           // 0x013B8394

class CThing {
public:
    const C3DVector& GetPosition() const;                       // 0x004C73D0
};

class CTCAtmosPlayer {
public:
    virtual void v00();
    CThing*         ParentThing;                                // +0x04
    unsigned char   _pad08[4];                                  // CTCBase flags
    CASoundEmitter* PSoundEmitter;                              // +0x0c
    long            AtmosDefIndex;                              // +0x10
    void SetAtmosDefIndex(long index);
};

void CTCAtmosPlayer::SetAtmosDefIndex(long index)
{
    AtmosDefIndex = index;
    if (index) {
        if (g_pSoundSystem)
            PSoundEmitter = g_pSoundSystem->CreateEmitter(ParentThing->GetPosition());
        if (PSoundEmitter) {
            CCharString name = g_DefStringTable.GetString(CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(AtmosDefIndex).m_index);
            PSoundEmitter->Play(name);
        }
    }
}