// CPersistTraits<std::pair<long,COpinionDeed*> >::TransferOut(CPersistContext&, CCharString&, const std::pair<long,COpinionDeed*>*) -- text persist write of a {long, opinion deed} pair as "(first,second)"
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C78F0, 100 bytes (+12 int3 padding), `__fastcall`, `ret 0Ch`.
// Callees: CCharString::operator+=(const char*) @ 0x0099F100 (x3; literals "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020),
//   TransferOutLong @ 0x009E2560 (`__fastcall`: context in ecx, output string in edx, &value on the stack),
//   CPersistTraits<COpinionDeed*>::TransferOut @ 0x006C7960 (out of line, on the traits local).
// Identity: the catalog types the pair as <long,long>; the second element goes through the out-of-line
//   CPersistTraits<COpinionDeed*>::TransferOut (manifest label of 0x006C7960), so the pair is <long, COpinionDeed*>.
// Shape: both element-traits objects are declared first. The long traits' TransferOut is inlined (the element copied
//   into the dead context-argument slot for TransferOutLong; its DefaultVal zeroing is dead and dropped); the deed
//   traits' address escapes to the out-of-line call, so its value-initialised DefaultVal (`mov [esp+10h],0`, the
//   `push ecx` frame word) is kept and scheduled after the first argument push.
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

class COpinionDeed;
template <> void CPersistTraits<COpinionDeed*>::TransferOut(CPersistContext& context, CCharString& out, COpinionDeed* const* value) const;   // 0x006C7960

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

template struct CPersistTraits<std::pair<long, COpinionDeed*> >;