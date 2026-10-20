// CTCAtmosPlayer::OnSerialise(CPersistContext& context, bool) -- persist the atmos def by name: saving writes the def-name handle of AtmosDefIndex, loading reads the name and looks its global index up again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E36A0, length 192 bytes, `ret 8`.
// Callees: CDefinitionManager::Get @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0
// (CDefString by hidden pointer), CPersistContext::Transfer<long>(const char*, long&, const long&) @ 0x007E39D0,
// CCharString() @ 0x0099E4B0, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
// CPersistContext::Transfer(const char*, CCharString&, const CCharString&) @ 0x004109A0, CDefinitionManager::
// GetDefGlobalIndexFromName @ 0x009AD410; the name literal "AtmosName" @ 0x01270520.
// Layout = Ego_r.pdb CTCAtmosPlayer (20 bytes): CTCBase, PSoundEmitter +0x0C, AtmosDefIndex +0x10; CPersistContext mode +0x18.
// Shape: both arms home a local in the dead `context` slot (the save default -1, the load name); the load default is an
// inline by-value empty string (built, copied into the argument temporary, destroyed: no NRVO); the def-manager singleton
// is fetched after the call arguments are pushed.
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
inline CCharString EmptyString() { CCharString empty; return empty; }
struct CDefString {                                     // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_tablePos;
    CDefString();
};
struct CDefinitionManager {
    static CDefinitionManager& __fastcall Get();        // 0x0044C6B0
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                          // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue);                  // 0x007E39D0
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);    // 0x004109A0
};
class CTCAtmosPlayer {
public:
    virtual void v00();
    void*  ParentThing;                                 // +0x04
    bool   ToBeRemoved;                                 // +0x08
    bool   IsInGlobalUpdateTCs;                         // +0x09
    void*  PSoundEmitter;                               // +0x0c CASoundEmitter*
    long   AtmosDefIndex;                               // +0x10
    void OnSerialise(CPersistContext& context, bool);
};

void CTCAtmosPlayer::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        CDefString name = CDefinitionManager::Get().GetDefNameFromGlobalIndex(AtmosDefIndex);
        context.Transfer("AtmosName", name.m_tablePos, -1);
    } else {
        CCharString name;
        context.Transfer("AtmosName", name, EmptyString());
        AtmosDefIndex = CDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
}
