// CCombatSequenceGeneric::GetName() -- the def's name looked up in the def string table, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0700, length 69 bytes (+ int3 padding).
// Callees: CCombatSequenceDef vslot 10 (`call [eax+28h]`, a 4-byte id class returned through a hidden
// pointer into the frame), CDefStringTable::GetString @ 0x009D49B0 on the global table @ 0x013CA828
// (CCharString by value into a frame temporary), CCharString::CCharString(const CCharString&) @ 0x0099EC30
// into the hidden result, CCharString::~CCharString @ 0x0099EAE0 on the temporary.
// Vtable 0x01261D74 slot 2. The 8-byte frame holds the id (high dword) and the temporary string (low
// dword); VC7.1 has no NRVO, so the named local is copied into the result and destroyed.
class CCharString { public:
    void* PStringData;                     // +0 the shared string block
    CCharString(const CCharString& other); // 0x0099EC30
    ~CCharString();                        // 0x0099EAE0
};

struct CDefStringId {                          // 4 bytes with a user constructor: returned through a hidden pointer
    long m_id;
    CDefStringId(long id) : m_id(id) { }
};

struct CDefStringTable {
    CCharString GetString(long id);            // 0x009D49B0
};

extern CDefStringTable g_DefStringTable;       // 0x013CA828

struct CCombatSequenceDef_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefStringId GetNameStringId() const;   // slot 10
};

struct CCombatSequenceGeneric_Retail {
    virtual ~CCombatSequenceGeneric_Retail();
    unsigned char _pad[0x24];                  // +0x04 CCombatSequenceBase data (retail 0x28 bytes in all)
    const CCombatSequenceDef_Retail* Def_Object;   // +0x28 CDefPointer<CCombatSequenceDef const>::Object
    virtual CCharString GetName();
};

CCharString CCombatSequenceGeneric_Retail::GetName()
{
    CCharString name = g_DefStringTable.GetString(Def_Object->GetNameStringId().m_id);
    return name;
}