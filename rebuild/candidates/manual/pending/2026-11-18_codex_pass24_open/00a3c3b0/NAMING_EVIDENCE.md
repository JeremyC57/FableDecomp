# Naming evidence

- Ego_r 0x00B96C10 and FableWin 0x031FEAD0 give the exact leaf `?SetInterpolate@C3DAnimationBlendState@@QAEXABV1@0M@Z`.
- Native blend reads and writes corroborate Ego_d's anim1, anim2, weight2, no_blends, c_blend, blend1, blend2 and dest.
- The animation-alpha layout and same-TU constructor are corroborated by the native writes and the donor copy. Native calls the already landed resize at 0x00A3D500.

## Behavioral fixture

The fixture runs the selected SetInterpolate body with different blend-list lengths, morph pointer cases, and weights. Its animation and pointer components are lightweight fixtures.
