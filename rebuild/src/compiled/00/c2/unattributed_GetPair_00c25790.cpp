// <unattributed>::GetPair() -- returns the {long, float} pair at +0x00 by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25790, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  fld dword ptr [ecx+4] ; mov ecx,[ecx] ; mov eax,[esp+4] ; fstp dword ptr [eax+4] ; mov [eax],ecx ; ret 4
// An 8-byte struct returned by value through the hidden result pointer (first stack slot, also
// returned in eax). The first word is copied as an integer, the second through the x87 stack, so the
// pair is {long, float}.
struct CIndexedValue {
    long  Index;    // +0x00
    float Value;    // +0x04
    CIndexedValue(long index, float value) : Index(index), Value(value) {}
};

struct CPairOwner_Retail {
    long  Index;    // +0x00
    float Value;    // +0x04
    CIndexedValue GetPair();
};

CIndexedValue CPairOwner_Retail::GetPair()
{
    // Built through the (long, float) constructor: the float travels on the x87 stack and the
    // long through a register, exactly the retail store pattern (a POD copy uses two movs).
    return CIndexedValue(this->Index, this->Value);
}