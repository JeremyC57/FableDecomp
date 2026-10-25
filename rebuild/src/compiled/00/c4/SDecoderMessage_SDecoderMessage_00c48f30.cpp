// SDecoderMessage::SDecoderMessage() -- default constructor of the 0x40-byte message record the async decoder's static queue holds (the `eh vector constructor iterator` element constructor of 0x00C48F80): the type word at +0 and the three-word argument record at +8 zeroed, then the owned heap block at +0x14 built out of line
// Leaf: ??0SDecoderMessage@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C48F30, 28 bytes, __fastcall, returns `this`.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only reference: the `push offset` at 0x00C48FCD in the queue
//   constructor 0x00C48F80 (element size 0x40, element destructor 0x00C48740 = `add ecx,14h ; jmp 0x00C167C0`, i.e.
//   the implicit destructor running only the +0x14 member's). The manifest's "CBaseIntelligentPointer_Constructor"
//   is propagated from the member's constructor label.
// Callee: the 8-byte owned block's constructor 0x00C167B0 (zeroes its two words; its destructor 0x00C167C0 deletes the
//   data and nulls it).
// Shape: the +0 store precedes the `lea ecx,[esi+14h]` and the +8..+0x10 stores follow it only when those three words are
//   an embedded record with its own inline zeroing constructor (with four plain initialisers the lea is hoisted above
//   every store; base-class and reordered forms likewise); one `xor eax,eax` feeds all four stores, the word at +4 is
//   not initialised, and the out-of-line member constructor runs last (declaration order).
struct CHeapBlock {
    CHeapBlock();                               // 0x00C167B0
    ~CHeapBlock();                              // 0x00C167C0
    void*         m_pData;
    unsigned long m_size;
};

struct SMessageArgs {
    long m_a;
    long m_b;
    long m_c;
    SMessageArgs() : m_a(0), m_b(0), m_c(0) { }
};

struct SDecoderMessage {
    SDecoderMessage();
    long          m_type;                       // +0x00
    long          m_param;                      // +0x04 (left uninitialised)
    SMessageArgs  m_args;                       // +0x08
    CHeapBlock    m_block;                      // +0x14
    unsigned char m_payload[0x40 - 0x1c];       // +0x1c
};

SDecoderMessage::SDecoderMessage()
    : m_type(0)
{
}