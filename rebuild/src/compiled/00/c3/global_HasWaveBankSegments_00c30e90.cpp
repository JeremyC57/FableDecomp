// _global::HasWaveBankSegments(CSegmentFile* file) -- nonzero when the file opens both the "LHAudioWaveData" and the "LHAudioBankSampleTable" segments (each name a CString temporary)
// Shape (retry1): parked as an "LTCG register argument" gap; as a same-TU `static __declspec(noinline)` with a caller VC7.1 /O2 gives the helper retail's private convention itself (file in ESI, unsaved, plain ret).
// Leaf: ?HasWaveBankSegments@@YAJPAVCSegmentFile@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C30E90, 203 bytes, esi = file, no stack arguments, eax = result (`xor eax,eax ;
//   sete al`). Descriptive name (no PDB covers this library): the catalog's `Audio_LoadAndValidateWaveData` is a
//   description. The `||` gives the two temporaries the conditional-destruction flags in ebx (1, then 3; `test bl,2` /
//   `test bl,1` before each ~CString 0x00C0BA40); the failure flag is a frame byte.
// Signature (agentNF57, settles NF53's queued "bool/struct vs long/class"): long over class CSegmentFile, as the landed caller
//   ReadBankData 0x00C30F60 declares it (?HasWaveBankSegments@@YAJPAVCSegmentFile@@@Z). Two sources: (1) the body zero-extends the
//   whole result (`xor eax,eax ; test cl,cl ; sete al`) and the caller tests the whole register (`test eax,eax` at 0x00C31035), the
//   int-sized return, not a bool (al only); (2) libpk/CSegmentFile.h (and every landed libpk row) keys CSegmentFile as `class`.
//   Was `bool` over a local `struct CSegmentFile` / `struct CString`.
// Callees: CString::CString(const char*) 0x00C0BFE0, CSegmentFile::OpenSegment 0x00C2D9C0 (its key is the address of the name string,
//   handed to the segment table's slot-17 Find), ~CString 0x00C0BA40 -- on the shared engine/CString.h / libpk/CSegmentFile.h, the
//   include set of the landed caller 0x00C30F60, whose same-TU copy of this body is the text below.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CBankProcessor.h"
#define FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#include "libpk/CScriptHost.h"

static __declspec(noinline) long HasWaveBankSegments(CSegmentFile* file)
{
    bool failed = file->OpenSegment(CString("LHAudioWaveData")) < 0
               || file->OpenSegment(CString("LHAudioBankSampleTable")) < 0;
    return !failed;
}

// Same-TU caller (the wave-bank loader that validates an opened segment file), present so the static gets retail's private
// convention. Not graded.
long LoadWaveBank_caller(CSegmentFile* file)
{
    return HasWaveBankSegments(file);
}
