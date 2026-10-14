// CArenaRoundDef_00c6a280::CArenaRoundDef_00c6a280() -- EH-framed constructor: out-of-line chunk base constructor (@ 0x00C74420), own vtable (0x012B9590), the array of four 0x28-byte behaviour elements at +4 built through the vector constructor iterator (`__ehvec_ctor`, element constructor @ 0x00C698D0, destructor @ 0x00C69930)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A280, oracle length 89 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CChunkBase::CChunkBase ; push offset ~CElement ; push offset CElement::CElement ; push 4 ; push 28h ; lea eax,[esi+4] ; push eax ; mov dword ptr [esp+24h],0 ; mov dword ptr [esi],offset vtable ; call __ehvec_ctor ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// An array member of class type with a constructor and destructor is built through the CRT's
// vector constructor iterator (which destroys the built elements if one throws); EH state 0 is
// entered once the base exists. The element type is the chunk-holding class of 0x00C698D0 /
// 0x00C69930. The manifest label is propagated; the class is named by address.
struct CChunkBase_00c6a280 {
    virtual void v00();
    CChunkBase_00c6a280();                 // 0x00C74420
    ~CChunkBase_00c6a280();
};

struct CElement_00c6a280 {                 // 0x28 bytes
    unsigned char _body[0x28];
    CElement_00c6a280();                   // 0x00C698D0
    ~CElement_00c6a280();                  // 0x00C69930
};

struct CArenaRoundDef_00c6a280 : CChunkBase_00c6a280 {   // vtable 0x012B9590
    CElement_00c6a280 m_items[4];          // +4
    CArenaRoundDef_00c6a280();
};

CArenaRoundDef_00c6a280::CArenaRoundDef_00c6a280()
{
}