// Behaviour test for LoadDataFromMarker 0x00CD5170 (body copied from src/00cd5170.cpp; CCharString / GF helpers and the
// vector growth implemented here).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
#include <stddef.h>
#include <new>

class CBasicString_char {                               // CBasicString<char> (Ego_r.pdb, 13 bytes); only the first words used
public:
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
};
class CCharStringData {
public:
    CBasicString_char Data;                             // +0x00
};
extern const char g_emptyLiteral_0129aaf4[];
const char g_emptyLiteral_0129aaf4[] = "";

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

// ---- test implementations of the retail callees ----
static CCharStringData* MakeData(const char* text, unsigned long n)
{
    CCharStringData* d = (CCharStringData*)std::malloc(sizeof(CCharStringData));
    d->Data.PData = (char*)std::malloc(n + 1);
    std::memcpy(d->Data.PData, text, n);
    d->Data.PData[n] = 0;
    d->Data.StringLength = n;
    return d;
}
CCharString::CCharString(const char* text, long length)
{
    unsigned long n = length < 0 ? (unsigned long)std::strlen(text) : (unsigned long)length;
    PStringData = n ? MakeData(text, n) : 0;
}
CCharString::CCharString(const CCharString& other)
{
    PStringData = other.PStringData ? MakeData(other.PStringData->Data.PData, other.PStringData->Data.StringLength) : 0;
}
CCharString::~CCharString()
{
    if (PStringData) { std::free(PStringData->Data.PData); std::free(PStringData); }
}
const CCharString& CCharString::operator=(const CCharString& other)
{
    if (this != &other) {
        this->~CCharString();
        new (this) CCharString(other);
    }
    return *this;
}
CCharString CCharString::GetSubString(unsigned long start, unsigned long end) const
{
    unsigned long len = (unsigned long)GetLength();
    if (end > len) end = len;
    if (start > end) start = end;
    return CCharString(GetAsCharArray() + start, (long)(end - start));
}
void CCharString::Append(char c, unsigned long count)
{
    unsigned long len = (unsigned long)GetLength();
    char* buf = (char*)std::malloc(len + count + 1);
    std::memcpy(buf, GetAsCharArray(), len);
    for (unsigned long i = 0; i < count; ++i) buf[len + i] = c;
    this->~CCharString();
    PStringData = MakeData(buf, len + count);
    std::free(buf);
}
bool __fastcall GFIsCharStringNumber(const CCharString& text)
{
    const char* p = text.GetAsCharArray();
    if (!*p) return false;
    if (*p == '-') ++p;
    for (; *p; ++p) if (*p < '0' || *p > '9') return false;
    return true;
}
long __fastcall GFCharStringToInt(const CCharString& text) { return std::atol(text.GetAsCharArray()); }

static int g_overflows = 0;
namespace std {
template <class T, class A>
void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool)
{
    ++g_overflows;
    unsigned int size = (unsigned int)(_M_finish - _M_start);
    unsigned int cap = size + (size > fillLength ? size : fillLength);
    T* mem = (T*)std::malloc(cap * sizeof(T));
    T* out = mem;
    for (T* p = _M_start; p != position; ++p, ++out) _Construct(out, *p);
    for (unsigned int i = 0; i < fillLength; ++i, ++out) _Construct(out, x);
    for (T* p = position; p != _M_finish; ++p, ++out) _Construct(out, *p);
    for (T* p = _M_start; p != _M_finish; ++p) p->~T();
    std::free(_M_start);
    _M_start = mem; _M_finish = out; _M_end_of_storage = mem + cap;
}
}

static bool Is(const CCharString& s, const char* want) { return std::strcmp(s.GetAsCharArray(), want) == 0; }

int main()
{
    CDataMarkerOutput out;
    out.Elements._M_start = out.Elements._M_finish = out.Elements._M_end_of_storage = 0;
    CCharString marker("#HP150 $NMBob #XYabc #ZZ-7");
    LoadDataFromMarker(marker, out);
    const CDataMarkerOutput::CElement* e = out.Elements._M_start;
    unsigned int n = (unsigned int)(out.Elements._M_finish - e);
    if (n != 4) { std::printf("00cd5170_TEST FAIL count %u\n", n); return 1; }
    if (!Is(e[0].VariableName, "HP") || !Is(e[0].DataAsString, "150") || e[0].DataAsULONG != 150) { std::printf("00cd5170_TEST FAIL e0\n"); return 1; }
    if (!Is(e[1].VariableName, "NM") || !Is(e[1].DataAsString, "Bob") || e[1].DataAsULONG != 10) { std::printf("00cd5170_TEST FAIL e1\n"); return 1; }
    if (!Is(e[2].VariableName, "XY") || !Is(e[2].DataAsString, "abc") || e[2].DataAsULONG != 10) { std::printf("00cd5170_TEST FAIL e2\n"); return 1; }
    if (!Is(e[3].VariableName, "ZZ") || !Is(e[3].DataAsString, "-7") || e[3].DataAsULONG != (unsigned long)-7) { std::printf("00cd5170_TEST FAIL e3\n"); return 1; }
    if (g_overflows == 0) { std::printf("00cd5170_TEST FAIL growth\n"); return 1; }
    // a '$' entry with a numeric value keeps the default
    CCharString second("$AB42");
    LoadDataFromMarker(second, out);
    e = out.Elements._M_start;
    if (out.Elements._M_finish - e != 5 || !Is(e[4].DataAsString, "42") || e[4].DataAsULONG != 10) { std::printf("00cd5170_TEST FAIL dollar\n"); return 1; }
    // an empty marker adds nothing
    CCharString empty("");
    LoadDataFromMarker(empty, out);
    if (out.Elements._M_finish - out.Elements._M_start != 5) { std::printf("00cd5170_TEST FAIL empty\n"); return 1; }
    std::printf("00cd5170_TEST PASS\n");
    return 0;
}