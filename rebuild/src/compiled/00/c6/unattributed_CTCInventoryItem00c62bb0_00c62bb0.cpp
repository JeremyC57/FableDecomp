// CTCInventoryItem_00c62bb0::~CTCInventoryItem_00c62bb0() -- EH-framed destructor: own vtable (0x012B938C), the block at +0x14 freed through operator delete and cleared when present, then the out-of-line base destructor (@ 0x00C62E10)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62BB0, oracle length 97 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+14h] ; test eax,eax ; mov dword ptr [esp+10h],0 ; je skip ; push eax ; call operator delete ; add esp,4 ; mov dword ptr [esi+14h],0 ; skip: mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 0 covers the body (the base is alive, its destructor is out of line). The manifest
// label (`OnDie`) is propagated garbage; the class is named by address.
struct CInventoryItemBase_00c62bb0 {
    virtual void v00();
    ~CInventoryItemBase_00c62bb0();        // 0x00C62E10
};

struct CTCInventoryItem_00c62bb0 : CInventoryItemBase_00c62bb0 {   // vtable 0x012B938C
    unsigned char _pad[0x10];
    void* m_pBlock;                        // +0x14
    ~CTCInventoryItem_00c62bb0();
};

CTCInventoryItem_00c62bb0::~CTCInventoryItem_00c62bb0()
{
    if (m_pBlock) {
        ::operator delete(m_pBlock);
        m_pBlock = 0;
    }
}