// CReverbProcessor::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C6A780), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCReverbProcessor@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A9F0, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family): slot 0 of agent443's CReverbProcessor vtable 0x012B9594 (stored by CReverbProcessor::CReverbProcessor 0x00C6A650;
//   slot 4 is CReverbProcessor::Process 0x00C6A5D0); it calls CReverbProcessor::~CReverbProcessor 0x00C6A780 (FINDINGS
//   chunk 444).
//   The reverb parts have no class allocator (the CRT's 0x00BFE9BC, as CDspGain's 0x00C696A0). The landed
//   CLandscapeBackgroundPatch vector deleting destructor is a propagated label over a placeholder; descriptive names
//   (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420
    virtual ~CDspObject();                              // 0x00C74450
};

class CVtable_012bf0c8 : public CDspObject {            // vtable 0x012BF0C8: the DSP effects' settings-block base, 0x28 bytes
public:
    CVtable_012bf0c8();                                 // 0x00C77440
    virtual ~CVtable_012bf0c8();                        // 0x00C775A0
    virtual void v01();                                 // 0x00C774A0
    virtual void v02();                                 // 0x00C774F0
    virtual void v03();                                 // 0x00C77590
    virtual bool Process(const float* in, bool active, const float* const* sends, unsigned long frames) = 0;
    unsigned char _body04[0x24];
};

class CReverbProcessor : public CVtable_012bf0c8 {      // vtable 0x012B9594, 0x298 bytes
public:
    CReverbProcessor() { }                              // out of line at 0x00C6A650
    virtual ~CReverbProcessor();                        // 0x00C6A780
    virtual void v03();                                 // 0x00C69B60
    virtual bool Process(const float* in, bool active, const float* const* sends, unsigned long frames);   // 0x00C6A5D0
    unsigned char _body28[0x298 - 0x28];                // members: see CReverbProcessor::CReverbProcessor 0x00C6A650
};

CReverbProcessor* Create_CReverbProcessor()
{
    return new CReverbProcessor;
}
