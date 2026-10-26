// CTCInventoryItem_00c3b6b0::~CTCInventoryItem_00c3b6b0() -- EH-framed destructor: the block holder at +4 releases its block through the `__cdecl` release @ 0x00BFEB1C and nulls it (inline), then the member at +0 is destroyed out of line (@ 0x00C0BA40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B6B0, oracle length 91 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esi+4] ; test eax,eax ; mov dword ptr [esp+10h],0 ; je skip ; push eax ; call Release ; add esp,4 ; mov dword ptr [esi+4],0 ; skip: mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CMember ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Members are destroyed last to first: the holder's inline destructor (EH state 0 while the first
// member is alive), then the first member out of line. The manifest label (`OnDie`) is propagated
// garbage; the class is named by address.

void __cdecl operator delete[](void* p);   // 0x00BFEB1C (MSVCR71)
struct CMember_00c3b6b0 {                  // +0
    unsigned char _body[4];
    ~CMember_00c3b6b0();                   // 0x00C0BA40
};

struct CBlockHolder_00c3b6b0 {             // +4
    void* m_pBlock;
    ~CBlockHolder_00c3b6b0() { if (m_pBlock) { operator delete[](m_pBlock); m_pBlock = 0; } }
};

struct CTCInventoryItem_00c3b6b0 {
    CMember_00c3b6b0 m_member;             // +0
    CBlockHolder_00c3b6b0 m_holder;        // +4
    ~CTCInventoryItem_00c3b6b0();
};

CTCInventoryItem_00c3b6b0::~CTCInventoryItem_00c3b6b0()
{
}