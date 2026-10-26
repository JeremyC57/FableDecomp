// CPersistTraits<CBrainBehaviour>::CPersistTraits() -- builds the traits default behaviour: an empty Name def string and zero Priority / ScriptAIPriority
// Leaf: ??0?$CPersistTraits@VCBrainBehaviour
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004670F5, 54 bytes, plain `ret` (returns `this`).
// Ego_r.pdb: ??0?$CPersistTraits@VCBrainBehaviour@@@@QAE@XZ; CBrainBehaviour (12 bytes) = { CDefString Name +0,
// long Priority +4, long ScriptAIPriority +8 }.
// Callees: 0x0099EBF0 CCharString::CCharString(const char*, long) on "" @ 0x0122D70E, 0x009D5060 CDefString::SetString(
// const CCharString&) (void member, Ego_r.pdb ?SetString@CDefString@@QAEXABVCCharString@@@Z -- or its same-signature
// sibling SetDefaultString), 0x0099EAE0 ~CCharString. Called by the binary vector transfer at 0x004670A0 and 0x0046713C.
// Shape: the "" temporary is built first and destroyed LAST, after the two zero stores, so it is the argument of the
// traits' mem-initializer `DefaultVal("")` (it lives to the end of that full expression, which spans the whole inlined
// CBrainBehaviour(const CCharString&) constructor: Name(name) -> SetString, then Priority, ScriptAIPriority = 0).
// Size TU with the EBP frame kept: `and dword ptr [esi+4],0` zero stores, `leave` (optimize("y", off) + ("s", on));
// the size TU leaves plain inline constructors out of line, so the element constructors are __forceinline.
// Fixed (rename1, 2026-10-24): CBrainBehaviour is a `class` (the Ego_r name ??0?$CPersistTraits@VCBrainBehaviour@@@@QAE@XZ
//   mangles V); it was declared `struct`, which produced ...@UCBrainBehaviour@@... Bytes unchanged.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CDefString {
public:
    long Index;                                        // +0x0 table index
    __forceinline CDefString(const CCharString& name) { SetString(name); }
    void SetString(const CCharString& name);           // 0x009D5060
};

class CBrainBehaviour {                                // 12 bytes
public:
    CDefString Name;                                   // +0x0
    long       Priority;                               // +0x4
    long       ScriptAIPriority;                       // +0x8
    __forceinline CBrainBehaviour(const CCharString& name) : Name(name), Priority(0), ScriptAIPriority(0) { }
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CBrainBehaviour> {
    CBrainBehaviour DefaultVal;
    CPersistTraits();
};

CPersistTraits<CBrainBehaviour>::CPersistTraits() : DefaultVal("")
{
}