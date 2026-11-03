// CVtable_012bf0c8::~CVtable_012bf0c8() -- destructor of the libpk DSP effects' settings-block base: release the settings blocks (ReleaseBlock inlined), then ~CVtable_012bf050
// Leaf: ??1CVtable_012bf0c8@@UAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C775A0, 54 bytes, thiscall, tail `jmp ~CVtable_012bf050`.
// Identity: it stores vtable 0x012BF0C8 first (no RTTI: libpk), the vtable the landed ctor 0x00C77440 stores; its callers are
//   that vtable's slot-0 `??_G` 0x00C77650, the landed ~CReverbProcessor 0x00C6A780 (its base-class tail) and two EH unwind
//   funclets (0x012165B3 / 0x01216643) of derived constructors. The landed CReverbProcessor / CommitAndProcess sources declare
//   it `virtual ~CVtable_012bf0c8();  // 0x00C775A0`; libpk has no PDB, so the class keeps the landed vtable name
//   (descriptive). The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: CCodaObjectBase::operator delete 0x00C3C370 (the libpk free hook, ??3CCodaObjectBase@@SAXPAX@Z), CVtable_012bf050::~CVtable_012bf050
//   0x00C74450 (vtable 0x012BF050; the shared-table release).
// Shape: user destructor `{ ReleaseBlock(); }` with ReleaseBlock a plain member defined above it in the same file: VC7.1 expands
//   it here (0x00C77470 is its out-of-line copy, which this TU's COMDAT also grades RELOCATION_MATCH), scheduling the vptr store
//   between the member test and its branch.
// Layout: the landed CommitAndProcess 0x00C774F0 model (+0x04 settings, +0x08 target, +0x0C pending, +0x10 size, +0x14 blending,
//   +0x18 step, +0x1C countdown, +0x20 step count, +0x24 dirty).
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