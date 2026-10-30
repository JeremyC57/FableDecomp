// _global::HasWaveBankSegments(CSegmentFile* file) -- true when the file opens both the "LHAudioWaveData" and the "LHAudioBankSampleTable" segments (each name a CString temporary)
// Shape (retry1): parked as an "LTCG register argument" gap; as a same-TU `static __declspec(noinline)` with a caller VC7.1 /O2 gives the helper retail's private convention itself (file in ESI, unsaved, plain ret).
// Leaf: ?HasWaveBankSegments@@YA_NPAUCSegmentFile@@@Z
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

static __declspec(noinline) bool HasWaveBankSegments(CSegmentFile* file)
{
    bool failed = file->OpenSegment(CString("LHAudioWaveData")) < 0
               || file->OpenSegment(CString("LHAudioBankSampleTable")) < 0;
    return !failed;
}

// Same-TU caller (the wave-bank loader that validates an opened segment file), present so the static gets retail's private
// convention. Not graded.
bool LoadWaveBank_caller(CSegmentFile* file)
{
    return HasWaveBankSegments(file);
}