// NCreatureMode::CModeSleepBed::`scalar deleting destructor'(unsigned int flags) -- ~CModeSleepBed, then operator delete when bit 0 of flags is set; returns this
// Leaf: ?CModeSleepBed_scalar_deleting_destructor@@YIPAXPAVCModeSleepBed@NCreatureMode@@HI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084B820, `ret 4`; vftable 0x01275760 slot 0; calls 0x0084B840 (the class destructor) and operator delete 0x00BFE9BC.
// Naming (NF456, r454b unit with 0x0084B840): the destructor call is the real one, ??1CModeSleepBed@NCreatureMode@@UAE@XZ (Ego_r + FableWin name sets; virtual),
//   written as the qualified call `mode->NCreatureMode::CModeSleepBed::~CModeSleepBed()` (direct, not through the vftable); was the free stand-in CModeSleepBed_dtor over a
//   global-scope `struct CModeSleepBed`. The class is NCreatureMode's (`class`, pdb_class_keys.tsv), so the stand-in self's parameter type moves with it
//   (was ?CModeSleepBed_scalar_deleting_destructor@@YIPAXPAUCModeSleepBed@@HI@Z). Retail's own self ??_GCModeSleepBed@NCreatureMode@@UAEPAXI@Z (FableWin name set) is
//   compiler-generated and only emitted beside the vftable (the ctor-owned thin-row route), which is not this row's object.
namespace NCreatureMode {
class CModeSleepBed { public:
    virtual ~CModeSleepBed();          // 0x0084B840
};
}

void operator delete(void*);

void* __fastcall CModeSleepBed_scalar_deleting_destructor(NCreatureMode::CModeSleepBed* mode, int edx_unused, unsigned int flags)
{
    mode->NCreatureMode::CModeSleepBed::~CModeSleepBed();
    if (flags & 1)
        operator delete(mode);
    return mode;
}
