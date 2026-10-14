// CTeleportDataList_00c2dbe0::~CTeleportDataList_00c2dbe0() -- EH-framed destructor: every item the container member at +0x80 still yields (virtual slot 11) is removed (virtual slot 8) and deleted (its string at +8 released @ 0x00C0BA40, then the global operator delete, null-checked), the container destroyed inline (vtable 0x012B47D0, node at +8 @ 0x00C19250 at state 2, root vtable 0x012B1AB0), then the out-of-line base destructor (@ 0x00C3A9D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2DBE0, oracle length 168 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push esi ; mov ebx,ecx ; push edi ; mov [esp+0Ch],ebx ; mov eax,[ebx+80h] ; lea esi,[ebx+80h] ; mov ecx,esi ; mov dword ptr [esp+1Ch],1 ; call [eax+2Ch] ; mov edi,eax ; test edi,edi ; je done ; loop: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+20h] ; test edi,edi ; je next ; lea ecx,[edi+8] ; call ~CString ; push edi ; call operator delete ; add esp,4 ; next: mov eax,[esi] ; mov ecx,esi ; call [eax+2Ch] ; mov edi,eax ; test edi,edi ; jne loop ; done: mov [esp+10h],esi ; mov dword ptr [esi],offset vtable ; lea ecx,[esi+8] ; mov byte ptr [esp+1Ch],2 ; call ~CNode ; mov ecx,ebx ; mov dword ptr [esi],offset root ; mov dword ptr [esp+1Ch],-1 ; call ~CBase ; ...
// The most-derived class stores no vtable (`__declspec(novtable)`); the body runs at state 1
// because a member before the container has a non-trivial but empty destructor (state 0). The
// manifest label is propagated; the classes are named by address.
struct CTeleportBase_00c2dbe0 {            // 0x7c bytes
    virtual void v00();
    unsigned char _body[0x78];
    ~CTeleportBase_00c2dbe0();             // 0x00C3A9D0
};

struct CMarker_00c2dbe0 {                  // +0x7c: empty but non-trivial destructor (owns EH state 0)
    long m_value;
    ~CMarker_00c2dbe0() { }
};

struct CString_00c2dbe0 {
    void* m_pData;
    ~CString_00c2dbe0();                   // 0x00C0BA40
};

struct CItem_00c2dbe0 {
    long m_key;
    long m_4;
    CString_00c2dbe0 m_name;               // +8
};

struct CNode_00c2dbe0 {
    unsigned char _body[0x28];
    ~CNode_00c2dbe0();                     // 0x00C19250
};

struct CContainerRoot_00c2dbe0 {           // vtable 0x012B1AB0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CItem_00c2dbe0* item);   // slot 8
    virtual void v09(); virtual void v10();
    virtual CItem_00c2dbe0* Next();        // slot 11
    CNode_00c2dbe0* m_pNode;               // +4
    ~CContainerRoot_00c2dbe0() { }
};

struct CContainer_00c2dbe0 : CContainerRoot_00c2dbe0 {   // vtable 0x012B47D0
    CNode_00c2dbe0 m_node;                 // +8
    ~CContainer_00c2dbe0() { }
};

struct __declspec(novtable) CTeleportDataList_00c2dbe0 : CTeleportBase_00c2dbe0 {
    CMarker_00c2dbe0 m_marker;             // +0x7c
    CContainer_00c2dbe0 m_items;           // +0x80
    ~CTeleportDataList_00c2dbe0();
};

CTeleportDataList_00c2dbe0::~CTeleportDataList_00c2dbe0()
{
    CItem_00c2dbe0* item = m_items.Next();
    while (item) {
        m_items.Remove(item);
        delete item;
        item = m_items.Next();
    }
}
