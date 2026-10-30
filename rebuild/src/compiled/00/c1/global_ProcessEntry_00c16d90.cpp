// _global::ProcessEntry(const SProcessorSettings_00c16ce0* settings, CBankFile* source, CScriptHost* host, CLoadLog* log) -- fills a local CBankData from `source` through Fill 0x00C30F60 (with the settings record's +8 value and the log), and when the log is still ok runs the bank processor 0x00C16CE0 over it with the settings
// Shape: one TU with the bank loader RunProcessor 0x00C16CE0 as a same-TU `static __declspec(noinline) __stdcall` (VC7.1 gives it the private
//   convention retail has: host in ecx, data in edx, the log on the stack, `ret 4`, and the settings record in esi) and the landed
//   CAutoDeletePointer::GetChecked 0x00C17740 body (its mod-ref lets the cleanup reuse edi without a reload or null test); the
//   GetChecked result is taken into a local before the call so the log is pushed after it, as retail does.
// Leaf: ProcessEntry
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17400, length 155 bytes (+ int3 padding), `__fastcall` `ret 8`, EH-framed (the holder is state 0).
// Retail: RunProcessor @ 0x00C16CE0, 166 bytes, EH-framed (the local CBankProcessor), `ret 4`, reads esi as its fourth argument.
// Retail: ProcessEntry @ 0x00C16D90, 122 bytes, EH-framed (the local CBankData), settings in ecx, the log in edi (never set or saved),
//   `ret 8`; caller 0x00C17572 in the bank loader 0x00C174A0 (stood in below by LoadLugBank, not graded). Callees: Fill @ 0x00C30F60,
//   CBankData::CBankData @ 0x00C2EED0, CLoadLog::IsOk @ 0x00C307C0.
// Callees: LookupSelectedWaypoint @ 0x00C17940 (`__fastcall`, context in ecx, log in edx), 0x00C30840 (landed as
//   CInstanceOwner_00c30840::CreateNewInstance -- the same CLoadLog: see NAME_FIXES), CDebugStream::operator<< @ 0x00C0DEC0 (the spelling the landed GetChecked uses; the entry the log hands back is the stream: {buffer, id}; message
//   @ 0x012B2D44 "Could not load from file"), CAutoDeletePointer::GetChecked @ 0x00C17740, CBankData::~CBankData @ 0x00C2EF30,
//   operator delete @ 0x00BFE9BC; in RunProcessor: CBankProcessor::CBankProcessor @ 0x00C2E0C0, Load @ 0x00C2EB00, ~CBankProcessor @ 0x00C2E140.
// Identity: libpk sound-bank code -- the callees are the landed CBankProcessor/CBankData/CLoadLog rows; the catalog's track-node labels
//   are propagated (kept here as parked; the descriptive names are open, see NOTES).
struct CBankData {
    CBankData();                                           // 0x00C2EED0
    ~CBankData();                                          // 0x00C2EF30
    unsigned char _data[0x30];
};
class CScriptHost;

struct SInstanceEntry;
class CLoadLog {
public:
    bool IsOk() const;                                     // 0x00C307C0
    SInstanceEntry* CreateNewInstance();                   // 0x00C30840
};
extern const char g_szCouldNotLoadFromFile[];             // 0x012B2D44

struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct CAutoDeletePointer {
    void* m_pObject;
    CAutoDeletePointer(void* object) : m_pObject(object) { }
    ~CAutoDeletePointer() { delete (CBankData*)m_pObject; }
    __declspec(noinline) void* GetChecked();               // 0x00C17740
};

void* CAutoDeletePointer::GetChecked()
{
    if (m_pObject == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pObject;
}

struct SProcessorSettings_00c16ce0 {
    unsigned long m_0;
    unsigned long m_4;
    unsigned long m_8;
    unsigned char m_c, m_d, m_e, m_f, m_10, m_11;
};

class CBankProcessor {
public:
    unsigned char _pad[0x6c];
    unsigned long m_6c;
    unsigned long m_70;
    unsigned char m_74, m_75, m_76, m_77, m_78, m_79, m_7a, m_7b;
    CBankProcessor(CBankData* data, CScriptHost* manager, CLoadLog* log);   // 0x00C2E0C0
    ~CBankProcessor();                                                      // 0x00C2E140
    void Load();                                                            // 0x00C2EB00
};

static __declspec(noinline) void __stdcall RunProcessor(CScriptHost* host, CBankData* data, CLoadLog* log, const SProcessorSettings_00c16ce0* settings)
{
    CBankProcessor processor(data, host, log);
    processor.m_6c = settings->m_0;
    processor.m_70 = settings->m_4;
    processor.m_79 = settings->m_c;
    processor.m_7a = settings->m_d;
    processor.m_76 = settings->m_e;
    processor.m_74 = settings->m_f;
    processor.m_75 = settings->m_10;
    if (settings->m_11) {
        processor.m_77 = 1;
        processor.m_78 = 1;
    }
    processor.Load();
}

struct CScriptContext;
CBankData* __fastcall LookupSelectedWaypoint(CScriptContext* context, CLoadLog* log);   // 0x00C17940

void __fastcall DeleteTrackNodeCommand(CScriptContext* context, CLoadLog* log, CScriptHost* host, const SProcessorSettings_00c16ce0* settings)
{
    CAutoDeletePointer holder(LookupSelectedWaypoint(context, log));
    if (holder.m_pObject == 0) {
        *(struct CDebugStream*)log->CreateNewInstance() << g_szCouldNotLoadFromFile;
        return;
    }
    CBankData* data = (CBankData*)holder.GetChecked();
    RunProcessor(host, data, log, settings);
}

class CBankFile;
// Fill @ 0x00C30F60 (`__fastcall`: source in ecx, data in edx, then value and the log; `ret 8`).
void __fastcall FillEntry(CBankFile* source, CBankData* data, unsigned long value, CLoadLog* log);   // 0x00C30F60

static __declspec(noinline) void __stdcall ProcessEntry(const SProcessorSettings_00c16ce0* settings, CBankFile* source, CScriptHost* host, CLoadLog* log)
{
    CBankData data;
    FillEntry(source, &data, settings->m_8, log);
    if (log->IsOk())
        RunProcessor(host, &data, log, settings);
}

// Same-TU caller (stands in for the bank loader 0x00C174A0's .lug branch). Not graded.
void __fastcall LoadLugBank(CAutoDeletePointer* file, CLoadLog* log, CScriptHost* host, const SProcessorSettings_00c16ce0* settings)
{
    ProcessEntry(settings, (CBankFile*)file->GetChecked(), host, log);
}