# Native VC7.1 source experiments

Received baseline DIFFER at 2546 bytes.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_light_clamp_comparisons` | Use two independent clamp comparisons instead of else-if. | DIFFER 2532/2516 | 459 |
| `r_light_initial_values` | Initialize zero-time defaults on the disabled lifetime arm. | DIFFER 2529/2516 | 429 |
| `r_light_interpolate_ternary` | Construct the main colour from a conditional interpolation result. | DIFFER 2505/2516 | 438 |
| `r_light_named_multiply` | Split the scaled radius values into assignments followed by scaling. | DIFFER 2534/2516 | 461 |
| `r_light_pdb_shared` | Use the real CPSCLight and CPSCBase classes, donor names and early clear. | DIFFER 2534/2516 | 461 |
| `r_light_donor_general` | Recover the FableWin midpoint byte conversion, lifetime names, component minimums and two interpolations. | DIFFER 2633/2516 | 365 |
| `r_light_donor_assignment` | Use the FableWin byte-colour assignment operator with the shared setter. | DIFFER 2540/2516 | 460 |
| `r_light_donor_clamps` | Write clamp caps as conditional statements instead of self-assigning expressions. | DIFFER 2540/2516 | 460 |
| `r_light_donor_native_literals` | Use single-precision forms for the native float-pool operands. | DIFFER 2553/2516 | 497 |

Selected `r_light_pdb_shared`. The shared CPSCLight and CPSCBase layout reproduces the complete light selection and submission path. The two radius calculations and colour temporary placement still differ.

The alignment column is diagnostic. It is not a byte or relocation grade.

## Continued main-chat grades

All 2,516 bytes and 675 instructions match. All 70 primary operands pass, as do the two mapped owners and the 108-byte primitive constructor, 233-byte colour clamp and 132-byte interpolation. The 4,608-case RTM fixture runs the submitted function with shared colour arithmetic and checks light state, byte channels, world/spot radius, and transformed position.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_light_plain_assignment | 2287 | 429 | 2 / 0 |
| round2_light_plain_explicit | 2287 | 429 | 2 / 0 |
| round2_lightbudget_depth16 | 2287 | 429 | 2 / 0 |
| round2_lightbudget_depth2 | 2287 | 429 | 2 / 0 |
| round2_lightbudget_depth4 | 2287 | 429 | 2 / 0 |
| round2_lightbudget_depth8 | 2287 | 429 | 2 / 0 |
| round2_lightbudget_flag0 | 2656 | 504 | 2 / 0 |
| round2_lightbudget_flag1 | 2656 | 504 | 2 / 0 |
| round2_lightbudget_flag2 | 2656 | 504 | 2 / 0 |
| round2_lightbudget_flag3 | 2656 | 504 | 2 / 0 |
| round2_lightowner_3d | 2553 | 499 | 1 / 0 |
| round2_lightowner_both | 2553 | 499 | 1 / 0 |
| round2_lightowner_byte | 2553 | 499 | 1 / 0 |
| round2_lightowner_compound | 2537 | 463 | 1 / 0 |
| round2_lightowner_fade | 2537 | 463 | 1 / 0 |
| round2_lightowner_gf | 2553 | 499 | 1 / 0 |
| round2_lightowner_guard | 2553 | 499 | 1 / 0 |
| round2_lightowner_lastcan | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastcast | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastdouble | 2516 | 670 | 2 / 0 |
| round2_lightowner_lastend | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastgroup | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastnamed | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastparAtt | 2516 | 675 | 2 / 0 |
| round2_lightowner_lastphaseorder | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastprod | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastrecept | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastref | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastreverse | 2516 | 671 | 2 / 0 |
| round2_lightowner_lastreverseScale | 2516 | 671 | 2 / 0 |
| round2_lightowner_laststart | 2516 | 671 | 2 / 0 |
| round2_lightowner_midarms | 2891 | 304 | 1 / 0 |
| round2_lightowner_midarmsmid | 2945 | 304 | 1 / 0 |
| round2_lightowner_midcash | 2537 | 463 | 1 / 0 |
| round2_lightowner_midcashbranch | 2578 | 360 | 1 / 0 |
| round2_lightowner_midcopy4 | 2553 | 499 | 1 / 0 |
| round2_lightowner_midcopydword | 2553 | 499 | 1 / 0 |
| round2_lightowner_midctor | 2553 | 499 | 1 / 0 |
| round2_lightowner_midctorbranch | 2578 | 360 | 1 / 0 |
| round2_lightowner_midget | 2537 | 463 | 1 / 0 |
| round2_lightowner_midgetbranch | 2578 | 360 | 1 / 0 |
| round2_lightowner_midgetref | 2551 | 447 | 1 / 0 |
| round2_lightowner_midop | 2537 | 463 | 1 / 0 |
| round2_lightowner_midopbranch | 2578 | 360 | 1 / 0 |
| round2_lightowner_midopref | 2551 | 447 | 1 / 0 |
| round2_lightowner_midparen | 2553 | 499 | 1 / 0 |
| round2_lightowner_midparenbranch | 2578 | 360 | 1 / 0 |
| round2_lightowner_midstatic | 2553 | 499 | 1 / 0 |
| round2_lightowner_midstaticbranch | 2578 | 360 | 1 / 0 |
| round2_lightowner_midstaticref | 2567 | 468 | 1 / 0 |
| round2_lightowner_mix | 2537 | 463 | 1 / 0 |
| round2_lightowner_origvec | 2553 | 499 | 1 / 0 |
| round2_lightowner_rgb | 2553 | 499 | 1 / 0 |
| round2_lightowner_rgbinline | 2553 | 499 | 1 / 0 |
| round2_lightowner_visibleboth | 2516 | 671 | 2 / 0 |
| round2_lightowner_visiblelight | 2537 | 461 | 1 / 0 |
| round2_lightowner_visibleoriginal_shape | 2516 | 614 | 2 / 0 |
| round2_lightowner_visiblescale | 2516 | 614 | 2 / 0 |
| round2_lightowner_visiblescale_basic | 2516 | 614 | 2 / 0 |
| round2_lightowner_visiblescale_no_realctor | 2532 | 608 | 2 / 0 |
| round2_lightowner_visiblesm_all | 2516 | 671 | 2 / 0 |
| round2_lightowner_visiblesm_light | 2537 | 461 | 1 / 0 |
| round2_lightowner_visiblesm_scale | 2516 | 614 | 2 / 0 |
| round2_lightowner_visiblesm_scale_late | 2516 | 614 | 2 / 0 |
| round2_lightowner_visiblesmooth | 2537 | 463 | 1 / 0 |
| round2_lightowner_visiblesmoothinline | 2537 | 463 | 1 / 0 |
| round2_lightowner_visiblesmoothlate | 2537 | 463 | 1 / 0 |
| round2_lightowner_withdiv | 2553 | 499 | 1 / 0 |
| round2_lightradius_clamp | 2537 | 459 | 1 / 0 |
| round2_lightradius_ctor4 | 2537 | 463 | 1 / 0 |
| round2_lightradius_division | 2537 | 463 | 1 / 0 |
| round2_lightradius_inline | 2537 | 463 | 1 / 0 |
| round2_lightradius_named | 2537 | 463 | 1 / 0 |
| round2_lightradius_paren | 2537 | 463 | 1 / 0 |
| round2_lightradius_phase | 2537 | 463 | 1 / 0 |
| round2_lightradius_phase_named | 2537 | 463 | 1 / 0 |
| round2_lightradius_phaseonly | 2534 | 461 | 1 / 0 |
| round2_lightradius_real | 2537 | 463 | 1 / 0 |
| round2_lightradius_rgba | 2537 | 463 | 1 / 0 |
