// CFileManager_00c1a230::~CFileManager() -- EH-framed destructor (vtable 0x012B30E4; the manifest's "FileManager_ThreadShutdown"): a quit request (seven words, the key -1) is submitted to the request queue at +0x58 (@ 0x00C1B130) until it is taken (result 1 = retry), each retry sleeping 100 ms and raising the "Wait to send quit command..." warning (PKCDefaultFileManager.cpp line 247: an MFC-style string in a nested block handed to the log sink at 0x0143BB2C; state 4 covers it, no trace); the worker's event at +0x78 is waited on (@ 0x00C2A090) and the thread handle at +0x7c closed; the compiler then destroys the event (@ 0x00C29FB0; state 2), the +0x44 table (@ 0x00C1B2A0; state 1), the +4 handle table (@ 0x00C1C820; state 0) and stores the base vtable 0x012B2FB8 (inline empty base destructor, 0x00C19F60 on unwind)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1A230, oracle length 310 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,24h ; push ebp ; push esi ; push edi ; mov edi,ecx ; mov [esp+10h],edi ; mov dword ptr [edi],12B30E4h ; lea eax,[esp+14h] ; lea ebp,[edi+58h] ; push eax ; mov ecx,ebp ; mov dword ptr [esp+3Ch],3 ; mov dword ptr [esp+1Ch],-1 ; call CRequestQueue::Submit ; cmp eax,1 ; jne done ; retry: mov esi,[__imp_Sleep] ; push 64h ; call esi ; mov ecx,[g_pchNil] ; mov [esp+0Ch],ecx ; mov edx,offset ".\PKCDefaultFileManager.cpp" ; lea ecx,[esp+0Ch] ; mov byte ptr [esp+38h],4 ; call Append ; ... mov edx,0F7h ; ... call AppendNumber ; ... call Append ("Wait to send quit command...") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; test eax,eax ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov byte ptr [esp+38h],3 ; call ~CString ; lea edx,[esp+14h] ; push edx ; mov ecx,ebp ; call CRequestQueue::Submit ; cmp eax,1 ; je retry ; done: lea esi,[edi+78h] ; mov ecx,esi ; call CEvent::Wait ; mov eax,[edi+7Ch] ; push eax ; call [__imp_CloseHandle] ; mov ecx,esi ; mov byte ptr [esp+38h],2 ; call CEvent::~CEvent ; lea ecx,[edi+44h] ; mov byte ptr [esp+38h],1 ; call CTable::~CTable ; lea ecx,[edi+4] ; mov byte ptr [esp+38h],0 ; call CHandleTable::~CHandleTable ; mov ecx,[esp+30h] ; mov dword ptr [edi],12B2FB8h ; ... ret
// The classes are named by address.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c1a230)(const char* text);
extern PFNLOGSINK_00c1a230 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c1a230 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c1a230() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c1a230();                       // 0x00C0BA40
    CString_00c1a230* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c1a230* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct SRequest_00c1a230 {                 // seven words: handle, key and five qualifiers
    void* m_hFile;
    void* m_pKey;
    long m_a;
    long m_b;
    long m_c;
    long m_d;
    long m_e;
};

struct CHandleTable_00c1a230 { unsigned char _body[0x40]; ~CHandleTable_00c1a230(); };   // +4, 0x00C1C820
struct CTable_00c1a230 { unsigned char _body[0x14]; ~CTable_00c1a230(); };               // +0x44, 0x00C1B2A0
struct CRequestQueue_00c1a230 {            // +0x58
    unsigned char _body[0x20];
    long Submit(const SRequest_00c1a230* request);   // 0x00C1B130: 1 when the queue must be retried
};
struct CEvent_00c1a230 {                   // +0x78: a Win32 event handle
    void* m_hEvent;
    void Wait();                           // 0x00C2A090
    ~CEvent_00c1a230();                    // 0x00C29FB0
};

struct CFileManagerBase_00c1a230 {         // vtable 0x012B2FB8
    virtual ~CFileManagerBase_00c1a230() { }
};

struct CFileManager_00c1a230 : CFileManagerBase_00c1a230 {
    CHandleTable_00c1a230 m_handles;       // +4
    CTable_00c1a230 m_table;               // +0x44
    CRequestQueue_00c1a230 m_queue;        // +0x58
    CEvent_00c1a230 m_event;               // +0x78
    void* m_hThread;                       // +0x7c
    virtual ~CFileManager_00c1a230();
};

CFileManager_00c1a230::~CFileManager_00c1a230()
{
    SRequest_00c1a230 quit;
    quit.m_pKey = (void*)-1;
    while (m_queue.Submit(&quit) == 1) {
        Sleep(100);
        {
            CString_00c1a230 line;
            line.Append(".\\PKCDefaultFileManager.cpp")->Append("(")->AppendNumber(247)->Append(") : ")->Append("Wait to send quit command...")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
    }
    m_event.Wait();
    CloseHandle(m_hThread);
}