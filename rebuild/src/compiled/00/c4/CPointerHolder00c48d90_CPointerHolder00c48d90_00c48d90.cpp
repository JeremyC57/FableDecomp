// CPointerHolder_00c48d90::CPointerHolder_00c48d90() -- constructor: three zeroed words at +4..+0xC, then the intelligent pointer member at +0x10 built out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48D90, length 26 bytes. Callee: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00C167B0.
// The manifest label (CNoise::CNoise) is propagated and wrong: the PDB CNoise is a 6148-byte table (P[512], G2[512],
// Seed at +0x1800, see 0x00F36BA0), while this body touches only 0x14 bytes and nothing in the image calls it
// directly; named by effect. Shape: initialiser-list zeroes share one `xor eax,eax` register and are scheduled
// around the `lea ecx,[esi+10h]` of the member constructor, which runs last (declaration order); the word at +0
// is left alone (a trivially constructed member).
// Leaf: ??0CPointerHolder_00c48d90
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();               // 0x00C167B0
    void* m_pObject;
};

class CPointerHolder_00c48d90 {
public:
    CPointerHolder_00c48d90();
    unsigned long           m_unknown0;      // +0x00 not initialised here
    unsigned long           m_a;             // +0x04
    unsigned long           m_b;             // +0x08
    unsigned long           m_c;             // +0x0C
    CBaseIntelligentPointer m_pointer;       // +0x10
};

CPointerHolder_00c48d90::CPointerHolder_00c48d90()
    : m_a(0), m_b(0), m_c(0)
{
}