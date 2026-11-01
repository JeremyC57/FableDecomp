// Faithful reconstruction of the compiler-generated destroy helper at 0x0056c820.
// Naming (naming wave 22, ws): the string type is the PDB CWideString (was StrB); name-only.
// Retail body inlines ~pair<CCharString, CSymbolMap::CSymbolInfo>():
//   destroys members in reverse declaration order, tail-calling the last.
// Layout (from disasm [esi+0x10/+0x14/+0x18]):
//   +0x18 : CWideString leaf (dtor 0x99b510)  destroyed first
//   +0x14 : CWideString leaf (dtor 0x99b510)  destroyed second
//   +0x10 : StrA leaf (dtor 0x99eae0)  destroyed last (tail jmp)

// Leaves: dtors are DECLARED ONLY (defined in another TU) so the compiler must
// emit real out-of-line calls, matching the two distinct dtor targets.
class CWideString { public:
    void* p;
    ~CWideString();            // -> 0x99b510
};

struct StrA {
    void* p;
    ~StrA();            // -> 0x99eae0
};

struct SymPair {
    char   _pad[0x10];  // 0x00..0x0F
    StrA   first;       // +0x10
    CWideString   subA;        // +0x14
    CWideString   subB;        // +0x18
    ~SymPair();
};

SymPair::~SymPair()
{
    // Compiler emits: ~subB (+0x18), ~subA (+0x14) [both CWideString, 0x99b510],
    // then ~first (+0x10) [StrA, 0x99eae0] as the tail call.
}