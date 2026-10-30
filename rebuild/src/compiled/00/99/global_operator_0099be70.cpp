// operator+(const CWideString& left, const CWideString& right) -- concatenate two CWideStrings into a new CWideString
// Leaf: ??H@YI?AVCWideString@@ABV0@0@Z
// Retail: Fable.exe 0x0099BE70, 177 bytes; FableWin.pdb ??H@YI?AVCWideString@@ABV0@0@Z: a free __fastcall returning
//   CWideString by value (hidden result in ecx, left in edx, right on the stack, `ret 4`). Callees: CWideString::operator+=
//   (const CWideString&) 0x0099B8D0, CWideString::UnassignString 0x0099B4D0, free 0x00BFEA14, operator delete 0x00BFE9BC.
//   Re-authors the landed CWideString_AddWideStrings asm.
// Shape: `CWideString result(left); result += right; return result;` -- VC7.1 has no NRVO, so the return copies through the
//   inline copy constructor (zero the block, count++, the inline operator= self/same-block tests, the out-of-line
//   UnassignString, share + NoRefs++) and then runs the local's inline destructor (NoRefs--, on zero free the wstring buffer
//   and delete the block, count--). The address-taking callees (CWideString.cpp) are same-TU noinline bodies so the local's
//   address never escapes and its block pointer stays in esi across UnassignString (model of the landed sibling
//   operator+(const unsigned short*, const CWideString&) 0x0099BFF0, which this file reuses verbatim).
// Globals: g_CWideStringInstanceCount 0x013BCA20 (+1 per constructor, -1 per destructor).
// Layout: CWideStringData = STLport basic_string<unsigned short> {_M_start, _M_finish, _M_end_of_storage} + NoRefs +0xC.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern long g_CWideStringInstanceCount;                 // 0x013BCA20
extern unsigned short g_EmptyWideString[];              // 0x0129A8E0

struct forward_iterator_tag {
    forward_iterator_tag() { }
};
struct CBasicStringW {                                  // STLport basic_string<wchar_t>, 12 bytes
    wchar16* _M_start;                                  // +0x00
    wchar16* _M_finish;                                 // +0x04
    wchar16* _M_end_of_storage;                         // +0x08
    static unsigned long length(const wchar16* s)
    {
        unsigned long n = 0;
        while (s[n] != 0)
            ++n;
        return n;
    }
    CBasicStringW(const wchar16* s, unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_range_initialize(s, s + n, forward_iterator_tag());
    }
    ~CBasicStringW()
    {
        if (_M_start != 0)
            free(_M_start);
    }
    void _M_range_initialize(const wchar16* first, const wchar16* last, const forward_iterator_tag& tag);   // 0x0099C670
    CBasicStringW& _M_append(const wchar16* first, const wchar16* last, const forward_iterator_tag& tag);   // 0x0099C7E0
    CBasicStringW& append(const wchar16* s) { return _M_append(s, s + length(s), forward_iterator_tag()); }
};
class CWideStringData { public:                                // 16 bytes
    CBasicStringW Data;                                 // +0x00
    long          NoRefs;                               // +0x0c
    CWideStringData(const wchar16* text, unsigned long n) : Data(text, n), NoRefs(1) { }
};

class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    CWideString(const wchar16* text);                   // 0x0099B6B0
    CWideString(const CWideString& other) : PStringData(0)
    {
        ++g_CWideStringInstanceCount;
        *this = other;
    }
    ~CWideString()
    {
        if (PStringData != 0) {
            if (--PStringData->NoRefs <= 0)
                delete PStringData;
            PStringData = 0;
        }
        --g_CWideStringInstanceCount;
    }
    const CWideString& operator=(const CWideString& other)
    {
        if (&other != this) {
            if (other.PStringData != PStringData) {
                if (PStringData != 0)
                    UnassignString();
                if (other.PStringData != 0) {
                    PStringData = other.PStringData;
                    ++PStringData->NoRefs;
                }
            }
        }
        return *this;
    }
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    const wchar16* GetText() const { return PStringData == 0 ? g_EmptyWideString : PStringData->Data._M_start; }
    void UnassignString();                              // 0x0099B4D0
    void MakeOwnCopyOfData();                           // 0x0099B560
    CWideStringData* AllocStringData(const wchar16* text, long length);   // 0x0099B3C0
};

// ---- same-TU callee bodies (CWideString.cpp), defined before the graded function ----
__declspec(noinline) void CWideString::UnassignString()
{
    if (PStringData != 0) {
        if (--PStringData->NoRefs <= 0)
            delete PStringData;
        PStringData = 0;
    }
}

__declspec(noinline) CWideStringData* CWideString::AllocStringData(const wchar16* text, long length)
{
    return new CWideStringData(text, length == -1 ? CBasicStringW::length(text) : length);
}

__declspec(noinline) void CWideString::MakeOwnCopyOfData()
{
    CWideStringData* data = PStringData;
    if (data != 0 && data->NoRefs > 1) {
        CWideStringData* copy = AllocStringData(data->Data._M_start, -1);
        UnassignString();
        PStringData = copy;
    }
}

__declspec(noinline) CWideString::CWideString(const wchar16* text) : PStringData(0)
{
    ++g_CWideStringInstanceCount;
    if (text != 0 && *text != 0)
        PStringData = AllocStringData(text, -1);
}

__declspec(noinline) const CWideString& CWideString::operator+=(const CWideString& other)
{
    if (PStringData != 0) {
        MakeOwnCopyOfData();
        PStringData->Data.append(other.GetText());
    } else if (&other != this && other.PStringData != 0) {
        PStringData = other.PStringData;
        ++PStringData->NoRefs;
    }
    return *this;
}

// ---- graded ----
CWideString __fastcall operator+(const CWideString& left, const CWideString& right)
{
    CWideString result(left);
    result += right;
    return result;
}