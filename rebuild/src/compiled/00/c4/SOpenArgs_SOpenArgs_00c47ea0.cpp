// SOpenArgs::~SOpenArgs() -- implicit destructor of the decoder's open-request arguments: releases their +0x10 name pointer
// Leaf: ??1SOpenArgs@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C47EA0, 8 bytes: `add ecx,10h ; jmp 0x00C167C0` (thiscall tail call).
// Identity: DESCRIPTIVE names (libpk has no PDB or RTTI; recorded in NOTES). Its one reference is the EH unwind funclet 0x01215920
//   (`lea ecx,[ebp-94h] ; jmp 0x00C47EA0`), state 2 of FuncInfo 0x0136FC80, whose handler 0x01215939 is pushed by the landed
//   CAsyncDecoder_00c48640::Run 0x00C47F10: state 2 there is the open request copied out of the request, the struct Run models as
//   SOpenArgs {opener, segment, name pointer at +0x10, reply queue}. The member's destructor 0x00C167C0 is spelled as Run binds it
//   (??1CIntelligentPointer_00c16900@@QAE@XZ).
// Shape: a compiler-generated destructor; EmitOpenArgsDestructor below only makes the translation unit emit the implicit ??1, it is
//   not a retail function.
struct CIntelligentPointer_00c16900 {           // 8 bytes: the request's name pointer
    void* m_p0;
    void* m_p4;
    ~CIntelligentPointer_00c16900();            // 0x00C167C0
};

struct SOpenArgs {                              // the open request viewed from the record's +4
    void*                        m_opener;      // +0x00 the stream opener
    unsigned long                m_segment[3];  // +0x04 the segment {start, length, ...}
    CIntelligentPointer_00c16900 m_name;        // +0x10
    void*                        m_reply;       // +0x18 the reply queue
};

void EmitOpenArgsDestructor(SOpenArgs* args)    // emits the implicit ??1; not a retail function
{
    delete args;
}