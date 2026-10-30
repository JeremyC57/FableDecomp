// LoadDataFromMarker(const CCharString& marker, CDataMarkerOutput& output) -- parse a data marker string into elements: each entry is a one-character type, a two-character variable name and a value running to the next space; a '#' entry whose value is a number also stores it as DataAsULONG (default 10)
// Leaf: ?LoadDataFromMarker@@YIXABVCCharString@@AAVCDataMarkerOutput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD5170, 352 bytes, `__fastcall` global (marker in ecx, output in edx),
//   plain `ret`; speed TU (no frame pointer) like its landed neighbours GetStringFromDataMarker 0x00CD4FF0 and
//   GetNumberFromDataMarker 0x00CD5090.
// Identity (proven): Ego_r.pdb (donor 0x00CB6230) and FableWin.pdb (0x035D4240) both carry this mangled name, and the
//   Ego_r donor order GetStringFromDataMarker, GetNumberFromDataMarker, LoadDataFromMarker is retail's order (0x00CD4FF0,
//   0x00CD5090, 0x00CD5170). The argument shape (a const CCharString read, a CElement vector filled by push_back with
//   _M_insert_overflow 0x00CDBDC0) is the PDB signature. Manifest label correct.
// Callees (Ego_r + FableWin spellings): CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   CCharString(const CCharString&) 0x0099EC30, operator= 0x0099EFB0, GetSubString(unsigned long, unsigned long) const
//   0x0099EC70, Append(char, unsigned long) 0x0099F3B0; GFIsCharStringNumber 0x0099E620, GFCharStringToInt 0x0099E7F0;
//   STLport vector<CDataMarkerOutput::CElement>::_M_insert_overflow 0x00CDBDC0 (protected: IAE).
// Data: "" (the literal 0x0122D70E, CElement's two strings); 0x0129AAF4 is the other "" (the inline GetAsCharArray of
//   a string with no data block) -- a second address, so it is the extern g_emptyLiteral_0129aaf4 (landed spelling).
// Layout (Ego_r.pdb): CCharString { CCharStringData* PStringData }, CCharStringData { CBasicString<char> Data
//   { char* PData +0, unsigned long StringLength +4, ... }, long NoRefs +0xd }; CDataMarkerOutput { vector<CElement>
//   Elements } (STLport in retail, 12 bytes), CElement { CCharString VariableName, CCharString DataAsString, unsigned long
//   DataAsULONG } (12 bytes). No engine header exists for CCharString or CDataMarkerOutput.
// Shape: the element is a loop-body local (built "" / "" / 10, destroyed DataAsString first); GetLength and
//   GetAsCharArray are inline `PStringData == 0 ? 0 / "" : ...` (null arm first: retail's jne order and the preloaded
//   "" in later reads); the value loop is rotated (space test, then the
//   `pos <= GetLength()` test); push_back is STLport's inline (_Construct copy, else _M_insert_overflow(finish, x,
//   __false_type(), 1, true)).
#include <stddef.h>
inline void* operator new(size_t, void* where) { return where; }

class CBasicString_char {                               // CBasicString<char> (Ego_r.pdb, 13 bytes); only the first words used
public:
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
};
class CCharStringData {
public:
    CBasicString_char Data;                             // +0x00
};
extern const char g_emptyLiteral_0129aaf4[];            // 0x0129AAF4: ""

class CCharString {
public:
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);                       // 0x0099EFB0
    CCharString GetSubString(unsigned long start, unsigned long end) const;       // 0x0099EC70
    void Append(char c, unsigned long count);                                     // 0x0099F3B0
    long GetLength() const { return PStringData == 0 ? 0 : (long)PStringData->Data.StringLength; }
    const char* GetAsCharArray() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->Data.PData; }
    CCharStringData* PStringData;
};
bool __fastcall GFIsCharStringNumber(const CCharString& text);   // 0x0099E620
long __fastcall GFCharStringToInt(const CCharString& text);      // 0x0099E7F0

namespace std {
struct __false_type { __false_type() { } };            // user ctor: no zero store into the tag temporary (GOTCHAS)
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd = false);   // 0x00CDBDC0
};
}

class CDataMarkerOutput {
public:
    class CElement {
    public:
        CElement() : VariableName(""), DataAsString(""), DataAsULONG(10) { }
        CCharString   VariableName;                     // +0x00
        CCharString   DataAsString;                     // +0x04
        unsigned long DataAsULONG;                      // +0x08
    };
    std::vector<CElement> Elements;                     // +0x00
};

void __fastcall LoadDataFromMarker(const CCharString& marker, CDataMarkerOutput& output)
{
    long pos = 0;
    while (pos < marker.GetLength()) {
        CDataMarkerOutput::CElement element;
        char type = marker.GetAsCharArray()[pos++];
        element.VariableName = marker.GetSubString(pos, pos + 2);
        pos += 2;
        char c = marker.GetAsCharArray()[pos++];
        while (c != ' ' && pos <= marker.GetLength()) {
            element.DataAsString.Append(c, 1);
            c = marker.GetAsCharArray()[pos++];
        }
        if (type == '#' && GFIsCharStringNumber(element.DataAsString))
            element.DataAsULONG = GFCharStringToInt(element.DataAsString);
        output.Elements.push_back(element);
    }
}