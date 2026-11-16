// SDecoderMessage::~SDecoderMessage() -- implicit destructor of the asynchronous decoder's 0x40-byte request record: releases its +0x14 name pointer
// Leaf: ??1SDecoderMessage@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C48740, 8 bytes: `add ecx,14h ; jmp 0x00C167C0` (thiscall tail call).
// Identity: DESCRIPTIVE names (libpk has no PDB or RTTI; recorded in NOTES). The landed PKCStaticQueue<SDecoderMessage>::PKCStaticQueue
//   0x00C48F80 declares and binds this destructor (??1SDecoderMessage@@QAE@XZ, handed to `eh vector constructor iterator` for its
//   record array); the record is the one the landed CAsyncDecoder_00c48640::Run 0x00C47F10 takes from its mailbox (modelled there as
//   SDecoderRequest, the same 0x40 bytes) with the request's name pointer at +0x14. The member's destructor 0x00C167C0 is spelled
//   as Run binds it with its copy constructor 0x00C16900 and Get 0x00C168A0 (??1CIntelligentPointer_00c16900@@QAE@XZ; the other
//   spellings bound to 0x00C167C0 are queued in NAME_FIXES).
// Shape: a compiler-generated destructor; EmitDecoderMessageDestructor below only makes the translation unit emit the implicit ??1,
//   it is not a retail function.
struct CIntelligentPointer_00c16900 {           // 8 bytes: the request's name pointer
    void* m_p0;
    void* m_p4;
    ~CIntelligentPointer_00c16900();            // 0x00C167C0
};

struct SDecoderMessage {                        // 0x40 bytes: one async-decoder request record
    long                         m_type;        // +0x00 request type (1 open, 2 close, 3 seek, 4 read, 5 exit)
    unsigned char                m_args[0x10];  // +0x04
    CIntelligentPointer_00c16900 m_name;        // +0x14
    unsigned char                m_payload[0x40 - 0x1c];
};

void EmitDecoderMessageDestructor(SDecoderMessage* message)   // emits the implicit ??1; not a retail function
{
    delete message;
}