// CEngineScreenEffectOutlineGlow_00c1e850::~CEngineScreenEffectOutlineGlow_00c1e850() -- EH-framed destructor: an out-of-line release on `this` (@ 0x00C1E800), the registry at +0x74 of the owner at +0 told to drop `this` (virtual slot 8), then the two block holders at +0x14 (state 1) and +8 (state 0) release their blocks through the `__cdecl` release @ 0x00BFEB1C and null them (inline)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E850, oracle length 128 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esp+10h],1 ; call Release ; mov eax,[esi] ; lea ecx,[eax+74h] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; mov eax,[esi+14h] ; test eax,eax ; mov byte ptr [esp+10h],0 ; je skip1 ; push eax ; call Release_00bfeb1c ; add esp,4 ; mov dword ptr [esi+14h],0 ; skip1: mov eax,[esi+8] ; test eax,eax ; mov dword ptr [esp+10h],-1 ; je skip2 ; push eax ; call Release_00bfeb1c ; add esp,4 ; mov dword ptr [esi+8],0 ; skip2: mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// No vtable store (not polymorphic): the owner pointer is the first member. The block holders'
// inline destructors run last to first. The manifest label (`FinishWithDynamicTextures`) is
// propagated; the class is named by address.
void __cdecl Release_00bfeb1c(void* block);   // 0x00BFEB1C

struct CEngineScreenEffectOutlineGlow_00c1e850;
struct CRegistry_00c1e850 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CEngineScreenEffectOutlineGlow_00c1e850* effect);   // slot 8
};

struct COwner_00c1e850 {
    unsigned char _pad[0x74];
    CRegistry_00c1e850 m_registry;         // +0x74
};

struct CBlockHolder_00c1e850 {
    void* m_pBlock;
    ~CBlockHolder_00c1e850() { if (m_pBlock) { Release_00bfeb1c(m_pBlock); m_pBlock = 0; } }
};

struct CEngineScreenEffectOutlineGlow_00c1e850 {
    COwner_00c1e850* m_pOwner;             // +0
    long m_4;
    CBlockHolder_00c1e850 m_first;         // +8
    long m_c, m_10;
    CBlockHolder_00c1e850 m_second;        // +0x14
    void Release();                        // 0x00C1E800
    ~CEngineScreenEffectOutlineGlow_00c1e850();
};

CEngineScreenEffectOutlineGlow_00c1e850::~CEngineScreenEffectOutlineGlow_00c1e850()
{
    Release();
    m_pOwner->m_registry.Remove(this);
}