// CThingFilter_SolidToThing::CThingFilter_SolidToThing(CThing* sourceThing, bool flag) -- constructor of a 16-byte record: the pointer argument (+0), a cleared word (+4), the bool argument (+8), an index of -1 (+0xC)
// Leaf: ??0CThingFilter_SolidToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61C70, length 32 bytes.
// Callees: none.
// Label caution: the PDB's CThingFilter_SolidToThing is 8 bytes { CThing* SourceThing; CTCSolidToThing* PTCSolidToThing; },
// while this constructor writes 16 bytes; its only caller (CGame_InitializeThingFilters region, 0x00C44CA4) allocates
// `operator new(0x10)` and passes (element, false). The first two members follow the PDB names; +8/+0xC are
// descriptive (a flag byte and a -1 index). The manifest label may be propagated.
// Shape: plain init-list stores in member order; `this` returned in eax.
struct CThing;
struct CTCSolidToThing;

struct CThingFilter_SolidToThing {
    CThing*          SourceThing;                   // +0x00
    CTCSolidToThing* PTCSolidToThing;               // +0x04
    bool             m_flag;                        // +0x08
    long             m_index;                       // +0x0c
    CThingFilter_SolidToThing(CThing* sourceThing, bool flag);
};

CThingFilter_SolidToThing::CThingFilter_SolidToThing(CThing* sourceThing, bool flag)
    : SourceThing(sourceThing)
    , PTCSolidToThing(0)
    , m_flag(flag)
    , m_index(-1)
{
}