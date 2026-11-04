// ReadBankData(void* file, CBankData* data, unsigned long chunk, CLoadLog* log) -- open the bank file's LiOnHeAd segment reader, check its version and wave segments, then read the wave records, the second record table and the criteria into data, logging each failure
// Leaf: ?ReadBankData@@YIXPAXPAVCBankData@@KPAVCLoadLog@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C30F60, 590 bytes, __fastcall (file ecx, data edx, chunk and log on the stack,
//   `ret 8`), EH-framed (the 0xB0-byte segment reader, the segment-name temporary).
// Identity (descriptive; libpk has no PDB or RTTI): the landed callers LoadBank 0x00C17260 and ProcessEntry 0x00C16D90
//   bind this spelling (reloc_bindings; the second binding ?FillEntry@@... is the older caller model). The texts are the
//   libpk bank loader's ("This bank is not a normal LUG!", "Could not open the wav segment").
// Callees: the segment reader ctor 0x00C2DA20 (catalog CLiOnHeAdSegmentReader: it checks the "LiOnHeAd" signature) and
//   dtor 0x00C2DBE0, its error count +0x7C, OpenSegment 0x00C2D9C0 and GetPosition 0x00C2DA10 (spelled CSegmentFile, the
//   landed OpenSegment binding); ReadBankInfoCount 0x00C16E10 (ecx); HasWaveBankSegments 0x00C30E90 (a same-TU static:
//   file in esi; the caller tests the whole eax, so it returns an int, not bool); the table reader wrapper ctor 0x00C3BB40,
//   Load 0x00C3BB50 and LoadCriteria 0x00C3BCB0 (one object serves all three calls); the three one-slot record readers
//   CWaveDataReader (vtable 0x012B4D3C), 0x012B4D34 and CSampleTableReader_012b4d40 (0x012B4D40); CLoadLog::AddError
//   0x00C30840 / IsOk 0x00C307C0; CString(const char*) 0x00C0BFE0, ~CString 0x00C0BA40, operator<< 0x00C0DEC0 / 0x00C0DF00.
// Shape: the first two record readers are temporaries (they share one frame slot, the wave reader's third word, the
//   segment position, stored before the wrapper is built); the criteria reader is a named local declared just before its
//   call (its own slot above the temporaries, its stores sunk to the call); the final `if (!log->IsOk()) return;` leaves
//   a bare call.
class CString { public:
    char* m_pchData;
    CString(const char* text);                          // 0x00C0BFE0
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};
class CLoadLog { public:
    CString& AddError();                                // 0x00C30840
    bool IsOk() const;                                  // 0x00C307C0
};
class CBankData;

struct CSegmentFile {                                   // the LiOnHeAd segment reader (0xB0 bytes)
    unsigned char m_base[0x7c];
    long          m_errors;                             // +0x7C
    unsigned char m_segments[0x30];                     // +0x80
    CSegmentFile(void* file, unsigned long chunk);      // 0x00C2DA20
    ~CSegmentFile();                                    // 0x00C2DBE0
    long OpenSegment(const CString& name);              // 0x00C2D9C0
    unsigned long GetPosition();                        // 0x00C2DA10
};
long __fastcall ReadBankInfoCount(CSegmentFile* file);  // 0x00C16E10
static long HasWaveBankSegments(CSegmentFile* file);    // 0x00C30E90

struct Callback {
    virtual void OnRecord(CLoadLog* log, const void* record) = 0;
};
struct CriteriaCallback {
    virtual void OnRecord(void* context, const char* name, unsigned long count, const unsigned long* values) = 0;
};
struct CWaveDataReader : Callback {                     // vtable 0x012B4D3C
    CBankData*    m_pData;
    unsigned long m_position;
    CWaveDataReader(CBankData* data, unsigned long position) : m_pData(data), m_position(position) { }
    virtual void OnRecord(CLoadLog* log, const void* record);
};
struct CUnprovenRecordReader_012b4d34 : Callback {      // vtable 0x012B4D34
    CBankData* m_pData;
    CUnprovenRecordReader_012b4d34(CBankData* data) : m_pData(data) { }
    virtual void OnRecord(CLoadLog* log, const void* record);
};
struct CSampleTableReader_012b4d40 : CriteriaCallback { // vtable 0x012B4D40
    CBankData* m_pData;
    CSampleTableReader_012b4d40(CBankData* data) : m_pData(data) { }
    virtual void OnRecord(void* context, const char* name, unsigned long count, const unsigned long* values);
};
struct AudioBankTableReader_00c3bb50 {
    CSegmentFile* reader;
    AudioBankTableReader_00c3bb50(CSegmentFile* file);  // 0x00C3BB40
    void Load(CLoadLog* log, Callback* callback);       // 0x00C3BB50
    void LoadCriteria(CLoadLog* log, CriteriaCallback* callback);   // 0x00C3BCB0
};

void __fastcall ReadBankData(void* file, CBankData* data, unsigned long chunk, CLoadLog* log)
{
    CSegmentFile reader(file, chunk);
    if (reader.m_errors) {
        log->AddError() << "There were problems decoding the file segment structure";
        return;
    }
    long version = ReadBankInfoCount(&reader);
    if (version < 0) {
        log->AddError() << "Could not extract the version number";
        return;
    }
    if (!HasWaveBankSegments(&reader)) {
        log->AddError() << "This bank is not a normal LUG!";
        return;
    }
    if (version != 0) {
        log->AddError() << "Expecting normal version number " << 0L << "... got " << version;
        return;
    }
    if (reader.OpenSegment(CString("LHAudioWaveData")) < 0) {
        log->AddError() << "Could not open the wav segment";
        return;
    }
    unsigned long position = reader.GetPosition();
    AudioBankTableReader_00c3bb50 tables(&reader);
    tables.Load(log, &CWaveDataReader(data, position));
    if (!log->IsOk())
        return;
    tables.Load(log, &CUnprovenRecordReader_012b4d34(data));
    if (!log->IsOk())
        return;
    CSampleTableReader_012b4d40 criteria(data);
    tables.LoadCriteria(log, &criteria);
    if (!log->IsOk())
        return;
}

// Same-TU static body (the landed 0x00C30E90 text, int result) so VC7.1 gives it the private convention (file in esi). Not graded.
static __declspec(noinline) long HasWaveBankSegments(CSegmentFile* file)
{
    bool failed = file->OpenSegment(CString("LHAudioWaveData")) < 0
               || file->OpenSegment(CString("LHAudioBankSampleTable")) < 0;
    return !failed;
}