# Naming evidence

- Ego_r 0x0093CBC0 and FableWin 0x02BB7E60 give the exact leaf `?FindInterestingThing@CAIStateGroup_LookAtInterestingThings@@AAEPAVCThing@@XZ`.
- Native counts the filtered objects and makes two weighted selection passes, corroborating Ego_d's objects, total_interest, chosen_interest, n and interesting.
- The filter instance has interface key 0xB3, demonstrated by the native constructor argument and the established CThingFilter_HasTC layout.

## Behavioral fixture

The fixture calls the selected search body with bounded thing lists and deterministic RNG. It checks filtering, selected indices, seeds, and position-derived search input.
