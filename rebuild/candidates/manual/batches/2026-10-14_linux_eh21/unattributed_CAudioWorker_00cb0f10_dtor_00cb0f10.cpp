// CAudioWorker_00cb0f10::~CAudioWorker_00cb0f10() -- EH-framed destructor: the thread handle at +8 waited for (import, INFINITE) and closed, every node of the list at +0xc removed through a cursor (@ 0x00CB1380) and freed, the list cleared (@ 0x00CB1340), the event handle at +0x3c closed, then the critical-section member at +0x24 deleted by its inline destructor and the list member destroyed out of line (@ 0x00CB1890)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0F10, oracle length 172 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+10h],esi ; mov eax,[esi+8] ; test eax,eax ; mov ebx,[__imp_CloseHandle] ; mov dword ptr [esp+1Ch],1 ; je nothread ; push -1 ; push eax ; call [__imp_WaitForSingleObject] ; mov eax,[esi+8] ; push eax ; call ebx ; nothread: mov eax,[esi+0Ch] ; test eax,eax ; lea edi,[esi+0Ch] ; mov [esp+0Ch],eax ; je empty ; loop: lea ecx,[esp+0Ch] ; push ecx ; mov ecx,edi ; call CNodeList::Remove ; push eax ; call operator delete ; mov eax,[esp+10h] ; add esp,4 ; test eax,eax ; jne loop ; empty: mov ecx,edi ; call CNodeList::Clear ; mov eax,[esi+3Ch] ; test eax,eax ; je noevent ; push eax ; call ebx ; noevent: add esi,24h ; push esi ; call [__imp_DeleteCriticalSection] ; mov ecx,edi ; mov dword ptr [esp+1Ch],-1 ; call ~CNodeList ; ...
// Not polymorphic (no vtable store); the section wrapper's inline destructor registers no state
// change because the import cannot throw. The manifest label is propagated; the class is named by
// address.
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);   // IAT 0x0143FEAC
extern "C" __declspec(dllimport) long __stdcall CloseHandle(void* handle);                                               // IAT 0x0143FE30
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);                                   // IAT 0x0143FDE4

struct CNode_00cb0f10 { };

struct CNodeList_00cb0f10 {                // +0xc, 0x18 bytes
    CNode_00cb0f10* m_pHead;               // +0
    unsigned char _body[0x14];
    CNode_00cb0f10* Remove(CNode_00cb0f10** cursor);   // 0x00CB1380
    void Clear();                          // 0x00CB1340
    ~CNodeList_00cb0f10();                 // 0x00CB1890
};

struct CCriticalSection_00cb0f10 {         // +0x24, 0x18 bytes
    unsigned char m_section[0x18];
    ~CCriticalSection_00cb0f10() { DeleteCriticalSection(m_section); }
};

struct CAudioWorker_00cb0f10 {
    long m_0;
    long m_4;
    void* m_hThread;                       // +8
    CNodeList_00cb0f10 m_nodes;            // +0xc
    CCriticalSection_00cb0f10 m_lock;      // +0x24
    void* m_hEvent;                        // +0x3c
    ~CAudioWorker_00cb0f10();
};

CAudioWorker_00cb0f10::~CAudioWorker_00cb0f10()
{
    if (m_hThread) {
        WaitForSingleObject(m_hThread, 0xFFFFFFFF);
        CloseHandle(m_hThread);
    }
    CNode_00cb0f10* cursor = m_nodes.m_pHead;
    while (cursor)
        delete m_nodes.Remove(&cursor);
    m_nodes.Clear();
    if (m_hEvent)
        CloseHandle(m_hEvent);
}
