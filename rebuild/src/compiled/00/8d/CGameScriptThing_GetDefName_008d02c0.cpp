// CGameScriptThing::GetDefName() const -- the definition name of the scripted thing (looked up in the global def-string table), or the empty string when the thing is gone or dead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D02C0, length 131 bytes (padded to 144).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls), CThing::GetDefName @ 0x004C7CC0 (CDefString by hidden pointer),
//          CDefStringTable::GetString @ 0x009D49B0 on the global table @ 0x013CA828 (CDefString by value, CCharString by hidden pointer),
//          CCharString copy constructor @ 0x0099EC30, CCharString::~CCharString @ 0x0099EAE0, CCharString(const char*, long) @ 0x0099EBF0
//          with the empty literal @ 0x0122D70E.
// Shape: two 4-byte frame temporaries (the CDefString and the looked-up name); the name is a NAMED local returned by value
// (VC7.1 has no NRVO: copy constructor into the hidden pointer, then the local's destructor).
// Leaf: ?GetDefName@CGameScriptThing@@UBE?AVCCharString@@XZ
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")
class CDefString { public:                                // a definition-string handle: one table index
    long m_index;
};
struct CDefStringTable {
    CCharString GetString(CDefString name);        // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828
struct CThing_Retail {
    unsigned char  _pad00[0x91];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    CDefString GetDefName() const;                 // 0x004C7CC0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<CThing>: vptr + item (8 bytes)
    void*          m_pVtbl;
    void*          m_pItem;
    CThing_Retail* GetPItem() const;               // 0x00A01B50
};
class CGameScriptThing { public:
    virtual void v00();
    unsigned char           _pad04[8];
    CBaseIntelligentPointer PThing;                // +0x0c CIntelligentPointer<CThing>
    virtual CCharString GetDefName() const;
};

CCharString CGameScriptThing::GetDefName() const
{
    if (PThing.GetPItem() && !PThing.GetPItem()->m_dead) {
        CCharString name = g_DefStringTable.GetString(PThing.GetPItem()->GetDefName());
        return name;
    }
    return CCharString(g_emptyString_0122d70e, -1);
}