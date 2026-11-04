// ReadDeferredBank(void* unused, void* file, CLoadLog* log, CScriptHost* host, const SBankRequest* request) -- open the deferred (version 4) bank's segment reader, check its version and bank segments, then read the composite-data position, the highest priority and the sample lookup table and queue one event per sample (offset by the composite data) on the host, logging each failure
// Grade: DIFFER(848v857) -- from the first Read on the scratch registers rotate (eax/ecx/edx) and the table lands in ebx, so the lookup-table failure tail merges with the earlier error tails (retail pushes ebp for the table and keeps that tail apart)
// Leaf: ?ReadDeferredBank@@YIXPAX0PAVCLoadLog@@PAVCScriptHost@@PBUSBankRequest@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C16F00, 857 bytes, __fastcall (ecx unused, the file in edx, log / host / request on the
//   stack, `ret 0Ch`), EH-framed (the 0xB0-byte segment reader, the two segment-name temporaries).
// Identity (descriptive; libpk has no PDB or RTTI): the ".lut" arm of the bank loader 0x00C174A0 (its only caller; the ".lug"
//   arm calls ProcessEntry 0x00C16D90 -> ReadBankData 0x00C30F60, whose landed model this row shares). Texts: "Expecting deferred
//   bank version number ", "Could not open the composite data segment", "Could not read in the lookup table".
// Callees: the segment reader ctor 0x00C2DA20 / dtor 0x00C2DBE0, its error count +0x7C, OpenSegment 0x00C2D9C0, GetPosition
//   0x00C2DA10, Read 0x00C2DA00; ReadBankInfoCount 0x00C16E10 (ecx); HasBankSegments 0x00C16C10 (a same-TU static: the file
//   in esi, the caller tests the whole eax); CLoadLog::AddError 0x00C30840; the host's QueueEvent 0x00C0E9E0;
//   operator new[] 0x00BFEB22 / delete[] 0x00BFEB1C; CString(const char*) 0x00C0BFE0, ~CString 0x00C0BA40, operator<<
//   0x00C0DEC0 / 0x00C0DF00.
class CString { public:
    char* m_pchData;
    CString(const char* text);                          // 0x00C0BFE0
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};
class CLoadLog { public:
    CString& AddError();                                // 0x00C30840
};
struct CSegmentFile {                                   // the LiOnHeAd segment reader (0xB0 bytes)
    unsigned char m_base[0x7c];
    long          m_errors;                             // +0x7C
    unsigned char m_segments[0x30];                     // +0x80
    CSegmentFile(void* file, unsigned long chunk);      // 0x00C2DA20
    ~CSegmentFile();                                    // 0x00C2DBE0
    long OpenSegment(const CString& name);              // 0x00C2D9C0
    unsigned long GetPosition();                        // 0x00C2DA10
    int Read(void* buffer, int size);                   // 0x00C2DA00
};
long __fastcall ReadBankInfoCount(CSegmentFile* file);  // 0x00C16E10
static long HasBankSegments(CSegmentFile* file);        // 0x00C16C10

struct SBankRequest {
    unsigned long m_0;
    unsigned long m_4;
    unsigned long m_chunk;                              // +0x08
};
struct SLookupEntry {                                   // 12 bytes
    long m_key;                                         // +0x00
    long m_length;                                      // +0x04
    long m_offset;                                      // +0x08 (into the composite data)
};
class CScriptHost { public:
    unsigned char _pad00[0x48];
    long          m_deferred;                           // +0x48
    long          m_highestPriority;                    // +0x4c
    void QueueEvent(long type, long time, long data);   // 0x00C0E9E0
};

void __fastcall ReadDeferredBank(void* unused, void* file, CLoadLog* log, CScriptHost* host, const SBankRequest* request)
{
    CSegmentFile reader(file, request->m_chunk);
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
    if (reader.OpenSegment(CString("LHAudioBankCompData")) < 0) {
        log->AddError() << "Could not open the composite data segment";
        return;
    }
    unsigned long compData = reader.GetPosition();
    if (reader.OpenSegment(CString("LHAudioBankLookupTable")) < 0) {
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
    SLookupEntry* table = new SLookupEntry[count];
    if (reader.Read(table, count * sizeof(SLookupEntry)) < (int)(count * sizeof(SLookupEntry))) {
        delete[] table;
        log->AddError() << "Could not read in the lookup table";
        return;
    }
    for (unsigned long i = 0; i < count; ++i)
        host->QueueEvent(table[i].m_key, table[i].m_offset + compData, table[i].m_length);
    host->m_highestPriority = highestPriority;
    host->m_deferred = 1;
    delete[] table;
}

// Same-TU static body (the landed 0x00C16C10 text, int result) so VC7.1 gives it the private convention (file in esi). Not graded.
static __declspec(noinline) long HasBankSegments(CSegmentFile* file)
{
    bool missing = file->OpenSegment(CString("LHAudioBankLookupTable")) < 0
                || file->OpenSegment(CString("LHAudioBankCompData")) < 0;
    return !missing;
}
