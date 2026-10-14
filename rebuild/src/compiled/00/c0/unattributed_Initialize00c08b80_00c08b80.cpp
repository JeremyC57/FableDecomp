// CTCDiceGame_00c08b80::Initialize(void**) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the game at +0x64 exists the word of the argument is stored into +0xd0 of the game's record at +8; else when the word is set a 0xa4-byte game is allocated (`new`, result homed in the dead argument slot, state 1) and built out of line with `this`, the argument and the word at +0x70 (@ 0x00C14E40), and stored at +0x64
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08B80, oracle length 198 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esi+64h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; je make ; mov ecx,[esp+1Ch] ; mov edx,[ecx] ; mov eax,[eax+8] ; lea ecx,[esp+4] ; mov [eax+0D0h],edx ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; ... ret 4 ; make: push edi ; mov edi,[esp+20h] ; cmp dword ptr [edi],0 ; je done ; push 0A4h ; call operator new ; add esp,4 ; mov [esp+20h],eax ; test eax,eax ; mov byte ptr [esp+18h],1 ; je null ; mov ecx,[esi+70h] ; push ecx ; push edi ; push esi ; mov ecx,eax ; call CTCDiceGame::CTCDiceGame ; jmp store ; null: xor eax,eax ; store: mov [esi+64h],eax ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; ...
// The early return keeps its own epilogue. The manifest label is a description; the class is
// named by address (the constructor callee carries the PDB class).
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CDiceHost_00c08b80;
struct CDiceRecord_00c08b80 {
    unsigned char _pad[0xd0];
    void* m_pPlayer;                       // +0xd0
};

struct CTCDiceGame_00c08b80 {              // 0xa4 bytes
    long m_0;
    long m_4;
    CDiceRecord_00c08b80* m_pRecord;       // +8
    unsigned char _pad[0x98];
    CTCDiceGame_00c08b80(CDiceHost_00c08b80* host, void** player, void* table);   // 0x00C14E40
};

struct CDiceHost_00c08b80 {
    unsigned char _pad[0x64];
    CTCDiceGame_00c08b80* m_pGame;         // +0x64
    unsigned char _pad2[8];
    void* m_pTable;                        // +0x70
    void Initialize(void** player);
};

void CDiceHost_00c08b80::Initialize(void** player)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (m_pGame) {
        m_pGame->m_pRecord->m_pPlayer = *player;
        return;
    }
    if (*player)
        m_pGame = new CTCDiceGame_00c08b80(this, player, m_pTable);
}