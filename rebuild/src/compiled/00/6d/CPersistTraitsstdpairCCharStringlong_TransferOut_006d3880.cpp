// CPersistTraits<std::pair<CCharString,long> >::TransferOut(CPersistContext&, CCharString&, const std::pair<CCharString,long>*) -- text persist write of a {string, long} pair as "(first,second)"
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D3880, 110 bytes, `__fastcall`, `ret 0Ch`.
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 (x3; literals "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020),
//   TransferOutLong @ 0x009E2560 (`__fastcall`: context in ecx, output string in edx, &value on the stack),
//   CPersistTraits<CCharString>::TransferOut @ 0x0040E630 (out of line, on the traits local), CCharString() @ 0x0099E4B0 and
//   ~CCharString @ 0x0099EAE0 (the string traits' DefaultVal).
// Shape: both element-traits objects are declared first: the string traits' DefaultVal is default-constructed in the
//   `push ecx` frame word before anything else and destroyed last; the long traits' TransferOut is inlined (the element
//   copied into the dead value-argument slot for TransferOutLong; its DefaultVal zeroing is dead and dropped).
class CPersistContext;
class CCharString {
public:
    void* PStringData;
    CCharString();                                                    // 0x0099E4B0
    ~CCharString();                                                   // 0x0099EAE0
    const CCharString& operator+=(const char* text);                       // 0x0099F100
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560
extern const char g_textOpenBracket_0122e028[];                       // 0x0122E028 "("
extern const char g_textComma_0122e024[];                             // 0x0122E024 ","
extern const char g_textCloseBracket_0122e020[];                      // 0x0122E020 ")"

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferOut(CPersistContext& context, CCharString& out, const T* value) const;
};

template <> inline void CPersistTraits<long>::TransferOut(CPersistContext& context, CCharString& out, const long* value) const
{
    long copy = *value;
    TransferOutLong(&context, &out, &copy);
}

template <> void CPersistTraits<CCharString>::TransferOut(CPersistContext& context, CCharString& out, const CCharString* value) const;   // 0x0040E630

template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    std::pair<T1, T2> DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const std::pair<T1, T2>* value) const
    {
        CPersistTraits<T1> firstTraits;
        CPersistTraits<T2> secondTraits;
        out += g_textOpenBracket_0122e028;
        firstTraits.TransferOut(context, out, &value->first);
        out += g_textComma_0122e024;
        secondTraits.TransferOut(context, out, &value->second);
        out += g_textCloseBracket_0122e020;
    }
};

template struct CPersistTraits<std::pair<CCharString, long> >;