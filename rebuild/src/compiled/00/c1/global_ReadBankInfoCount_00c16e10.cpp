// _global::ReadBankInfoCount(CSegmentFile_00c16e10* file) -- opens the bank file's "LHFileSegmentBankInfo" segment, skips its two MAX_PATH name fields and returns the entry count that follows (0 when the count field is empty, -1 on any open/read failure)
// Leaf: ReadBankInfoCount
// Grade: RELOCATION_MATCH (retry wave 2) -- the open test result held in a named bool and every failure as its own
//   return -1 (plus explicit read==0 return 0) puts the merged return -1 block right after the open test, as retail
// Flags: /EHsc /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16E10, 225 bytes, ecx-passed file (`__fastcall`, plain `ret`),
//   EH-framed (the segment-name string temporary) and /GS-cookied (the 0x104-byte name buffer).
// Callers: the bank loader @ 0x00C16F8E (logs and bails on a negative count) and 0x00C30FEF; both pass a local
//   segment file in ecx. Callees: OpenSegment @ 0x00C2D9C0 (directory slot 17 lookup + seek; 0 / -1), Read @ 0x00C2DA00
//   (forwarder to 0x00C3A9E0; bytes read or negative), the MFC-style string @ 0x00C0BFE0 / 0x00C0BA40.
// Shape: (retail) the open test is `setl` into bl before the temporary's destructor; both name fields are read into the same
//   buffer (the second Read reuses eax = 0x104 as its size push); the count is zeroed after its address push; the
//   result `read ? count : 0` compiles to neg/sbb/and.
// Label correction: the catalog's `LHFileSegment_ReadMurderTwistData` is propagated; descriptive name.
struct CString_00c16e10 {                  // MFC-style string: one word
    char* m_pchData;
    CString_00c16e10(const char* text);    // 0x00C0BFE0
    ~CString_00c16e10();                   // 0x00C0BA40
};

struct CSegmentFile_00c16e10 {
    int OpenSegment(const CString_00c16e10& name);   // 0x00C2D9C0: 0 found and positioned, -1 missing
    int Read(void* buffer, int size);                // 0x00C2DA00: bytes read, negative on error
};

long __fastcall ReadBankInfoCount(CSegmentFile_00c16e10* file)
{
    char name[0x104 + 1];
    long count;
    int read;
    bool missing = file->OpenSegment(CString_00c16e10("LHFileSegmentBankInfo")) < 0;
    if (missing)
        return -1;
    if (file->Read(name, 0x104) != 0x104)
        return -1;
    if (file->Read(name, 0x104) != 0x104)
        return -1;
    count = 0;
    read = file->Read(&count, 4);
    if (read < 0)
        return -1;
    if (read == 0)
        return 0;
    return count;
}