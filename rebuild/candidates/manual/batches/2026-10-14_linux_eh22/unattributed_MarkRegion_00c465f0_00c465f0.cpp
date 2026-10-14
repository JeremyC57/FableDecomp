// CRegionTracker_00c465f0::MarkRegion(const SRegionKey*, long) -- EH-framed: a 12-byte key (the two words of the argument and its computed height @ 0x00C46E90) looked up in the table at +4 (virtual slot 17); when absent a 16-byte node is allocated (`new`, result homed in the dead argument slot, state 0) and built inline (flag 0, the two words, the height) and inserted (virtual slot 9); when the second argument is set the node's flag becomes 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C465F0, oracle length 186 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,[esp+24h] ; mov eax,[esi] ; mov ebx,ecx ; mov ecx,[esi+4] ; mov [esp+0Ch],ecx ; push edi ; mov ecx,esi ; mov [esp+0Ch],eax ; call SRegionKey::Height ; fstp [esp+14h] ; mov ecx,[ebx+4] ; mov edx,[ecx] ; lea eax,[esp+0Ch] ; push eax ; call [edx+44h] ; mov edi,eax ; test edi,edi ; jne have ; push 10h ; call operator new ; mov edi,eax ; add esp,4 ; mov [esp+28h],edi ; test edi,edi ; mov dword ptr [esp+20h],0 ; je null ; mov dword ptr [edi],0 ; mov ecx,[esi] ; mov [edi+4],ecx ; mov edx,[esi+4] ; mov ecx,esi ; mov [edi+8],edx ; call SRegionKey::Height ; fstp [edi+0Ch] ; jmp ins ; null: xor edi,edi ; ins: mov ecx,[ebx+4] ; mov eax,[ecx] ; push edi ; mov dword ptr [esp+24h],-1 ; call [eax+24h] ; have: mov eax,[esp+2Ch] ; test eax,eax ; je done ; mov dword ptr [edi],1 ; done: ...
// The lookup key is a stack local of the same three words; the node's inline constructor takes
// the key. The manifest label is a description; the classes are named by address.
struct SRegionKey_00c465f0 {
    long m_x;
    long m_y;
    float Height() const;                  // 0x00C46E90
};

struct SLookupKey_00c465f0 {
    long m_x;
    long m_y;
    float m_height;
    SLookupKey_00c465f0(const SRegionKey_00c465f0* key) : m_x(key->m_x), m_y(key->m_y), m_height(key->Height()) { }
};

struct CRegionNode_00c465f0 {              // 16 bytes
    long m_flag;
    long m_x;
    long m_y;
    float m_height;
    CRegionNode_00c465f0(const SRegionKey_00c465f0* key) : m_flag(0), m_x(key->m_x), m_y(key->m_y), m_height(key->Height()) { }
};

struct CRegionTable_00c465f0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Insert(CRegionNode_00c465f0* node);   // slot 9
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual CRegionNode_00c465f0* Find(const SLookupKey_00c465f0* key);   // slot 17
};

struct CRegionTracker_00c465f0 {
    long m_0;
    CRegionTable_00c465f0* m_pTable;       // +4
    void MarkRegion(const SRegionKey_00c465f0* key, long visited);
};

void CRegionTracker_00c465f0::MarkRegion(const SRegionKey_00c465f0* key, long visited)
{
    SLookupKey_00c465f0 lookup(key);
    CRegionNode_00c465f0* node = m_pTable->Find(&lookup);
    if (!node) {
        node = new CRegionNode_00c465f0(key);
        m_pTable->Insert(node);
    }
    if (visited)
        node->m_flag = 1;
}
