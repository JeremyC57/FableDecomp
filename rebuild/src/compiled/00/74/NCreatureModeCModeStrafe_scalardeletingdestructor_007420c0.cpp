// NCreatureMode::CModeStrafe::`scalar deleting destructor'(unsigned int flags) -- ~CModeStrafe, then operator delete when bit 0 of flags is set; returns this
// Leaf: ?CModeStrafe_scalar_deleting_destructor@@YIPAXPAVCModeStrafe@NCreatureMode@@HI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007420C0, `ret 4`; vftable 0x01268588 slot 0; calls 0x007420E0 (the class destructor) and operator delete 0x00BFE9BC.
// Naming (NF456, r454b unit with 0x007420E0): the destructor call is the real one, ??1CModeStrafe@NCreatureMode@@UAE@XZ (Ego_r + FableWin name sets; virtual),
//   written as the qualified call `mode->NCreatureMode::CModeStrafe::~CModeStrafe()` (direct, not through the vftable); was the free stand-in CModeStrafe_dtor over a
//   global-scope `struct CModeStrafe`. The class is NCreatureMode's (`class`, pdb_class_keys.tsv), so the stand-in self's parameter type moves with it
//   (was ?CModeStrafe_scalar_deleting_destructor@@YIPAXPAUCModeStrafe@@HI@Z). Retail's own self ??_GCModeStrafe@NCreatureMode@@UAEPAXI@Z (FableWin name set) is
//   compiler-generated and only emitted beside the vftable (the ctor-owned thin-row route), which is not this row's object.
namespace NCreatureMode {
class CModeStrafe { public:
    virtual ~CModeStrafe();          // 0x007420E0
};
}

void operator delete(void*);

void* __fastcall CModeStrafe_scalar_deleting_destructor(NCreatureMode::CModeStrafe* mode, int edx_unused, unsigned int flags)
{
    mode->NCreatureMode::CModeStrafe::~CModeStrafe();
    if (flags & 1)
        operator delete(mode);
    return mode;
}
