// CPersistTraits<CFaction>::TransferBinaryOut(CDataOutputStream*, const CFaction*) -- save the faction as text into a string through a saving persist context, then write the text's length and characters
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BA030, 121 bytes, `ret 8` (__fastcall on the traits: its TransferOut is called on `this`).
// Callees: CCharString::CCharString() @ 0x0099E4B0, CPersistContext::CPersistContext(CCharString&) @ 0x009BAE10 (the
//   saving context, writes into the string), CPersistTraits<CFaction>::TransferOut(CPersistContext*, CCharString*,
//   const CFaction*) @ 0x006B91A0 (manifest "Faction_ExportToString"), CDataOutputStream::WriteULong @ 0x00993EA0,
//   CDataOutputStream::WriteData(const void*, unsigned long) @ 0x00996610, ~CCharString @ 0x0099EAE0.
// Layout: CCharString = pointer to CCharStringData { CBasicString<char> { char* PData; ulong StringLength; ... } }
//   (the landed 0x00463334 model); the empty string reads as length 0 and the static "" 0x0129AAF4. CPersistContext is
//   0x2C bytes (Ego_r.pdb) with no destructor. The traits holds the default CFaction (landed caller 0x006BABD0).
// Shape: the string's inline GetLength()/GetString() are written null-test-first (`if (!PStringData) return ...;`):
//   the first length reuses the zero data pointer, the second pair shares one test with the empty arm laid out first
//   (the `?:` form inverts both). Length pushed first.
//   Speed-optimised (default) TU.
class CDataOutputStream;
struct CBasicStringChar {
    char*         PData;                              // +0x00
    unsigned long StringLength;                       // +0x04
};
class CCharStringData { public:
    CBasicStringChar Data;
};
extern const char g_EmptyCharString[];                // 0x0129AAF4
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                    // 0x0099E4B0
    ~CCharString();                                   // 0x0099EAE0
    unsigned long GetLength() const
    {
        if (PStringData == 0)
            return 0;
        return PStringData->Data.StringLength;
    }
    const char* GetString() const
    {
        if (PStringData == 0)
            return g_EmptyCharString;
        return PStringData->Data.PData;
    }
};
struct CDataOutputStream_Methods {
    void WriteULong(unsigned long value);             // 0x00993EA0
    void WriteData(const void* data, unsigned long size);   // 0x00996610
};
class CPersistContext { public:
    unsigned char _body[0x2C];
    CPersistContext(CCharString& output);             // 0x009BAE10
};
struct CCharStringSet {                               // STLport set<CCharString>, 12 bytes in retail
    void*         m_header;
    unsigned long m_count;
    long          m_compare;
};
class CFaction { public:
    CCharString    Name;                              // +0x00
    CCharStringSet AlliedFactions;                    // +0x04
    CCharStringSet EnemyFactions;                     // +0x10
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CFaction> {
    CFaction DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, const CFaction* value);   // 0x006B91A0
    void TransferBinaryOut(CDataOutputStream* stream, const CFaction* value);
};

void CPersistTraits<CFaction>::TransferBinaryOut(CDataOutputStream* stream, const CFaction* value)
{
    CCharString text;
    CPersistContext context(text);
    TransferOut(&context, &text, value);
    CDataOutputStream_Methods* out = (CDataOutputStream_Methods*)stream;
    out->WriteULong(text.GetLength());
    out->WriteData(text.GetString(), text.GetLength());
}