// behaviour test for 0x00520C80 CThingManager::OnThingKill (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
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
class CCharStringData { public:
    const char*   m_pChars;                     // +0x00
    unsigned long m_length;                     // +0x04
};
class CCharString { public:
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
class CThingManager { public:
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


// ---- stubs ----
static bool g_gameState, g_level; static int g_gsCalls, g_lvCalls;
bool CThing::IsGameStatePersisted() const { g_gsCalls++; return g_gameState; }
bool CThing::IsLevelPersistent() const { g_lvCalls++; return g_level; }
static unsigned long g_calcSeed; static const void* g_calcData; static unsigned long g_calcLen; static int g_calcCalls;
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length)
{
    g_calcSeed = seed; g_calcData = data; g_calcLen = length; g_calcCalls++;
    return 0xC0FFEE00UL + length;
}
static int g_adds; static unsigned __int64 g_addUid; static bool g_addGs; static unsigned long g_addCrc; static CEntityRuntimePersistence* g_addThis;
void CEntityRuntimePersistence::AddPersistedDeadEntity(unsigned __int64 uid, bool gameState, unsigned long sectionCRC)
{
    g_adds++; g_addUid = uid; g_addGs = gameState; g_addCrc = sectionCRC; g_addThis = this;
}

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

static double g_thingBuf[0x20], g_mgrBuf[0x20];

int main()
{
    CThing* thing = (CThing*)g_thingBuf;
    CThingManager* mgr = (CThingManager*)g_mgrBuf;
    thing->UID = 0x1122334455667788ULL;
    static CCharStringData data; data.m_pChars = "Level_Section"; data.m_length = 13;
    thing->SectionName.m_pStringData = &data;

    // 1: not tracked -> nothing
    thing->m_flags93 = 0xfd; g_gameState = true;
    mgr->OnThingKill(thing);
    check(g_gsCalls == 0 && g_adds == 0, "not tracked");
    // 2: tracked but not persisted -> nothing
    thing->m_flags93 = 2; g_gameState = false; g_level = false;
    mgr->OnThingKill(thing);
    check(g_gsCalls == 1 && g_lvCalls == 1 && g_adds == 0, "not persisted");
    // 3: game-state persisted -> record with gameState true
    g_gameState = true;
    mgr->OnThingKill(thing);
    check(g_adds == 1 && g_addThis == &mgr->EntityRuntimePersistence && g_addUid == 0x1122334455667788ULL && g_addGs
          && g_calcSeed == 0 && g_calcData == data.m_pChars && g_calcLen == 13 && g_addCrc == 0xC0FFEE00UL + 13
          && g_gsCalls == 3 && g_lvCalls == 1, "game state");
    // 4: level persistent only -> gameState false
    g_gameState = false; g_level = true;
    mgr->OnThingKill(thing);
    check(g_adds == 2 && !g_addGs && g_lvCalls == 2, "level");
    // 5: empty section name -> "" and length 0
    thing->SectionName.m_pStringData = 0;
    mgr->OnThingKill(thing);
    check(g_adds == 3 && g_calcLen == 0 && g_calcData != 0 && ((const char*)g_calcData)[0] == 0 && g_addCrc == 0xC0FFEE00UL, "empty name");
    if (!fails) printf("00520c80_TEST PASS\n");
    return fails;
}