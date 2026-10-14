// CTCKraken_00c69930::~CTCKraken_00c69930() -- EH-framed destructor: the chunk member at +4 is destroyed inline (its vtable 0x012B9578 stored, then the chunk base destructor @ 0x00C74450), then the class's own base destructor @ 0x00C74450 on `this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69930, oracle length 82 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; lea ecx,[esi+4] ; mov dword ptr [esp+10h],0 ; mov dword ptr [ecx],offset chunk vtable ; call CChunkBase::~CChunkBase ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call CChunkBase::~CChunkBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// No vtable of its own is stored at entry, so the class is `__declspec(novtable)`; its member at
// +4 is a chunk whose inline empty destructor stores the chunk vtable and runs the shared chunk
// base destructor (the `DtorToBase` shape of the tiny families), under EH state 0 while the
// class's own base is alive; then that base's out-of-line destructor runs on `this` (state -1).
// The manifest label (`EndAttackBeam`) is propagated garbage; the classes are named by address.
struct CChunkBase_00c69930 {
    virtual void v00();
    ~CChunkBase_00c69930();                    // 0x00C74450
};

struct CChunk_00c69930 : CChunkBase_00c69930 {     // vtable 0x012B9578
    ~CChunk_00c69930() { }
};

struct __declspec(novtable) CTCKraken_00c69930 : CChunkBase_00c69930 {

    CChunk_00c69930 m_chunk;                   // +4
    ~CTCKraken_00c69930();
};

CTCKraken_00c69930::~CTCKraken_00c69930()
{
}