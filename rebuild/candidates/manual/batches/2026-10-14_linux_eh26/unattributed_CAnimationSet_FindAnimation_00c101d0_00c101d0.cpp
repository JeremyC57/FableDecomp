// CAnimationSet_00c101d0::FindAnimation(name, counter) -- EH-framed lookup: the name's hash (@ 0x00C0DBB0) keys the map embedded at +0x10 (slot 0x44, key passed by address); when an entry comes back its name (slot 8) is compared (inline `operator==` over references, @ 0x00C0BCE0) with an empty-string temporary whose construction is conditional (the flag byte at +0x10 of the frame guards its destructor, the result byte lives in the dead name-argument slot); an entry with an empty name that also matches the requested name bumps the optional counter; the entry (or 0) is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C101D0, oracle length 214 bytes. Built under /EHsc.
// Retail bytes:  ... mov ebp,[esp+24h] ; push esi ; mov esi,ecx ; xor ebx,ebx ; mov ecx,ebp ; mov [esp+10h],ebx ; call CString::Hash ; lea ecx,[esi+10h] ; lea edx,[esp+0Ch] ; mov [esp+0Ch],eax ; mov eax,[ecx] ; push edx ; call [eax+44h] ; mov esi,eax ; test esi,esi ; je none ; push edi ; push offset "" ; lea ecx,[esp+1Ch] ; call CString::CString ; mov edi,eax ; mov eax,[esi] ; mov [esp+24h],ebx ; mov ebx,1 ; mov ecx,esi ; mov [esp+14h],ebx ; call [eax+8] ; mov edi,[edi] ; push edi ; mov ecx,eax ; call CString::Equals ; test eax,eax ; pop edi ; mov [esp+28h],bl ; jne have ; none: mov byte ptr [esp+28h],0 ; have: test bl,1 ; mov dword ptr [esp+20h],-1 ; je skip ; lea ecx,[esp+14h] ; call ~CString ; skip: mov al,[esp+28h] ; test al,al ; je done ; mov edx,[esi] ; mov ecx,esi ; call [edx+8] ; mov ecx,[ebp] ; push ecx ; mov ecx,eax ; call CString::Equals ; test eax,eax ; je done ; mov eax,[esp+2Ch] ; test eax,eax ; je done ; inc dword ptr [eax] ; done: ... mov eax,esi ; ... ret 8
// The classes are named by address; the string is the MFC-style CString of this module.
struct CString_00c101d0 {                  // one word: the character data
    char* m_pchData;
    CString_00c101d0(const char* text);    // 0x00C0BFE0
    ~CString_00c101d0();                   // 0x00C0BA40
    long Hash() const;                     // 0x00C0DBB0
    long Equals(const char* text) const;   // 0x00C0BCE0
};
inline bool operator==(const CString_00c101d0& a, const CString_00c101d0& b) { return a.Equals(b.m_pchData) != 0; }

struct CAnimEntry_00c101d0 {
    virtual void Slot0();
    virtual void Slot4();
    virtual CString_00c101d0* GetName();   // slot 8
};

struct CAnimMap_00c101d0 {                 // embedded at +0x10
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1C();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2C();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3C();
    virtual void Slot40();
    virtual CAnimEntry_00c101d0* Lookup(long* key);   // slot 0x44
};

struct CAnimationSet_00c101d0 {
    unsigned char _pad[0x10];
    CAnimMap_00c101d0 m_map;               // +0x10
    CAnimEntry_00c101d0* FindAnimation(const CString_00c101d0& name, long* counter);
};

CAnimEntry_00c101d0* CAnimationSet_00c101d0::FindAnimation(const CString_00c101d0& name, long* counter)
{
    long key = name.Hash();
    CAnimEntry_00c101d0* entry = m_map.Lookup(&key);
    bool unnamed = entry && *entry->GetName() == CString_00c101d0("");
    if (unnamed) {
        if (*entry->GetName() == name) {
            if (counter)
                ++*counter;
        }
    }
    return entry;
}
