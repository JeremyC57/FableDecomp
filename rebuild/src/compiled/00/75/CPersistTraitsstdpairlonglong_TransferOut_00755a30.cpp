// CPersistTraits<std::pair<long,long> >::TransferOut(CPersistContext&, CCharString&, const std::pair<long,long>*) -- text persist write of a long pair as "(first,second)"
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x00755A30, 98 bytes, `__fastcall`, `ret 0Ch`.
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 (x3; literals "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020),
//   TransferOutLong @ 0x009E2560 (x2, `__fastcall`: context in ecx, output string in edx, &value on the stack).
// Shape: each element goes through its own element-traits object, CPersistTraits<long>::TransferOut, inlined (the
//   out-of-line copy is 0x005CC6C0): the element is copied into the dead context-argument slot whose address goes to
//   TransferOutLong. The long traits' DefaultVal zeroing is dead (the inlined method never reads it) and is dropped.
class CPersistContext;
class CCharString {
public:
    void* PStringData;
    CCharString& operator+=(const char* text);                       // 0x0099F100
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

template struct CPersistTraits<std::pair<long, long> >;