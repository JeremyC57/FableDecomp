// CLineCountedMemoryDataOutputStream_00c2be10::~CLineCountedMemoryDataOutputStream_00c2be10() -- EH-framed destructor: the two container members at +0x34 (vtable 0x012B4510) and +4 (vtable 0x012B45E8) destroyed in reverse order with their destructors inlined (each: own vtable, map member at +8 destroyed out of line @ 0x00C19250 at states 1 and 2, then the root vtable 0x012B1AB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2BE10, oracle length 124 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push edi ; mov edi,ecx ; mov [esp+8],edi ; lea esi,[edi+34h] ; mov dword ptr [esp+18h],0 ; mov [esp+0Ch],esi ; mov dword ptr [esi],offset vtableB ; lea ecx,[esi+8] ; mov byte ptr [esp+18h],1 ; call ~CMap ; mov dword ptr [esi],offset root ; lea esi,[edi+4] ; mov [esp+0Ch],esi ; mov dword ptr [esi],offset vtableA ; lea ecx,[esi+8] ; mov dword ptr [esp+18h],2 ; call ~CMap ; mov ecx,[esp+10h] ; mov dword ptr [esi],offset root ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The owner stores no vtable (not polymorphic); each inlined member destructor parks the member's
// address in the second frame slot for the unwind funclet. The manifest label is propagated; the
// classes are named by address.
struct CItem_00c2be10 { };

struct CContainerBase_00c2be10 {             // vtable 0x012B1AB0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CItem_00c2be10* item);   // slot 8
    virtual void v09(); virtual void v10();
    virtual CItem_00c2be10* Next();            // slot 11
    long m_count;                          // +4
    ~CContainerBase_00c2be10() { }
};

struct CMap_00c2be10 {
    unsigned char _body[0x28];
    ~CMap_00c2be10();                          // 0x00C19250
};

struct CContainerA_00c2be10 : CContainerBase_00c2be10 {   // vtable 0x012B45E8
    CMap_00c2be10 m_map;                   // +8
    ~CContainerA_00c2be10() { }
};

struct CContainerB_00c2be10 : CContainerBase_00c2be10 {   // vtable 0x012B4510
    CMap_00c2be10 m_map;                   // +8
    ~CContainerB_00c2be10() { }
};

struct CLineCountedMemoryDataOutputStream_00c2be10 {
    unsigned char _pad[4];
    CContainerA_00c2be10 m_first;          // +4
    CContainerB_00c2be10 m_second;         // +0x34
    ~CLineCountedMemoryDataOutputStream_00c2be10();
};

CLineCountedMemoryDataOutputStream_00c2be10::~CLineCountedMemoryDataOutputStream_00c2be10()
{
}