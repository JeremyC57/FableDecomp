// CAudioBankTableReader::CAudioBankTableReader(CSegmentFile* file) -- the bank table reader wrapper's constructor: it keeps the
//   segment file whose "LHAudioBankSampleTable" / "LHAudioBankCriteiaInfo" segments Load 0x00C3BB50 / LoadCriteria 0x00C3BCB0 walk
// Leaf: ??0CAudioBankTableReader@@QAE@PAVCSegmentFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3BB40, 11 bytes, thiscall `ret 4` (mov eax,ecx; store the argument at this+0).
// agentNF57 (q10487): the only retail call is 0x00C31102 in the landed ReadBankData 0x00C30F60 (callers.py), which builds the
//   wrapper on its segment reader and then calls Load at 0x00C3BB50 twice and LoadCriteria at 0x00C3BCB0 on the same object.
//   The catalog label `__EH_epilog3` was wrong (the CRT epilog is not this body; 0x00C3BB40 is not an EH helper), and the class
//   was the placeholder AudioBankTableReader_00c3bb50. Descriptive name: libpk has no PDB or RTTI, and no string, PDB name set,
//   FSE manifest or libpk header names this wrapper. Was a `struct T { T& Assign(int); }` stand-in.
class CSegmentFile;

struct CAudioBankTableReader {
    CSegmentFile* reader;                           // +0x00
    CAudioBankTableReader(CSegmentFile* file);
};

CAudioBankTableReader::CAudioBankTableReader(CSegmentFile* file) : reader(file) { }
