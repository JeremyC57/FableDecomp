// SStringPairRecord_0055de90::SStringPairRecord_0055de90() -- constructor: an empty CCharString at +0, the index word at +4 set to -1, an empty CCharString at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055DE90, length 27 bytes. Callee: CCharString::CCharString() @ 0x0099AED0 (twice).
// The manifest label (CObservable::CObservable) is propagated and wrong: the PDB CObservable is a polymorphic 32-byte
// class (vptr, two observer lists, an exclusive observer) and this body stores no vtable; it builds a 12-byte record
// {CCharString, long, CCharString}. Named by effect. Shape: the initialiser list in declaration order -- the first
// string constructed on `this` itself, the -1 word stored between the `lea ecx,[esi+8]` and the second constructor call.
// Leaf: ??0SStringPairRecord_0055de90
class CCharString {
public:
    CCharString();                                 // 0x0099AED0
    void* m_pStringData;
};

struct SStringPairRecord_0055de90 {
    CCharString m_first;                           // +0x00
    long        m_index;                           // +0x04
    CCharString m_second;                          // +0x08

    SStringPairRecord_0055de90();
};

SStringPairRecord_0055de90::SStringPairRecord_0055de90()
    : m_index(-1)
{
}