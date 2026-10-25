// CString::operator<<(const CString& string) -- appends another CString in place through Append and returns *this for chaining (LTCG register convention: this in ecx, the source in edx)
// Leaf: ??6CString@@QAIAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DEA0, 13 bytes + int3 padding:
//   push esi ; push edx ; mov esi,ecx ; call 0xc0dcf0 ; mov eax,esi ; pop esi ; ret
// Naming (sweep1, 2026-10-25): was landed as a global `operator&&<NScript::CPredicate_And<...>>` stand-in
//   (`operator_And(CPredicate_And*, CIsThingAlive*)`, a propagated manifest label); descriptive (no PDB covers this
//   library). The only callee is the landed CString::Append(const CString&) 0x00C0DCF0; the body returns *this. It is
//   one of a family of seven 13/24-byte `this`-returning wrappers at 0x00C0DEA0..0x00C0DF20 over the library's append
//   helpers: text (0x00C0DEC0 -> Append(const char*) 0x00C0DD40), double (0x00C0DED0), "%d" numbers (0x00C0DF00 ->
//   0x00C0DDB0, 0x00C0DEF0 / 0x00C0DF10 -> 0x00C0DE00) and "%p" pointers (0x00C0DF20 -> 0x00C0DE50). Its callers chain
//   it (the landed DescribeBitset 0x00C0AD10 at 0x00C0ADAF passes the previous call's result as `this`), and chunk 436's
//   libpk error macro uses the family as `__fastcall` stream operators (`file << "(" << (long)line << ") : " << text`,
//   agent436 LESSONS), so the operator is `<<`. FINDINGS chunk 431 proposed `operator+=(const CString&)`: the same
//   body, but a `const CString&` += cannot be chained and the family's numeric and pointer overloads are stream-style;
//   see NOTES.md (owner note).
// Calling convention: retail passes `this` in ecx and the argument in edx with a bare `ret` (an LTCG-converted member);
//   VC7.1 gives that for a `__fastcall` member, as the landed DescribeBitset 0x00C0AD10 declares it.
class CString {
public:
    char* m_pchData;                            // +0x00
    void Append(const CString& string);         // 0x00C0DCF0 (thiscall, `ret 4`)
    CString& __fastcall operator<<(const CString& string);
};

CString& __fastcall CString::operator<<(const CString& string)
{
    Append(string);
    return *this;
}
