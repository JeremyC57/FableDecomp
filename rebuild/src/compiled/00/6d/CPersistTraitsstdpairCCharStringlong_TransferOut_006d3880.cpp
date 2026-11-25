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
// Name (2026-10-31, hand621 NAME_FIXES): 0x0040E630 is ?TransferOut@?$CPersistTraits@VCCharString@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABV3@@Z
//   (Ego_r.pdb 0x0040C2D0 / FableWin.pdb 0x0189E7E0: non-const member, value by const reference; was QBE with a const
//   pointer), so CPersistTraits<CCharString> is its own specialization and the element traits take the value by reference
//   (the long traits stay inline). Bytes unchanged.
class CPersistContext;
class CCharString {
public:
    void* PStringData;
    CCharString();                                                    // 0x0099E4B0
    ~CCharString();                                                   // 0x0099EAE0
    const CCharString& operator+=(const char* text);                       // 0x0099F100
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560
// Literal (NF420, 2026-11-25): made-up data symbol(s) replaced by retail's pooled literal (/GF-merged ??_C@) at each use site:
//   g_textOpenBracket_0122e028 (table ?g_textOpenBracket_0122e028@@3QBDB -> 0x0122E028) -> "("
//   g_textComma_0122e024 (table ?g_textComma_0122e024@@3QBDB -> 0x0122E024) -> ","
//   g_textCloseBracket_0122e020 (table ?g_textCloseBracket_0122e020@@3QBDB -> 0x0122E020) -> ")"

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferOut(CPersistContext& context, CCharString& out, const T& value) const;
};

template <> inline void CPersistTraits<long>::TransferOut(CPersistContext& context, CCharString& out, const long& value) const
{
    long copy = value;
    TransferOutLong(&context, &out, &copy);
}

template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferOut(CPersistContext& context, CCharString& out, const CCharString& value);   // 0x0040E630
};

template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    std::pair<T1, T2> DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const std::pair<T1, T2>* value) const
    {
        CPersistTraits<T1> firstTraits;
        CPersistTraits<T2> secondTraits;
        out += "(";
        firstTraits.TransferOut(context, out, value->first);
        out += ",";
        secondTraits.TransferOut(context, out, value->second);
        out += ")";
    }
};

template struct CPersistTraits<std::pair<CCharString, long> >;