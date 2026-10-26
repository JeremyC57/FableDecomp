// CDefString::ToWideString() const -- look the table position up in the global def string table and widen the result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415D90, length 64 bytes.
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 on the global table @ 0x013CA828 (CCharString by value into a
// 4-byte frame temporary), CCharString::ToWideString() @ 0x0099E480 (CWideString by value into the hidden result),
// CCharString::~CCharString @ 0x0099EAE0 on the temporary.
// Shape: `push ecx` is the slot of the named CCharString local (a bare temporary would reuse GetString's returned
// pointer in eax; retail re-derives `lea ecx,[esp+8]`); the hidden result pointer is read into esi only after the
// GetString call and returned in eax after the local's destructor. PDB CDefString: long TablePos at +0.
class CWideString { public:
    void* PStringData;                     // +0 the shared wide-string block
    CWideString(const CWideString& other);
    ~CWideString();
};

class CCharString { public:
    void* PStringData;                     // +0 the shared string block
    CCharString(const CCharString& other);     // 0x0099EC30
    ~CCharString();                            // 0x0099EAE0
    CWideString ToWideString() const;          // 0x0099E480
};

struct CDefStringTable {
    CCharString GetString(long id);            // 0x009D49B0
};

extern CDefStringTable g_DefStringTable;       // 0x013CA828

class CDefString { public:
    long TablePos;                             // +0
    CWideString ToWideString() const;
};

CWideString CDefString::ToWideString() const
{
    CCharString text = g_DefStringTable.GetString(TablePos);
    return text.ToWideString();
}