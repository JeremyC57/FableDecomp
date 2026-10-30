// Process_00c172f0(CProcessor_00c09110* processor, void* name, void* context, CRequest_00c09110* request) -- libpk bank-file load: open the (wide) name through the processor's slot 0 into a PKCAutoFile, fail when no file; otherwise run the bank-data load into a stack CLoadLog, convert the name to ANSI for the log and return the log's status
// Leaf: ?Process_00c172f0@@YI_NPAUCProcessor_00c09110@@PAX1PAUCRequest_00c09110@@@Z
// Flags: /EHsc /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C172F0, 269 bytes, __fastcall (processor ecx, name edx, context and request on the
//   stack, `ret 8`), C++ EH frame (handler 0x01211E3B: state 0 = the PKCAutoFile, 1 = the CLoadLog) and a /GS cookie
//   (0x0139C8A8, __security_check_cookie 0x00BFE9F9) for the 256-byte ANSI name buffer.
// Identity: libpk (0xC08AE0..~0xC40000: no PDB, no RTTI), so the names are descriptive. The only caller is the landed
//   0x00C09110, which declares and binds this row as `bool __fastcall Process_00c172f0(CProcessor_00c09110*, void*,
//   void*, CRequest_00c09110*)`; that spelling is kept (one retail address, one name). The manifest label
//   "Setup_ProcessGraphicsAndInventory" is propagated.
// Callees: processor slot 0 (open by name), PKCAutoFile ctor 0x00C31D80 / dtor 0x00C31D90 / GetChecked 0x00C31DB0 (the
//   PKCAutoFile.cpp trio: `mov [ecx],arg`, `if (m_pFile) m_pFile->slot3()`, the ".\PKCAutoFile.cpp(26) : Should have
//   checked first..." accessor), CLoadLog ctor 0x00C308F0 / dtor 0x00C30940 / IsOk 0x00C307C0 (+0x30 status byte, set by
//   the ctor, cleared by AddError 0x00C30840), CLoadLog::SetSourceName 0x00C308A0, WideToMultiByte 0x00C16760 (size 0x100),
//   and the same-TU static LoadBank 0x00C17260.
// Shape: LoadBank is a TU-local static, so VC7.1 (plain /O2, no /GL) gives it a private convention: the log in esi
//   (`lea esi,[esp+14h]` before the call); `__stdcall` gives retail's callee-pop `ret 8`. Its body is written here (from
//   the retail bytes 0x00C17260: a CBankData 0x00C2EED0 / 0x00C2EF30 on the stack, ReadBankData 0x00C30F60 with chunk
//   0x2000, then the "There was a problem serialising the extracted meta data." error) so the register choice is VC's
//   own. Each `return` destroys the log and the file in place (two cleanup copies).
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int, unsigned long, const unsigned short*, int, char*, int, const char*, int*);
void __fastcall WideToMultiByte(const unsigned short* source, char* dest, int destSize);   // 0x00C16760

struct PKCAutoFile {                       // libpk PKCAutoFile.cpp: owns an open file object
    void* m_pFile;                         // +0
    PKCAutoFile(void* file);               // 0x00C31D80
    ~PKCAutoFile();                        // 0x00C31D90 (releases the file through its slot 3)
    void* GetChecked();                    // 0x00C31DB0
};
class CString { public:                    // libpk trace line (shared buffer)
    char* m_pBuffer;
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
};
class CLoadLog {                           // vtable 0x012B4CC0, 0x34 bytes
public:
    virtual void v00();
    unsigned char _pad04[0x2c];
    bool m_ok;                             // +0x30
    CLoadLog();                            // 0x00C308F0
    ~CLoadLog();                           // 0x00C30940
    void SetSourceName(const char* name);  // 0x00C308A0 (descriptive)
    CString& AddError();                   // 0x00C30840 (descriptive: queues an error message, clears m_ok)
    bool IsOk() const;                     // 0x00C307C0
};
struct CRequest_00c09110;
class CBankData { public:                  // 0x30 bytes on LoadBank's stack
    long _words[12];
    CBankData();                           // 0x00C2EED0
    ~CBankData();                          // 0x00C2EF30
};
bool __fastcall SerialiseBankMetaData(CBankData* data, CRequest_00c09110* request);   // 0x00C17820 (descriptive)
void __fastcall ReadBankData(void* file, CBankData* data, unsigned long chunk, CLoadLog* log);   // 0x00C30F60 (descriptive)

struct CProcessor_00c09110 {
    virtual void* Open(void* name);        // slot 0
};

// 0x00C17260: same-TU static (private convention: log in esi, callee-pop).
static void __stdcall LoadBank(CLoadLog* log, void* file, CRequest_00c09110* request)
{
    CBankData data;
    ReadBankData(file, &data, 0x2000, log);
    if (log->IsOk()) {
        if (!SerialiseBankMetaData(&data, request))
            log->AddError() << "There was a problem serialising the extracted meta data.";
    }
}

bool __fastcall Process_00c172f0(CProcessor_00c09110* processor, void* name, void* context, CRequest_00c09110* request)
{
    PKCAutoFile file(processor->Open(name));
    if (!file.m_pFile)
        return false;
    CLoadLog log;
    LoadBank(&log, file.GetChecked(), request);
    char buffer[0x100];
    WideToMultiByte((const unsigned short*)name, buffer, 0x100);
    log.SetSourceName(buffer);
    if (!log.IsOk())
        return false;
    return true;
}