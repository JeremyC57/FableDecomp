// CEditControlCentre_00caff70::RemoveTrackNode(node) -- EH-framed: holds the embedded critical section at +8 through a scoped lock (state 0); the node's +0x30 flag selects the list at +0x38 (set) or +0x20 (clear); the node is searched by an inlined Find (head at +0, GetAt @ 0x00CB13A0, GetNext @ 0x00CB1380); 0x80040216 when absent, otherwise removed (@ 0x00CB13F0), its reference at +0xc released through the table entry at +8 with the member's address and the centre refreshed (@ 0x00CAF750); the 0 result lives in a frame slot and the error constant in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAFF70, oracle length 194 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,ecx ; lea ebp,[edi+8] ; push ebp ; mov [esp+1Ch],ebp ; call [__imp_EnterCriticalSection] ; mov ebx,[esp+2Ch] ; mov eax,[ebx+30h] ; xor ecx,ecx ; cmp eax,ecx ; mov [esp+24h],ecx ; mov [esp+14h],ecx ; lea esi,[edi+38h] ; jne pick ; lea esi,[edi+20h] ; pick: mov eax,[esi] ; cmp eax,ecx ; mov [esp+10h],eax ; je fail ; loop: push eax ; mov ecx,esi ; call GetAt ; cmp eax,ebx ; je found ; lea eax,[esp+10h] ; push eax ; mov ecx,esi ; call GetNext ; mov eax,[esp+10h] ; test eax,eax ; jne loop ; mov esi,80040216h ; jmp out ; found: mov eax,[esp+10h] ; test eax,eax ; jne remove ; fail: mov esi,80040216h ; jmp out ; remove: push eax ; mov ecx,esi ; call RemoveAt ; mov ecx,[ebx+0Ch] ; lea eax,[ebx+0Ch] ; push eax ; call [ecx+8] ; mov ecx,edi ; call Refresh ; mov esi,[esp+14h] ; out: push ebp ; call [__imp_LeaveCriticalSection] ; ... ret 4
// The manifest label is propagated; the classes are named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00caff70 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00caff70(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00caff70() { LeaveCriticalSection(m_pSection); }
};

struct ITrackRef_00caff70 {                // the node's owner reference: a table of entry points
    void* m_pfn0;
    void* m_pfn4;
    void (__stdcall* m_pfnRelease)(ITrackRef_00caff70** slot);   // +8
};

struct CTrackNode_00caff70 {
    unsigned char _pad[0xc];
    ITrackRef_00caff70* m_pRef;            // +0xc
    unsigned char _pad2[0x20];
    long m_bSecondary;                     // +0x30
};

struct CNodeList_00caff70 {                // 0x18-byte position list
    void* m_pNodeHead;                     // +0
    void* m_pNodeTail;                     // +4
    long m_nCount;                         // +8
    unsigned char _pad[0xc];
    void* GetHeadPosition() const { return m_pNodeHead; }
    void* GetAt(void* position);           // 0x00CB13A0
    void* GetNext(void*& position);        // 0x00CB1380
    void RemoveAt(void* position);         // 0x00CB13F0
    __forceinline void* Find(void* data, void*& position)
    {
        for (position = GetHeadPosition(); position; GetNext(position))
            if (GetAt(position) == data)
                return position;
        return 0;
    }
};

struct CEditControlCentre_00caff70 {
    unsigned char _pad[8];
    unsigned char m_section[0x18];         // +8
    CNodeList_00caff70 m_primary;          // +0x20
    CNodeList_00caff70 m_secondary;        // +0x38
    void Refresh();                        // 0x00CAF750
    long RemoveTrackNode(CTrackNode_00caff70* node);
};

long CEditControlCentre_00caff70::RemoveTrackNode(CTrackNode_00caff70* node)
{
    CSectionLock_00caff70 lock(m_section);
    long result = 0;
    CNodeList_00caff70* list = node->m_bSecondary ? &m_secondary : &m_primary;
    void* position;
    if (!list->Find(node, position)) {
        result = 0x80040216;
    } else {
        list->RemoveAt(position);
        node->m_pRef->m_pfnRelease(&node->m_pRef);
        Refresh();
    }
    return result;
}