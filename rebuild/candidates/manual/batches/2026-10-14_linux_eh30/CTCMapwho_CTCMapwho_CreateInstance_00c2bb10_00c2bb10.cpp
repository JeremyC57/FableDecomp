// CTCMapwho_00c2bb10::CreateInstance(owner, key, c, d, position, fa, fb) -- EH-framed (the manifest's "CTCMapwho_CreateInstance" is propagated): a two-word find key (owner, c) on the frame (@ 0x00C38330, empty out-of-line destructor @ 0x00C38350; state 0) is resolved through slot 17 of the registry at +0x34 and any existing entry destroyed (@ 0x00C2BAF0: unlink @ 0x00C37DA0 then `operator delete`); a frame position (@ 0x00C20390 zeroes three words and a flag byte) takes the caller's vector with the flag cleared, and a 0x60-byte entry is allocated (null-checked `new`, state 1 covers the raw block) and built from (this, owner, key, c, d, &position, fa, fb) (the landed 0x00C37C90, which registers it with the owner); the key is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2BB10, oracle length 244 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,18h ; push ebp ; mov ebp,[esp+2Ch] ; push esi ; push edi ; mov edi,[esp+3Ch] ; push edi ; mov esi,ecx ; push ebp ; lea ecx,[esp+14h] ; call CFindKey::CFindKey ; mov eax,[esi+34h] ; lea ecx,[esi+34h] ; lea edx,[esp+0Ch] ; push edx ; mov dword ptr [esp+30h],0 ; call [eax+44h] ; test eax,eax ; je none ; push eax ; mov ecx,esi ; call CTCMapwho::Destroy ; none: lea ecx,[esp+0Ch] ; mov dword ptr [esp+2Ch],-1 ; call CFindKey::~CFindKey ; lea ecx,[esp+14h] ; call CPosition::CPosition ; mov eax,[esp+44h] ; mov ecx,[eax] ; mov edx,[eax+4] ; mov eax,[eax+8] ; push 60h ; mov byte ptr [esp+24h],0 ; mov [esp+18h],ecx ; mov [esp+1Ch],edx ; mov [esp+20h],eax ; call operator new ; add esp,4 ; mov [esp+3Ch],eax ; test eax,eax ; mov dword ptr [esp+2Ch],1 ; je null ; mov ecx,[esp+4Ch] ; mov edx,[esp+48h] ; push ecx ; push edx ; mov edx,[esp+48h] ; lea ecx,[esp+1Ch] ; push ecx ; push edx ; push edi ; mov edi,[esp+4Ch] ; push edi ; push ebp ; push esi ; mov ecx,eax ; call CEntry::CEntry ; mov eax,edi ; ... ret 1Ch ; null: ... mov eax,[esp+38h] ; ... ret 1Ch
// The classes are named by address.
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* block);
struct CTCMapwho_00c2bb10;

struct CVector3_00c2bb10 { float x, y, z; };

struct CPosition_00c2bb10 {                // three coordinates and a flag byte
    float x, y, z;
    bool m_bFlag;
    CPosition_00c2bb10();                  // 0x00C20390: zeroes everything
    CPosition_00c2bb10& operator=(const CVector3_00c2bb10& v) { m_bFlag = false; x = v.x; y = v.y; z = v.z; return *this; }
};

struct CFindKey_00c2bb10 {                 // two words: owner and the +8 qualifier
    void* m_pOwner;
    void* m_pQualifier;
    CFindKey_00c2bb10(void* owner, void* qualifier);   // 0x00C38330
    ~CFindKey_00c2bb10();                  // 0x00C38350 (empty, out of line)
};

struct CEntry_00c2bb10 {                   // 0x60 bytes: the landed opinion-state entry
    unsigned char _body[0x60];
    CEntry_00c2bb10(CTCMapwho_00c2bb10* owner, void* a, void* key, void* c, void* d, const CPosition_00c2bb10* position, long fa, long fb);   // 0x00C37C90
    void Unlink();                         // 0x00C37DA0
};

struct CRegistry_00c2bb10 {                // +0x34: slot 17 finds an entry by key
    virtual void Slot00(); virtual void Slot04(); virtual void Slot08(); virtual void Slot0C();
    virtual void Slot10(); virtual void Slot14(); virtual void Slot18(); virtual void Slot1C();
    virtual void Slot20(); virtual void Slot24(); virtual void Slot28(); virtual void Slot2C();
    virtual void Slot30(); virtual void Slot34(); virtual void Slot38(); virtual void Slot3C();
    virtual void Slot40();
    virtual CEntry_00c2bb10* Find(const CFindKey_00c2bb10* key);   // slot 0x44
};

struct CTCMapwho_00c2bb10 {
    unsigned char _pad[0x34];
    CRegistry_00c2bb10 m_registry;         // +0x34
    void Destroy(CEntry_00c2bb10* entry);  // 0x00C2BAF0
    void* CreateInstance(void* owner, void* key, void* c, void* d, const CVector3_00c2bb10* position, long fa, long fb);
};

void* CTCMapwho_00c2bb10::CreateInstance(void* owner, void* key, void* c, void* d, const CVector3_00c2bb10* position, long fa, long fb)
{
    {
        CFindKey_00c2bb10 find(owner, c);
        CEntry_00c2bb10* existing = m_registry.Find(&find);
        if (existing)
            Destroy(existing);
    }
    CPosition_00c2bb10 pos;
    pos = *position;
    new CEntry_00c2bb10(this, owner, key, c, d, &pos, fa, fb);
    return key;
}
