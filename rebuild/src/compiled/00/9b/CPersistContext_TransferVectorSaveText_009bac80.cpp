// CPersistContext::TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& vec) -- text save of a vector: one `name[i] = value;\r\n` line per element
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BAC80, 215 bytes (+ int3 padding), `ret 8`.
// Callees: CCharString::operator+=(const char*) 0x0099F100, GFIntToCharString 0x0099F830 (`__fastcall`, hidden result),
//   CCharString::CCharString(const char*, long) 0x0099EBF0, operator+(const CCharString&, const char*) 0x0099F600,
//   operator+(const CCharString&, const CCharString&) 0x0099F570, CCharString::operator+=(const CCharString&) 0x0099F0A0,
//   CCharString::~CCharString 0x0099EAE0; the wrapper's virtual GetCount (slot 0) and TransferOut (slot 2).
// Shape: the element count is read once (ebp) and the index lives in edi. Each line appends the key, then the
//   subscript built as one expression `CCharString("") + "[" + GFIntToCharString(i) + "] "`: VC pushes the rightmost
//   operand "] " first, then makes the index string, pushes "[" and only then builds the empty CCharString -- five
//   temporaries destroyed in reverse order of construction after the append. The element text is written by the
//   wrapper's TransferOut straight into the context's output string, then ";\r\n" ends the line.
//   Layout from Ego_r.pdb: CPersistContext PStringToSaveTo +0x20 (0x2C bytes).
class CStringParser;
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830 (hidden result in ecx)
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

class CPersistContext;
class CPersistContext_TransferableVectorGenericType { public:
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
};

class CPersistContext { public:
    unsigned char           _pad00[0x20];
    CCharString*            PStringToSaveTo;      // +0x20
    unsigned char           _pad24[8];
    void TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& vec);
};

void CPersistContext::TransferVectorSaveText(const char* name, CPersistContext_TransferableVectorGenericType& vec)
{
    long count = vec.GetCount();
    for (long i = 0; i < count; ++i) {
        *PStringToSaveTo += name;
        *PStringToSaveTo += CCharString("") + "[" + GFIntToCharString(i) + "] ";
        vec.TransferOut(this, PStringToSaveTo, i);
        *PStringToSaveTo += ";\r\n";
    }
}