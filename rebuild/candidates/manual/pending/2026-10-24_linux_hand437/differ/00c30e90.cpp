// _global::HasWaveBankSegments(CSegmentFile* file) -- true when the file opens both the "LHAudioWaveData" and the "LHAudioBankSampleTable" segments (each name a CString temporary)
// Grade: DIFFER(209v203) -- lane gap: ltcg register argument: retail is an LTCG static helper that takes the segment file in esi unsaved (`mov ecx,esi` before each OpenSegment, no prologue load, plain `ret`); standard VC7.1 C++ cannot name an esi argument. Reference body graded once
// Leaf: HasWaveBankSegments
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C30E90, 203 bytes, esi = file, no stack arguments, eax = result (`xor eax,eax ;
//   sete al`). Descriptive name (no PDB covers this library): the catalog's `Audio_LoadAndValidateWaveData` is a
//   description. The `||` gives the two temporaries the conditional-destruction flags in ebx (1, then 3; `test bl,2` /
//   `test bl,1` before each ~CString 0x00C0BA40); the failure flag is a frame byte.
// Callees: CString::CString(const char*) 0x00C0BFE0 (agent431), CSegmentFile::OpenSegment 0x00C2D9C0 (agent436; its key is
//   the address of the name string, handed to the segment table's slot-17 Find).
struct CString {
    char* m_pchData;
    CString(const char* text);              // 0x00C0BFE0
    ~CString();                             // 0x00C0BA40
};

struct CSegmentFile {
    long OpenSegment(const CString& name);  // 0x00C2D9C0
};

bool HasWaveBankSegments(CSegmentFile* file)
{
    bool failed = file->OpenSegment(CString("LHAudioWaveData")) < 0
               || file->OpenSegment(CString("LHAudioBankSampleTable")) < 0;
    return !failed;
}
