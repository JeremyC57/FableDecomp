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

// NF53: the file owner, log, string and bank data are the shared libpk classes (as the landed twin 0x00C17640 includes them):
//   libpk/PKCAutoFile.h (ctor 0x00C31D80, dtor 0x00C31D90, ?GetChecked@PKCAutoFile@@QAEPAVCAudioFile@@XZ 0x00C31DB0), libpk/PKCFileManager.h
//   (the processor; kept as the caller-bound CProcessor_00c09110 view), libpk/CScriptHost.h CLoadLog, libpk/CBankProcessor.h struct CBankData. Was local stand-ins.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CBankProcessor.h"
#include "libpk/PKCAutoFile.h"
#define FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#include "libpk/CScriptHost.h"
struct CRequest_00c09110;
bool __fastcall SerialiseBankMetaData(CBankData* data, CRequest_00c09110* request);   // 0x00C17820 (descriptive)
void __fastcall ReadBankData(void* file, CBankData* data, unsigned long chunk, CLoadLog* log);   // 0x00C30F60 (descriptive)

static void __stdcall LoadBank(CLoadLog* log, void* file, CRequest_00c09110* request)
{
    CBankData data;
    ReadBankData(file, &data, 0x2000, log);
    if (log->IsOk()) {
        if (!SerialiseBankMetaData(&data, request))
            log->AddError() << "There was a problem serialising the extracted meta data.";
    }
}

struct CProcessor_00c09110 {              // the landed caller 0x00C09110 binds this spelling (libpk/PKCFileManager.h is the class)
    virtual CAudioFile* Open(void* name);  // slot 0
};

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