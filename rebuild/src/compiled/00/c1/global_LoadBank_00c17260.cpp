// LoadBank(CLoadLog* log, void* file, CRequest_00c09110* request) -- libpk bank load step: read the bank data of the open file (chunk
// 0x2000) into a stack CBankData, logging into `log`; when the log is still good but the meta data does not serialise into the request,
// queue the error "There was a problem serialising the extracted meta data."
// Leaf: ?LoadBank@@YGXPAVCLoadLog@@PAXPAUCRequest_00c09110@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17260, length 138 bytes (+ int3 padding), `ret 8`, EH frame (`push -1 ; push offset ;
//   mov eax,fs:[0]`, state -1/0 around the 0x30-byte CBankData).
// Naming (naming wave 24, DECISIONS_W23_RESOLVED item 8): a same-TU static of its only caller 0x00C172F0 (the libpk bank-file load),
//   which defines the identical body and binds this spelling. libpk has no PDB / RTTI, so the name is descriptive; it is the caller's
//   model the other landed rows corroborate (CLoadLog::IsOk 0x00C307C0 bound by 0x00C2E480, the CBankData ctor / dtor bound by
//   0x00C17940, the CLoadLog* last argument of 0x00C30F60 bound by 0x00C16D90, the CString stream operator<< 0x00C0DEC0 per
//   DECISIONS_W21). Was the descriptive LoadRecordAndReport_00c17260 on a propagated AI model (CInventoryOwner / CRecord0x30 / CLoader /
//   AppendText).
// Shape: the TU-local static gets VC7.1's private convention (the log in esi: the caller does `lea esi,[esp+14h] ; call`); a same-TU
//   caller below (not graded) makes VC7.1 choose it; `__stdcall` gives the callee-pop `ret 8`.
// Callees: CBankData ctor 0x00C2EED0 / dtor 0x00C2EF30, ReadBankData 0x00C30F60 (__fastcall: file ecx, data edx, chunk and log on the
//   stack, `ret 8`), CLoadLog::IsOk 0x00C307C0 (`mov al,[ecx+30h]`), SerialiseBankMetaData 0x00C17820 (__fastcall(data, request)),
//   CLoadLog::AddError 0x00C30840, CString::operator<< 0x00C0DEC0 (__fastcall, the text 0x012B2CD0).
// Flags: /EHsc
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

static __declspec(noinline) void __stdcall LoadBank(CLoadLog* log, void* file, CRequest_00c09110* request)
{
    CBankData data;
    ReadBankData(file, &data, 0x2000, log);
    if (log->IsOk()) {
        if (!SerialiseBankMetaData(&data, request))
            log->AddError() << "There was a problem serialising the extracted meta data.";
    }
}
// Same-TU caller (0x00C172F0 passes its stack CLoadLog in esi), present so the static gets the private convention. Not graded.
void LoadBank_SameTUCaller(void* file, CRequest_00c09110* request)
{
    CLoadLog log;
    LoadBank(&log, file, request);
}