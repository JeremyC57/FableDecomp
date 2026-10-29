// NScript::CRuntimeScriptedEntityPersist::SaveToGameStateScript(CCharString& out) -- append every persisted entity script to the game-state script text, each wrapped in START_NEW_ENTITY_SCRIPT <id> / END_NEW_ENTITY_SCRIPT lines
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7A00, length 146 bytes (+14 int3 padding).
// PDB: ?SaveToGameStateScript@CRuntimeScriptedEntityPersist@NScript@@QAEXAAVCCharString@@@Z; the class (20 bytes)
// holds PersistScripts, a CVectorMap<unsigned __int64, CPersistInfo> whose element vector begins at +0 (begin
// +0x00, end +0x04); each element is a 24-byte pair<unsigned __int64, CPersistInfo> and the persist info's
// 64-bit id sits at +0x08, its script text at +0x10.
// Callees: CCharString(unsigned __int64) @ 0x0099F930, operator+(const char*, const CCharString&) @ 0x0099F690,
// operator+(const CCharString&, const char*) @ 0x0099F600 (both __fastcall, hidden result in ecx),
// CCharString::operator+=(const CCharString&) @ 0x0099F0A0, operator+=(const char*) @ 0x0099F100,
// ~CCharString @ 0x0099EAE0. Literals: "\r\nSTART_NEW_ENTITY_SCRIPT " 0x012C162C, "\r\n" 0x012397F8,
// "END_NEW_ENTITY_SCRIPT\r\n" 0x012C1614.
// Shape: `out += "..." + CCharString(id) + "\r\n";` (the "\r\n" right operand is pushed first, the three
// temporaries die in reverse order), then the script, then the end marker; end is re-read every pass.
class CCharStringData;

class CCharString { public:
    CCharStringData* PStringData;
    explicit CCharString(unsigned __int64 value);                // 0x0099F930
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
    const CCharString& operator+=(const char* text);                   // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

namespace NScript {

struct CRuntimeScriptedEntityPersist {
    class CPersistInfo { public:
        unsigned __int64 EntityID;       // +0x00 (pair +0x08)
        CCharString      Script;         // +0x08 (pair +0x10)
        long             Flags;          // +0x0C (pair +0x14)
    };
    struct CEntry {                      // pair<unsigned __int64, CPersistInfo>
        unsigned __int64 first;          // +0x00
        CPersistInfo     second;         // +0x08
    };

    CEntry* m_begin;                     // +0x00 PersistScripts element vector
    CEntry* m_end;                       // +0x04
    CEntry* m_capacityEnd;               // +0x08
    unsigned char _rest[0x14 - 0x0C];

    void SaveToGameStateScript(CCharString& out);
};

void CRuntimeScriptedEntityPersist::SaveToGameStateScript(CCharString& out)
{
    for (CEntry* it = m_begin; it != m_end; ++it)
    {
        out += "\r\nSTART_NEW_ENTITY_SCRIPT " + CCharString(it->second.EntityID) + "\r\n";
        out += it->second.Script;
        out += "END_NEW_ENTITY_SCRIPT\r\n";
    }
}

} // namespace NScript