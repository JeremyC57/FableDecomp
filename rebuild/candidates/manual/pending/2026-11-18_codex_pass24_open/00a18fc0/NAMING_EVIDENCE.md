# 0x00A18FC0 naming evidence

The selected RTM object emits `?MoveToPosProximity@CNavigatorFlyer@@UAE?AW4ENavigatorReturn@@ABVC3DVector@@0MMAAV3@_N22222M@Z`. Ego_r donor 0x00B840F0 and FableWin donor 0x031A5720 carries this exact decorated spelling.
The Ego_d module record identifies `CNavigatorFlyer::?MoveToPosProximity@CNavigatorFlyer@@UAE?AW4ENavigatorReturn@@ABVC3DVector@@0MMAAV3@_N22222M@Z` in `lib_navigator_flyer.obj`. CNavigatorFlyer::MoveToPosProximity (debug length 795).
Retail vtable 0x0129CB54 slot 4 resolves to this address under `CNavigatorFlyer`.
The imported draft used local models. The submitted port uses the engine headers. See the operand review for binding status.

The Ego_d module data for lib_navigator_flyer.cpp places MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT next to ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM. A second symbol record is adjacent to CNavigatorFlyer methods. In retail the first object is the constant 0.1f at 0x0129CB48 used in both altitude clamps. The second object is the vector at 0x01397130 independently named in the landed GetRandomNavigablePos sibling. The shared interface orders GetThemeHeightAt in slot 3, FindPosZAtIncludingThemes in slot 5 and PeekNavigatorRadius in slot 6. The two manager calls use the const C2DVector overloads.
