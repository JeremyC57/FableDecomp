# Naming evidence

- Ego_r 0x007DC9F0 and FableWin 0x027812A0 give the exact leaf `?FrameUpdate@CTCAICreatureWillPowerIndicator@@UAEXXZ`.
- The native will-power, timer and effect-attachment paths corroborate ptc_combat, pspecial_fx, fx_index, rhs, ptc_carrying, weapon_pos, pweapon, pemitter and ptcd_particle_emitter.
- The Ego_r class layout and virtual slot order agree with native vtable slot 9 and the component's field offsets.

The donor local ptcd_particle_emitter uses the particle-emitter component. The submitted local particle_driver preserves the type while respecting the requested source spelling rule.

## Behavioral fixture

The fixture executes the selected FrameUpdate body across 6,144 combinations of timer values, combat availability, melee, current power and emitter status, and particle setup gates. Stub calls check the effect name, creation position and arguments, emitter ownership, and weapon attachment arguments.
