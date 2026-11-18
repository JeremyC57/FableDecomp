# Naming evidence

- Ego_r 0x00577560 and FableWin 0x01E98340 give the exact leaf `?CalcCachedAnims@CAnimateThing@@ABEXXZ`.
- The native tail's CT lookup and two writes corroborate the Ego_d const ptc_appearance_morph. The source omits the debug-only c1 loop from the release implementation.
- Ego_d types and the native virtual dispatch corroborate private const CalcCachedAnims, the CTCSkeletalMorph member model and its SetSkeletalMorph call.

## Behavioral fixture

The fixture calls the selected CalcCachedAnims body with absent and present skeletal-morph components. It records the animation identifiers and morph data forwarded to the cache helper.
