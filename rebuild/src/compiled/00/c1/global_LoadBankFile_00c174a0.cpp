// LoadBankFile(CProcessor_00c09110* processor, CLoadLog* log, const unsigned short* name, CScriptHost* host, const SProcessorSettings_00c16ce0* settings) -- libpk bank load by file type: open the (wide) name through the processor's slot 0 into a PKCAutoFile (logging "Could not open <name>" when that fails), convert the name to ANSI and dispatch on its last four characters: .lug to ProcessEntry, .lut to the lookup-table loader 0x00C16F00, .met to the meta loader 0x00C17400, anything else logs "This bank is not a recognised format (.lug, .lut, .met)"
// Leaf: ?LoadBankFile@@YGXPAUCProcessor_00c09110@@PAVCLoadLog@@PBGPAVCScriptHost@@PBUSProcessorSettings_00c16ce0@@@Z
// Flags: /EHsc /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C174A0, 406 bytes, private convention (processor ecx, log edx, name esi, host ebx,
//   settings on the stack, `ret 4`), C++ EH frame (handler 0x01211E78: 0 = the PKCAutoFile, 1 = the ANSI name, 2 = its
//   extension) and a /GS cookie for the 256-byte ANSI buffer. Only caller 0x00C17640 (its stack CLoadLog in edx).
// Identity: libpk (no PDB, no RTTI): descriptive name; the catalog's "BankFile_LoadAndValidateFormat" is a propagated guess.
// Callees: processor slot 0, PKCAutoFile ctor 0x00C31D80 / dtor 0x00C31D90 / GetChecked 0x00C31DB0, CLoadLog::AddError
//   0x00C30840, CString::operator<<(const char*) 0x00C0DEC0 / (const void*) 0x00C0DF20, WideToMultiByte 0x00C16760,
//   CString(const char*) 0x00C0BFE0, Right 0x00C0E0F0, CompareNoCase 0x00C0BD60, ~CString 0x00C0BA40, ProcessEntry
//   0x00C16D90, 0x00C16F00, DeleteTrackNodeCommand 0x00C17400.
// Shape: a TU-local static with its same-TU caller 0x00C17640 below (not graded), so VC7.1 (plain /O2) picks the private
//   convention; the callees ProcessEntry / RunProcessor (landed text), the .lut loader 0x00C16F00 (written from the
//   retail bytes; with a stand-in body VC gives it another convention) and DeleteTrackNodeCommand 0x00C17400 are
//   same-TU statics too. The latter two take the log first: VC evaluates the arguments right to left, so the file
//   (GetChecked) is fetched before the log is pushed / moved to edx, as retail does (with the log second the move to
//   edx lands before the move to ecx). See NOTES: 0x00C17400 is landed as `__fastcall(context, log, ...)`, which
//   emits the same callee bytes.
void __fastcall WideToMultiByte(const unsigned short* source, char* dest, int destSize);   // 0x00C16760

class CString { public:                                   // libpk's MFC-style string (one word)
    char* m_pchData;
    CString(const char* text);                            // 0x00C0BFE0
    ~CString();                                           // 0x00C0BA40
    CString Right(int nCount) const;                      // 0x00C0E0F0
    int CompareNoCase(const char* lpsz) const;            // 0x00C0BD60
    CString& __fastcall operator<<(const char* text);     // 0x00C0DEC0
    CString& __fastcall operator<<(long value);           // 0x00C0DF00
    CString& __fastcall operator<<(const void* pointer);  // 0x00C0DF20
};
class CLoadLog {                                          // vtable 0x012B4CC0, 0x34 bytes
public:
    virtual void v00();
    unsigned char _pad04[0x2c];
    bool m_ok;                                            // +0x30
    CLoadLog();                                           // 0x00C308F0
    ~CLoadLog();                                          // 0x00C30940
    void SetSourceName(const char* name);                 // 0x00C308A0
    CString& AddError();                                  // 0x00C30840
    bool IsOk() const;                                    // 0x00C307C0
};
struct PKCAutoFile {                                      // libpk PKCAutoFile.cpp: owns an open file object
    void* m_pFile;                                        // +0
    PKCAutoFile(void* file);                              // 0x00C31D80
    ~PKCAutoFile();                                       // 0x00C31D90
    void* GetChecked();                                   // 0x00C31DB0
};
class CUnprovenPKObject_00c099f0;
struct CBankLoadDesc_00c08d40;
struct CProcessor_00c09110 {                              // the bank loader (landed Process_00c172f0 spelling)
    virtual void* Open(const unsigned short* fileName);   // slot 0: open the bank file by name
    bool __fastcall Load(const unsigned short* fileName, CUnprovenPKObject_00c099f0* bank, CBankLoadDesc_00c08d40* desc);   // 0x00C17640
};

struct CBankData {
    CBankData();                                          // 0x00C2EED0
    ~CBankData();                                         // 0x00C2EF30
    unsigned char _data[0x30];
};
class CScriptHost {
public:
    unsigned char _pad[0x48];
    long m_hasLookup;                                     // +0x48
    long m_highestPriority;                               // +0x4c
    void QueueEvent(long type, long time, long data);     // 0x00C0E9E0
};
class CBankFile;
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
void __fastcall FillEntry(CBankFile* source, CBankData* data, unsigned long value, CLoadLog* log);   // 0x00C30F60
struct CScriptContext;

// 0x00C16CE0 (landed text): same-TU static.
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
            CString s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32L << ") : " << "Should have checked first..." << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pchData);
        }
        CDebugBreak();
    }
    return m_pObject;
}
CBankData* __fastcall LookupSelectedWaypoint(CScriptContext* context, CLoadLog* log);   // 0x00C17940
extern const char g_szCouldNotLoadFromFile[];             // 0x012B2D44
static __declspec(noinline) void __stdcall DeleteTrackNodeCommand(CLoadLog* log, CScriptContext* context, CScriptHost* host, const SProcessorSettings_00c16ce0* settings)
{
    CAutoDeletePointer holder(LookupSelectedWaypoint(context, log));
    if (holder.m_pObject == 0) {
        log->AddError() << g_szCouldNotLoadFromFile;
        return;
    }
    CBankData* data = (CBankData*)holder.GetChecked();
    RunProcessor(host, data, log, settings);
}

// 0x00C16D90 (landed text): same-TU static (settings ecx, log edi).
static __declspec(noinline) void __stdcall ProcessEntry(const SProcessorSettings_00c16ce0* settings, CBankFile* source, CScriptHost* host, CLoadLog* log)
{
    CBankData data;
    FillEntry(source, &data, settings->m_8, log);
    if (log->IsOk())
        RunProcessor(host, &data, log, settings);
}

// 0x00C16F00: same-TU static (file edx, the rest on the stack, `ret 0Ch`): the .lut loader. Written from the retail
//   bytes so VC7.1 picks the same private convention; not graded.
struct CSegmentFile {                                     // the LiOnHeAd segment reader (0xB0 bytes)
    long          m_base[0x1f];
    long          m_errors;                               // +0x7C
    long          m_segments[0xc];                        // +0x80
    CSegmentFile(CBankFile* file, unsigned long chunk);   // 0x00C2DA20
    ~CSegmentFile();                                      // 0x00C2DBE0
    long OpenSegment(const CString& name);                // 0x00C2D9C0
    long Read(void* buffer, long size);                   // 0x00C2DA00
    unsigned long GetPosition();                          // 0x00C2DA10
};
long __fastcall ReadBankInfoCount(CSegmentFile* file);    // 0x00C16E10
// 0x00C16C10 (landed text; the caller tests the whole eax): same-TU static, file in esi.
static __declspec(noinline) long HasBankSegments(CSegmentFile* file)
{
    bool missing = file->OpenSegment(CString("LHAudioBankLookupTable")) < 0
                || file->OpenSegment(CString("LHAudioBankCompData")) < 0;
    return !missing;
}
struct SLookupEntry {
    long m_type;
    long m_data;
    long m_time;
};

static __declspec(noinline) void __stdcall LoadLookupTableBank(CLoadLog* log, CBankFile* file, CScriptHost* host, const SProcessorSettings_00c16ce0* settings)
{
    CSegmentFile reader(file, settings->m_8);
    if (reader.m_errors) {
        log->AddError() << "There were problems decoding the file segment structure";
        return;
    }
    long version = ReadBankInfoCount(&reader);
    if (version < 0) {
        log->AddError() << "Could not extract the version number";
        return;
    }
    if (!HasBankSegments(&reader)) {
        log->AddError() << "This bank is not a normal LUG!";
        return;
    }
    if (version != 4) {
        log->AddError() << "Expecting deferred bank version number " << 4L << "... got " << version;
        return;
    }
    bool missing = reader.OpenSegment(CString("LHAudioBankCompData")) < 0;
    if (missing) {
        log->AddError() << "Could not open the composite data segment";
        return;
    }
    unsigned long position = reader.GetPosition();
    missing = reader.OpenSegment(CString("LHAudioBankLookupTable")) < 0;
    if (missing) {
        log->AddError() << "Could not open the lookup table";
        return;
    }
    long highestPriority;
    if (reader.Read(&highestPriority, 4) < 4) {
        log->AddError() << "Could not read the highest priority";
        return;
    }
    unsigned long count;
    if (reader.Read(&count, 4) < 4) {
        log->AddError() << "Could not read the number of samples";
        return;
    }
    if (count == 0)
        return;
    SLookupEntry* entries = new SLookupEntry[count];
    if (reader.Read(entries, count * sizeof(SLookupEntry)) < (long)(count * sizeof(SLookupEntry))) {
        delete[] entries;
        log->AddError() << "Could not read in the lookup table";
        return;
    }
    for (unsigned long i = 0; i < count; ++i)
        host->QueueEvent(entries[i].m_type, entries[i].m_time + position, entries[i].m_data);
    host->m_highestPriority = highestPriority;
    host->m_hasLookup = 1;
    delete[] entries;
}

static __declspec(noinline) void __stdcall LoadBankFile(CProcessor_00c09110* processor, CLoadLog* log, const unsigned short* name, CScriptHost* host, const SProcessorSettings_00c16ce0* settings)
{
    PKCAutoFile file(processor->Open(name));
    if (!file.m_pFile) {
        log->AddError() << "Could not open " << (const void*)name;
        return;
    }
    char buffer[0x100];
    WideToMultiByte(name, buffer, 0x100);
    CString fileName(buffer);
    CString extension = fileName.Right(4);
    if (extension.CompareNoCase(".lug") == 0)
        ProcessEntry(settings, (CBankFile*)file.GetChecked(), host, log);
    else if (extension.CompareNoCase(".lut") == 0)
        LoadLookupTableBank(log, (CBankFile*)file.GetChecked(), host, settings);
    else if (extension.CompareNoCase(".met") == 0)
        DeleteTrackNodeCommand(log, (CScriptContext*)file.GetChecked(), host, settings);
    else
        log->AddError() << "This bank is not a recognised format (.lug, .lut, .met)";
}

// Same-TU caller 0x00C17640 (`__fastcall` member, `ret 8`; its stack CLoadLog goes in edx), the CSystem::LoadBank 0x00C08D40
//   spelling. Not graded. The bank and the load descriptor are the objects the landed ProcessEntry TU calls CScriptHost and
//   SProcessorSettings_00c16ce0 (see NOTES: one type, two landed names).
bool __fastcall CProcessor_00c09110::Load(const unsigned short* name, CUnprovenPKObject_00c099f0* bank, CBankLoadDesc_00c08d40* desc)
{
    CLoadLog log;
    LoadBankFile(this, &log, name, (CScriptHost*)bank, (const SProcessorSettings_00c16ce0*)desc);
    char buffer[0x100];
    WideToMultiByte(name, buffer, 0x100);
    log.SetSourceName(buffer);
    if (!log.IsOk())
        return false;
    return true;
}