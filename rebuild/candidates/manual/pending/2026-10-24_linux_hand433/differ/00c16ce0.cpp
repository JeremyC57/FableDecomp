// _global::RunProcessor(void* target, void* source, void* context, const SProcessorSettings_00c16ce0* settings) -- builds a local 0x7c-byte processor over (source, target, context), copies the caller's settings record into it (two dwords, five option bytes, and the paired flag bytes set when settings+0x11 is non-zero), runs it and destroys it
// Grade: DIFFER(ours v 166) -- LTCG register argument (lane gap): retail reads the settings record from esi (never set or saved; both callers 0x00C16DE2 / 0x00C1746B load esi first) on top of ecx, edx and one stack argument (`ret 4`); this reference body takes it as a second stack argument. One grade, no dummy parameters.
// Leaf: RunProcessor
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16CE0, 166 bytes, EH-framed (the local processor).
// Callees: processor ctor @ 0x00C2E0C0 (source, target, context -- note the ecx/edx order swap), Run @ 0x00C2EB00,
//   dtor @ 0x00C2E140.
// Label correction: the catalog's `NScript_DeleteTrackNode` is propagated; descriptive name.
struct SProcessorSettings_00c16ce0 {
    unsigned long m_0;
    unsigned long m_4;
    unsigned long m_8;
    unsigned char m_c, m_d, m_e, m_f, m_10, m_11;
};

struct CProcessor_00c16ce0 {
    unsigned char _pad[0x6c];
    unsigned long m_6c;
    unsigned long m_70;
    unsigned char m_74, m_75, m_76, m_77, m_78, m_79, m_7a, m_7b;
    CProcessor_00c16ce0(void* source, void* target, void* context);   // 0x00C2E0C0
    ~CProcessor_00c16ce0();                                           // 0x00C2E140
    void Run();                                                       // 0x00C2EB00
};

void __fastcall RunProcessor(void* target, void* source, void* context, const SProcessorSettings_00c16ce0* settings)
{
    CProcessor_00c16ce0 processor(source, target, context);
    processor.m_6c = settings->m_0;
    processor.m_70 = settings->m_4;
    processor.m_79 = settings->m_c;
    processor.m_7a = settings->m_d;
    processor.m_76 = settings->m_e;
    processor.m_74 = settings->m_f;
    processor.m_75 = settings->m_10;
    if (settings->m_11) {
        processor.m_77 = 1;
        processor.m_78 = 1;
    }
    processor.Run();
}
