// SProcessorSettings_00c16d90::ProcessEntry(void* source, void* target, CBankFile_00c16d90* file) -- fills a local 0x30-byte entry from `source` through the bank file (0x00C30F60, with this record's +8 value), and when the file is open runs the processor 0x00C16CE0 over (target, the entry, the file) with this settings record
// Grade: DIFFER(ours v 122) -- LTCG register arguments (lane gap): retail takes the bank file in edi (pushed / moved to ecx, never set or saved; the caller 0x00C17572 sets it) and hands this record to 0x00C16CE0 in esi; this reference body takes the file as a third stack argument and cannot pass esi. One grade, no dummy parameters.
// Leaf: ProcessEntry
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16D90, 122 bytes, `__thiscall` (`ret 8`) plus edi, EH-framed (the
//   local entry: ctor 0x00C2EED0, dtor 0x00C2EF30).
// Callees: Fill @ 0x00C30F60 (ecx source, edx &entry, stack: value, file), IsOpen @ 0x00C307C0, RunProcessor
//   @ 0x00C16CE0 (this chunk, parked: ecx target, edx &entry, stack file, esi settings).
// Label correction: the catalog's `Script_ProcessInventoryItem` is propagated; descriptive name.
struct CBankFile_00c16d90 {
    bool IsOpen();                         // 0x00C307C0
};

struct CEntry_00c16d90 {
    unsigned char _data[0x30];
    CEntry_00c16d90();                     // 0x00C2EED0
    ~CEntry_00c16d90();                    // 0x00C2EF30
};

struct SProcessorSettings_00c16d90;

void __fastcall FillEntry(void* source, CEntry_00c16d90* entry, unsigned long value, CBankFile_00c16d90* file);   // 0x00C30F60
void __fastcall RunProcessor(void* target, CEntry_00c16d90* entry, CBankFile_00c16d90* file,
                             const SProcessorSettings_00c16d90* settings);                                     // 0x00C16CE0

struct SProcessorSettings_00c16d90 {
    unsigned long m_0;
    unsigned long m_4;
    unsigned long m_value;                 // +0x08
    void ProcessEntry(void* source, void* target, CBankFile_00c16d90* file);
};

void SProcessorSettings_00c16d90::ProcessEntry(void* source, void* target, CBankFile_00c16d90* file)
{
    CEntry_00c16d90 entry;
    FillEntry(source, &entry, m_value, file);
    if (file->IsOpen())
        RunProcessor(target, &entry, file, this);
}
