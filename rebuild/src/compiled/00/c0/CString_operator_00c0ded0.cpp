// CString::operator<<(double value) -- appends the value formatted with "%.2f" in place and returns *this for chaining (LTCG register convention)
// Leaf: ??6CString@@QAIAAV0@N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DED0, 24 bytes:
//   fld qword [esp+4] ; push esi ; sub esp,8 ; fstp qword [esp] ; mov esi,ecx ; call 0xc0dd60 ; mov eax,esi ;
//   pop esi ; ret 8
// Naming (sweep1, 2026-10-25): was landed as a global `operator&&<...CPredicate_And...>` stand-in (`operator_andand_predicate(Obj*, double)`, a
//   propagated manifest label); descriptive (no PDB covers this library).
//   It is one of the family of `this`-returning `__fastcall` stream operators at 0x00C0DEA0..0x00C0DF20 over the
//   library CString's append helpers: CString 0x00C0DEA0 -> Append(const CString&) 0x00C0DCF0, text 0x00C0DEC0 ->
//   Append(const char*) 0x00C0DD40, double 0x00C0DED0 -> 0x00C0DD60 ("%.2f" @ 0x012B21D4), long 0x00C0DF00 ->
//   0x00C0DDB0 ("%d" @ 0x01265DE8), unsigned 0x00C0DEF0 / 0x00C0DF10 -> 0x00C0DE00 ("%d"), pointer 0x00C0DF20 ->
//   0x00C0DE50 ("%p" @ 0x012B21DC). Every helper formats into a 64-byte stack buffer through the CRT sprintf import
//   (0x01440128) and ends in CString::Append(const char*) 0x00C0DD40, which ends in ConcatInPlace 0x00C0C270.
//   libpk's error-report macro chains them (`file << "(" << (long)line << ") : " << text`, chunk 436 PKCSemaphore
//   0x00C29FC0, which spells the class CDebugStream; it is the same 4-byte CString over the nil buffer 0x0139C900).
// Calling convention: retail passes `this` in ecx and the argument in edx (a double on the stack) with `ret` /
//   `ret 8` (an LTCG-converted member); VC7.1 gives that for a `__fastcall` member.
class CString {
public:
    char* m_pchData;                            // +0x00
    void Append(double value);              // 0x00C0DD60 (thiscall, `ret 8`; "%.2f")
    CString& __fastcall operator<<(double value);
};

CString& __fastcall CString::operator<<(double value)
{
    Append(value);
    return *this;
}
