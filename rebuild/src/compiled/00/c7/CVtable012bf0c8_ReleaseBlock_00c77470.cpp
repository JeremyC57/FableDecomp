// CVtable_012bf0c8::ReleaseBlock() -- free the libpk DSP effect's settings blocks through the library allocator and forget all three block pointers
// Leaf: ?ReleaseBlock@CVtable_012bf0c8@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C77470, 42 bytes (int3 padding before at 0x00C7746A..F after the ctor 0x00C77440, and
//   0x00C7749A..F after it), thiscall, `if (+4) { free(+4); +4 = +8 = +0xC = 0; }`, plain `ret`.
// Identity (descriptive; libpk has no PDB and no RTTI, so the landed reverb-family spelling is kept): the out-of-line copy of the
//   settings-block base's ReleaseBlock -- (1) its callers are CReverbProcessor::Shutdown 0x00C6A3D0 and ~CReverbProcessor
//   0x00C6A780 (mid-body calls on `this`, the CVtable_012bf0c8 base at offset 0), (2) ~CVtable_012bf0c8 0x00C775A0 expands the same
//   body inline right after its vptr store (chunk 699, whose TU COMDAT of this function graded RELOCATION_MATCH against this row),
//   (3) it sits in that class's TU between the ctor 0x00C77440 and slot 1 SetSettings 0x00C774A0, and touches exactly the three
//   settings pointers CommitAndProcess 0x00C774F0 / SetSettings use. Not a destructor (no vtable store) and not
//   CLandscapeBackgroundPatch::ClearTextures (the forge label is propagated; no engine class is involved).
// Callee: CCodaObjectBase::operator delete 0x00C3C370 (`push [esp+4] ; call [0x0139C980]`, the libpk free hook; the spelling the landed
//   ~CVtable_012bf0c8 0x00C775A0 binds after naming wave 31, descriptive).
// Shape: a plain non-inline member defined in the class's file (VC7.1 inlines it into the dtor on its own, chunk 699); this
//   source is that file's class block: ctor/virtuals declared, ReleaseBlock and the user dtor `{ ReleaseBlock(); }` defined.
// Layout: the landed CommitAndProcess 0x00C774F0 / ~CVtable_012bf0c8 0x00C775A0 model (+0x04 settings, +0x08 target, +0x0C pending,
//   +0x10 size, +0x14 blending, +0x18 step, +0x1C countdown, +0x20 step count, +0x24 dirty; 0x28 bytes).
extern "C" void* __cdecl CodaMalloc(unsigned int size);   // 0x00C3C360 (GameCODA Memory.obj)
extern "C" void __cdecl CodaFree(void* p);                // 0x00C3C370
class CCodaObjectBase {                                       // the libpk root: its allocator hooks
public:
    static void __cdecl operator delete(void* p) { CodaFree(p); }       // 0x00C3C370
};

class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420
    virtual ~CVtable_012bf050();                              // 0x00C74450
};

class CVtable_012bf0c8 : public CVtable_012bf050 {            // vtable 0x012BF0C8: the DSP effects' settings-block base, 0x28 bytes
public:
    CVtable_012bf0c8();                                 // 0x00C77440
    virtual ~CVtable_012bf0c8();
    virtual void SetSettings(const void* settings, bool immediate);   // slot 1: 0x00C774A0
    virtual bool CommitAndProcess_00c774f0(const float* dry, bool active, const float* const* inputs, unsigned long count);   // slot 2
    virtual void v03();                                 // slot 3 (0x00C77590)
    virtual bool Process(const float* dry, bool active, const float* const* inputs, unsigned long count) = 0;   // slot 4
    void ReleaseBlock();                                // 0x00C77470
    void*         m_pSettings;                          // +0x04 current settings
    void*         m_pTargetSettings;                    // +0x08 settings being blended to
    void*         m_pPendingSettings;                   // +0x0c settings queued by SetSettings
    unsigned long m_settingsSize;                       // +0x10 bytes per settings block
    bool          m_blending;                           // +0x14
    long          m_blendStep;                          // +0x18
    long          m_blendCountdown;                     // +0x1c
    long          m_blendSteps;                         // +0x20
    bool          m_dirty;                              // +0x24
};

void CVtable_012bf0c8::ReleaseBlock()
{
    if (m_pSettings) {
        CodaFree(m_pSettings);
        m_pSettings = 0;
        m_pTargetSettings = 0;
        m_pPendingSettings = 0;
    }
}

CVtable_012bf0c8::~CVtable_012bf0c8()
{
    ReleaseBlock();
}