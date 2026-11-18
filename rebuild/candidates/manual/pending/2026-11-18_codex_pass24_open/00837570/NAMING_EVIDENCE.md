# Naming evidence

- Ego_r 0x0082B160 and FableWin 0x027FD3A0 give the exact leaf `?Save@CThingTrackNode@@UAEXAAVCCharString@@@Z`.
- The save override binds its only Ego_d formal script to all native string operations. All its intermediate source values are unnamed.
- The ordered base call, two remapped UID values, start/end flags and script-name line identify the virtual override and the native fields.

## Behavioral fixture

The fixture calls the selected Save body with previous and next nodes, start and end flags, and scripted names. It checks generated text and delegates UID remapping and base serialization to fixtures.
