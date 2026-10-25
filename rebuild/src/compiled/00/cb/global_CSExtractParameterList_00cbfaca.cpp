// CSExtractParameterList(CCharString& text, vector<CCharString>& params) -- split a comma-separated cutscene parameter text into the vector (a trailing non-empty token too); returns the vector's size
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?CSExtractParameterList@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBFACA, 179 bytes, `__fastcall` global (text in ecx, vector in edx),
//   plain `ret`, returns int (PDB ?CSExtractParameterList@@YIHAAVCCharString@@AAV?$vector@VCCharString@@...@@@Z).
// Callees: CCharString(const char*, long = -1) @ 0x0099EBF0 ("" @ 0x0122D70E), operator const char*() @ 0x0099E4C0,
//   vector<CCharString>::push_back(const CCharString&) @ 0x0044BFF0 (out of line), operator=(const char*) @ 0x0099EFE0,
//   operator+(const CCharString&, char) @ 0x0099F7A0 (__fastcall: hidden result in ecx, left in edx, the char pushed
//   `movsx`'d), operator=(const CCharString&) @ 0x0099EFB0, ~CCharString @ 0x0099EAE0 (x2).
// Shape: frame-pointer size TU; the "" literal is hoisted into esi for the constructor and the reset assignment; the
//   vector reference is spilled to the frame and re-read in each arm; GetLength() is the zero-reusing inline
//   `data == 0 ? 0 : data->length` (signed: `jge`, and the trailing `> 0` test folds into `je ; cmp [eax+4],0 ; jle`);
//   `current = current + c` builds the sum in a frame temporary that is assigned and destroyed; size() of the 4-byte
//   element vector is `sar esi,2`.
class CCharString {
public:
    struct CStringData {                                        // CBasicString<char>
        char*         m_pChars;                                 // +0x00
        unsigned long m_length;                                 // +0x04
    };
    CStringData* m_pData;
    CCharString(const char* text, long length = -1);            // 0x0099EBF0
    ~CCharString();                                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);           // 0x0099EFB0
    CCharString& operator=(const char* text);                   // 0x0099EFE0
    operator const char*() const;                               // 0x0099E4C0
    long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
};
CCharString __fastcall operator+(const CCharString& left, char right);   // 0x0099F7A0
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& value);                             // 0x0044BFF0 (vector<CCharString>, out of line)
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}

int __fastcall CSExtractParameterList(CCharString& text, std::vector<CCharString>& params)
{
    CCharString current("");
    for (long i = 0; i < text.GetLength(); ++i) {
        if (((const char*)text)[i] == ',') {
            params.push_back(current);
            current = "";
        } else {
            current = current + ((const char*)text)[i];
        }
    }
    if (current.GetLength() > 0)
        params.push_back(current);
    return params.size();
}