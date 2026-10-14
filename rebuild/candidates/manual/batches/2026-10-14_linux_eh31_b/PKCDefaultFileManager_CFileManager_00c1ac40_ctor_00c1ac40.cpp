// CFileManager_00c1ac40::CFileManager(owner) -- EH-framed constructor mirroring the landed destructor 0x00C1A230 (vtable 0x012B30E4; the manifest's "FileManager_CreateHandler"): the +4 handle table (@ 0x00C1C810; state 1), the vptr-only sink at +0x1c (vtable 0x012B30EC) handed to the +0x20 dispatcher (@ 0x00C1AE20), the request queue at +0x44 (root @ 0x00C1B340 with capacity 0x28, its address homed on the frame for the root unwind @ 0x00C1AEA0 (state 2), the node @ 0x00C1AFD0 over the root; state 3), the worker event at +0x78 (@ 0x00C29FC0 with (0, 1); state 4), +0x80 cleared and the owner at +0x84; the body creates the worker thread (`CreateThread`, 0x3000-byte stack, procedure 0x00C1AC30 over `this`) into +0x7c, raising the "Could not create thread" warning (PKCDefaultFileManager.cpp line 234, log sink at 0x0143BB2C, the trace temporary on the string's slot; state 5) when it fails, and sleeps 20 ms
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AC40, oracle length 337 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; lea ecx,[esi+4] ; mov dword ptr [esp+18h],0 ; mov dword ptr [esi],12B30E4h ; call CHandleTable::CHandleTable ; lea eax,[esi+1Ch] ; push eax ; lea ecx,[esi+20h] ; mov byte ptr [esp+1Ch],1 ; mov dword ptr [eax],12B30ECh ; call CDispatcher::CDispatcher ; lea edi,[esi+44h] ; push 28h ; mov ecx,edi ; mov [esp+10h],edi ; call CQueueRoot::CQueueRoot ; push 28h ; push edi ; lea ecx,[edi+14h] ; mov byte ptr [esp+20h],2 ; call CQueueNode::CQueueNode ; push 1 ; push 0 ; lea ecx,[esi+78h] ; mov byte ptr [esp+20h],3 ; call CEvent::CEvent ; mov eax,[esp+20h] ; push 0 ; push 0 ; push esi ; push offset ThreadProc ; push 3000h ; push 0 ; mov byte ptr [esp+30h],4 ; mov dword ptr [esi+80h],0 ; mov [esi+84h],eax ; call [__imp_CreateThread] ; test eax,eax ; mov [esi+7Ch],eax ; jne ok ; mov ecx,[g_pchNil] ; mov [esp+20h],ecx ; mov edx,offset ".\PKCDefaultFileManager.cpp" ; lea ecx,[esp+20h] ; mov byte ptr [esp+18h],5 ; call Append ; ... mov edx,0EAh ; ... call AppendNumber ; ... call Append ("Could not create thread") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+20h] ; call eax ; skip: lea ecx,[esp+20h] ; mov byte ptr [esp+18h],4 ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; ok: push 14h ; call [__imp_Sleep] ; ... mov eax,esi ; ... ret 4
// The classes are named by address, matching the destructor's model.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* attributes, unsigned long stackSize, unsigned long (__stdcall* procedure)(void*), void* parameter, unsigned long flags, unsigned long* threadId);
unsigned long __stdcall FileManagerThread_00c1ac30(void* manager);   // 0x00C1AC30
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c1ac40)(const char* text);
extern PFNLOGSINK_00c1ac40 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c1ac40 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c1ac40() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c1ac40();                       // 0x00C0BA40
    CString_00c1ac40* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c1ac40* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c1ac40 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c1ac40();                         // 0x00C16750
};

struct CHandleTable_00c1ac40 { unsigned char _body[0x18]; CHandleTable_00c1ac40(); ~CHandleTable_00c1ac40(); };   // +4, 0x00C1C810 / 0x00C1C820
struct CSink_00c1ac40 { virtual void v00(); };   // +0x1c, vtable 0x012B30EC
struct CDispatcher_00c1ac40 { unsigned char _body[0x24]; CDispatcher_00c1ac40(CSink_00c1ac40* sink); };   // +0x20, 0x00C1AE20
struct CQueueRoot_00c1ac40 { unsigned char _body[0x14]; CQueueRoot_00c1ac40(long capacity); ~CQueueRoot_00c1ac40(); };   // 0x00C1B340 / 0x00C1AEA0
struct CQueueNode_00c1ac40 { unsigned char _body[0x20]; CQueueNode_00c1ac40(CQueueRoot_00c1ac40* root, long capacity); };   // 0x00C1AFD0
struct CRequestQueue_00c1ac40 {            // +0x44: root and node, destroyed as a whole @ 0x00C1B2A0
    CQueueRoot_00c1ac40 m_root;
    CQueueNode_00c1ac40 m_node;
    CRequestQueue_00c1ac40() : m_root(0x28), m_node(&m_root, 0x28) { }
    ~CRequestQueue_00c1ac40();
};
struct CEvent_00c1ac40 { void* m_hEvent; CEvent_00c1ac40(long manual, long initial); ~CEvent_00c1ac40(); };   // +0x78, 0x00C29FC0 / 0x00C29FB0

struct CFileManagerBase_00c1ac40 {         // vtable 0x012B2FB8
    virtual ~CFileManagerBase_00c1ac40() { }
};

struct CFileManager_00c1ac40 : CFileManagerBase_00c1ac40 {
    CHandleTable_00c1ac40 m_handles;       // +4
    CSink_00c1ac40 m_sink;                 // +0x1c
    CDispatcher_00c1ac40 m_dispatcher;     // +0x20
    CRequestQueue_00c1ac40 m_queue;        // +0x44
    CEvent_00c1ac40 m_event;               // +0x78
    void* m_hThread;                       // +0x7c
    long m_80;
    void* m_pOwner;                        // +0x84
    CFileManager_00c1ac40(void* owner);
    virtual ~CFileManager_00c1ac40();
};

CFileManager_00c1ac40::CFileManager_00c1ac40(void* owner)
    : m_handles(), m_sink(), m_dispatcher(&m_sink), m_queue(), m_event(0, 1), m_80(0), m_pOwner(owner)
{
    m_hThread = CreateThread(0, 0x3000, FileManagerThread_00c1ac30, this, 0, 0);
    if (!m_hThread) {
        {
            CString_00c1ac40 line;
            line.Append(".\\PKCDefaultFileManager.cpp")->Append("(")->AppendNumber(234)->Append(") : ")->Append("Could not create thread")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c1ac40();
    }
    Sleep(20);
}
