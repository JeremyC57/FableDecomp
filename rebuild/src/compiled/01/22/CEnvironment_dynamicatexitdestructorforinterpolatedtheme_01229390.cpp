// CEnvironment::RenderUpdate'::`5'::`dynamic atexit destructor for 'interpolated_theme''() -- destroy the function-local static CBlendedEnvironmentTheme interpolated_theme at exit
// Leaf: _$E2
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229390, 10 bytes: `mov ecx, offset 0x013BA900 ; jmp 0x006B5D80`
//   (~NEnvironmentInternal::CBlendedEnvironmentTheme on the static).
// Identity (two sources): (1) the only reference to 0x01229390 is the `push offset 0x01229390 ; call atexit 0x004012BC` at 0x006B43C5
//   inside CEnvironment::RenderUpdate 0x006B42F0, right after the guarded default construction of the static at 0x013BA900
//   (guard 0x013BAAF4 bit 0; ctor ??0CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ 0x008864A0); (2) both PDB name sets list
//   `CEnvironment::RenderUpdate'::`5'::`dynamic atexit destructor for 'interpolated_theme'' (donors 0x0400F700 / 0x0101C0B0).
//   Scope 5 (VC7.1 numbering, probed): an unbraced `if` then one block before a function-scope static gives ?4?, retail's shape
//   (`if (!PassedFirstUpdate) Update(0)`, the clamp, then the tick-state temporary at [esp+0D4h] built and destroyed before the static).
//   Static: ?interpolated_theme@?4??RenderUpdate@CEnvironment@@QAEXM@Z@4VCBlendedEnvironmentTheme@NEnvironmentInternal@@A.
//   The catalog's "OnPostRelocation_Callback" was a guess. VC7.1 emits the stub as the TU-local `_$E2` in .text$yd (precedent 0x01229170).
// TU: ABRIDGED owner. Only the thunk is graded; the owner body below models the static's declaration context and nothing else. The
//   real 0x610-byte owner 0x006B42F0 is its own unworked row and must replace this body when it is authored.
// Callees: ??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ 0x006B5D80.
// Header: engine/CEnvironment.h (FABLE_RETAIL_CENVIRONMENT, FABLE_RETAIL_CENVIRONMENT_RENDERUPDATE). No CBlendedEnvironmentTheme header
//   exists; the local view declares only the two special members the static needs (retail object 0x1F4 bytes: 0x013BA900..0x013BAAF4).
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_RENDERUPDATE
#include "engine/CEnvironment.h"
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    CBlendedEnvironmentTheme();                                  // 0x008864A0
    ~CBlendedEnvironmentTheme();                                 // 0x006B5D80
    unsigned char Data[0x1f4];
};
}
void CEnvironment::RenderUpdate(float frame_fraction)
{
    if (!PassedFirstUpdate)
        Update(0.0f);
    {
        NEnvironmentInternal::CBlendedEnvironmentTheme blend;
        UpdateInterpolation();
    }
    static NEnvironmentInternal::CBlendedEnvironmentTheme interpolated_theme;
    Update(frame_fraction);
}