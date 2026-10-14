// SPair_00c49d00::~SPair_00c49d00() -- EH-framed destructor of a map node value: the second's block at +0x1c released inline through the `__cdecl` release @ 0x00BFEB1C, then the first's out-of-line destructor (@ 0x00C1C820)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49D00, oracle length 80 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esi+1Ch] ; push eax ; mov dword ptr [esp+14h],0 ; call Release ; add esp,4 ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CFirst ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The manifest label is Dinkumware's `_Dest_val<allocator<pair<CCharString,...>>>`: the second is a
// one-pointer holder whose inline destructor frees its block (EH state 0 while the first is alive),
// then the first (a 0x1c-byte string) is destroyed out of line (state -1). The user-declared pair
// destructor is the landed unit.
void __cdecl Release_00bfeb1c(void* block);   // 0x00BFEB1C

struct CFirst_00c49d00 {                   // 0x1c bytes
    unsigned char _body[0x1c];
    ~CFirst_00c49d00();                    // 0x00C1C820
};

struct CBlockHolder_00c49d00 {             // +0x1c
    void* m_pBlock;
    ~CBlockHolder_00c49d00() { Release_00bfeb1c(m_pBlock); }
};

struct SPair_00c49d00 {
    CFirst_00c49d00 first;                 // +0
    CBlockHolder_00c49d00 second;          // +0x1c
    ~SPair_00c49d00();
};

SPair_00c49d00::~SPair_00c49d00()
{
}