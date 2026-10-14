// CKeyedOwner_007467d0::Register() -- hand a copy of the key at +0 to the owner's 2-argument inserter, dropping the 4-byte handle it returns
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007467D0, length 20 bytes.
// Callees: CKeyedOwner_007467d0::Insert(CKey) @ 0x007464A0 (__thiscall on the same object, hidden result + one by-value
// 4-byte class, `ret 8`). The manifest label "CDefString::ToWideString" is propagated across the forwarder shape
// (0x009E4AA0 is the same body on another class); named by address.
// Shape: `push ecx` reserves the frame slot of the discarded result; the key (a 4-byte class with a user copy
// constructor) is built in its argument slot (`push ecx ; mov eax,esp ; mov [eax],edx`), the result slot's
// address is pushed as the hidden pointer, and the slot is popped with `pop ecx` after the call.
struct CKey_007467d0 {
    long m_value;
    CKey_007467d0(const CKey_007467d0& other) : m_value(other.m_value) { }
};

struct CHandle_007467d0 {                  // 4-byte class with a user constructor: returned through a hidden pointer
    void* m_pNode;
    CHandle_007467d0(void* node) : m_pNode(node) { }
};

struct CKeyedOwner_007467d0 {
    CKey_007467d0 m_key;                   // +0
    CHandle_007467d0 Insert(CKey_007467d0 key);   // 0x007464A0
    void Register();
};

void CKeyedOwner_007467d0::Register()
{
    Insert(m_key);
}
