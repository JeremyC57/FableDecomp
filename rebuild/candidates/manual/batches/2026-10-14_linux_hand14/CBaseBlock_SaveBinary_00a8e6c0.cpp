// CBaseBlock::SaveBinary(CDataOutputStream&) -- two dwords, a bool, a byte and a bool through the stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8E6C0, oracle length 71 bytes.
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0 (x2), WriteBool @ 0x00993E30 (x2), WriteByte @ 0x00993E50.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; push edi ; mov edi,[esp+0Ch] ; push eax ; mov ecx,edi ; call WriteULong ;
//                mov ecx,[esi+4] ; push ecx ; mov ecx,edi ; call WriteULong ; xor edx,edx ; mov dl,[esi+8] ; mov ecx,edi ; push edx ;
//                call WriteBool ; xor eax,eax ; mov al,[esi+0Ah] ; mov ecx,edi ; push eax ; call WriteByte ; xor ecx,ecx ;
//                mov cl,[esi+9] ; push ecx ; mov ecx,edi ; call WriteBool ; pop edi ; pop esi ; ret 4
// The block is not polymorphic (its first dword is written as data).  The manifest labels the first
// callee "WriteCBYTE" but it receives a whole dword; the byte members are zero-extended into a
// register before the push.  Member +0x0a is written before +0x09.
struct CDataOutputStream_Calls {
    void WriteULong(unsigned long value);     // 0x00993EA0
    void WriteBool(bool value);               // 0x00993E30
    void WriteByte(unsigned char value);      // 0x00993E50
};

struct CBaseBlock {
    unsigned long m_id;          // +0x00
    unsigned long m_size;        // +0x04
    bool          m_flagA;       // +0x08
    bool          m_flagB;       // +0x09
    unsigned char m_type;        // +0x0a
    void SaveBinary(CDataOutputStream_Calls& stream);   // 0x00A8E6C0
};

void CBaseBlock::SaveBinary(CDataOutputStream_Calls& stream)
{
    stream.WriteULong(m_id);
    stream.WriteULong(m_size);
    stream.WriteBool(m_flagA);
    stream.WriteByte(m_type);
    stream.WriteBool(m_flagB);
}
