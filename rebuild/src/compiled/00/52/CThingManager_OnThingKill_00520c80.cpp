// CThingManager::OnThingKill(CThing* thing) -- when a persistence-tracked thing dies, records it (UID, persistence kind and a CRC of its section name) in the entity runtime persistence
// Grade: RELOCATION_MATCH -- the section name is bound to a `const CCharString& name` local and both inline accessors run on it, so VC7.1 keeps the string-data pointer from the length test in eax and re-tests it for the text (`test eax,eax ; jne ; mov eax,offset empty ; jmp ; mov eax,[eax]`); the text accessor is the landed `PStringData == 0 ? "" : ...` ternary
// Leaf: OnThingKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00520C80, length 111 bytes (`ret 4`, + int3 padding).
// Callees: CThing::IsGameStatePersisted @ 0x004C8280, CThing::IsLevelPersistent @ 0x004C82A0, CRC Calc(ulong seed, const void* data,
// ulong length) @ 0x004014A0 (__fastcall), CEntityRuntimePersistence::AddPersistedDeadEntity(unsigned __int64 uid, bool gameState,
// ulong sectionCRC) @ 0x00520330.
// Globals: the CCharString empty text @ 0x0129AAF4.
// Layout (retail, local model): CThing UID (64-bit) +0x18, SectionName (CCharString) +0x58, flag byte +0x93 (bit 1 = persistence
// tracked); CThingManager EntityRuntimePersistence at retail +0xa0 (PDB +0xbc).
// Shape: named locals in retail load order: the section-name length and text (inline CCharString GetLength() / GetString()),
// then the UID, then the CRC call; the second IsGameStatePersisted call is the bool argument.
struct CCharStringData {
    const char*   m_pChars;                     // +0x00
    unsigned long m_length;                     // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { if (!m_pStringData) return 0; return m_pStringData->m_length; }
    const char* GetString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pChars; }   // "" = 0x0129AAF4
};
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
class CThing {
public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char    _pad20[0x58 - 0x20];
    CCharString      SectionName;               // +0x58
    unsigned char    _pad5c[0x93 - 0x5c];
    unsigned char    m_flags93;                 // +0x93
    bool IsGameStatePersisted() const;          // 0x004C8280
    bool IsLevelPersistent() const;             // 0x004C82A0
};
struct CEntityRuntimePersistence {
    void AddPersistedDeadEntity(unsigned __int64 uid, bool gameState, unsigned long sectionCRC);   // 0x00520330
};
struct CThingManager {
    unsigned char             _pad00[0xa0];
    CEntityRuntimePersistence EntityRuntimePersistence;   // +0xa0
    void OnThingKill(CThing* thing);
};

void CThingManager::OnThingKill(CThing* thing)
{
    if (thing->m_flags93 & 2) {
        if (thing->IsGameStatePersisted() || thing->IsLevelPersistent()) {
            const CCharString& name = thing->SectionName;
            unsigned long length = name.GetLength();
            const char* text = name.GetString();
            unsigned __int64 uid = thing->UID;
            unsigned long crc = Calc(0, text, length);
            EntityRuntimePersistence.AddPersistedDeadEntity(uid, thing->IsGameStatePersisted(), crc);
        }
    }
}