// CAIStateGroupBehaviour_00c698d0::CAIStateGroupBehaviour_00c698d0() -- EH-framed constructor: out-of-line chunk-base constructor on `this` (@ 0x00C74420), own vtable (0x012B958C), then the chunk member at +0x4 (the same base constructor, then its vtable 0x012B9578 stored inline)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C698D0, oracle length 84 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; call CChunkBase::CChunkBase ; lea edi,[esi+0x4] ; mov ecx,edi ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset vtable ; call CChunkBase::CChunkBase ; mov ecx,[esp+0Ch] ; mov dword ptr [edi],offset chunk vtable ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The constructor counterpart of the destructors at 0x00C69930/0x00C69990: the class derives from
// the chunk base (out-of-line constructor and destructor: EH state 0 once it exists) and holds a
// chunk member whose inline constructor runs the same base constructor and then stores the chunk
// vtable. The member's address is kept in edi across its base constructor call. The class comes
// from the manifest label; the chunk types are descriptive.
struct CChunkBase_00c698d0 {
    virtual void v00();
    CChunkBase_00c698d0();                      // 0x00C74420
    ~CChunkBase_00c698d0();
};

struct CChunk_00c698d0 : CChunkBase_00c698d0 {      // vtable 0x012B9578
    CChunk_00c698d0() { }
};

struct CAIStateGroupBehaviour_00c698d0 : CChunkBase_00c698d0 {   // vtable 0x012B958C
    CChunk_00c698d0 m_chunk;                    // +0x4
    CAIStateGroupBehaviour_00c698d0();
};

CAIStateGroupBehaviour_00c698d0::CAIStateGroupBehaviour_00c698d0()
    : m_chunk()
{
}