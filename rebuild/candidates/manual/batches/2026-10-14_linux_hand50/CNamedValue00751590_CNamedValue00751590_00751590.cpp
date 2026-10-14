// CNamedValue_00751590::CNamedValue_00751590() -- a non-polymorphic 16-byte record: the value word (+0) cleared and the two CCharString members (+4, +0xc) default-constructed; the word at +8 is left alone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751590, length 29 bytes. Returns `this` (`mov eax,esi ; ret`), so a constructor.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (twice; stores a null data pointer and bumps the instance counter).
// The chunk label "CInputProcessManager::~CInputProcessManager" is propagated. Shape: the zero store is scheduled after the first
// member's address is formed (`lea ecx,[esi+4] ; mov dword ptr [esi],0`), the two constructor calls follow in declaration order.
// Leaf: ??0CNamedValue_00751590
struct CCharString {                                // 4 bytes: the shared string data pointer
    void* storage_;
    CCharString();                                  // 0x0099E4B0
};
struct CNamedValue_00751590 {
    long        m_value;                            // +0x00
    CCharString m_name;                             // +0x04
    long        m_reserved;                         // +0x08 (not initialised here)
    CCharString m_text;                             // +0x0c
    CNamedValue_00751590();
};

CNamedValue_00751590::CNamedValue_00751590()
    : m_value(0), m_name(), m_text()
{
}
