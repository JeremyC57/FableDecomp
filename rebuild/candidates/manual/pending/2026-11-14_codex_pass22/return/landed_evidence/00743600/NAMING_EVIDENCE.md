# Identity and extent

`??1CModeGuildmasterPowerUpShield@NCreatureMode@@UAE@XZ` is the original FableWin024BEC50 destructor. Native RTTI012685F0 slot0 holds007435E0, whose first call reaches assigned00743600. The121-byte complete body ends with a tail jump to CModeStandBase::~CModeStandBase0073D860. It is not the manifest's catch funclet. The shared header's original member names and types agree with Ego_r layouts: Filter+68, Timer+74, LightningBeamParticleEmitters+78, Guards+84, total90 after native12-byte vectors replace donor16-byte vectors.

It is an implicit destructor, as proven by no initial derived-vptr store and by the matched sibling Heal/Speed destruction sequences. The shared header's user declaration is suppressed only by a new option. The genuine already-landed one-argument constructor00742DA0 materialises this COMDAT. Its exact signature is in Ego_r007560E0/FableWin024BEAF0. The constructor builds the literal POWER_UP filter, initialises Timer to0, and default-constructs the two vectors;151/151 bytes match independently. This is a real carried constructor, not an invented instantiation helper.

# Vtables and bindings

All five row relocations are decoded and checked: two calls to CRT free00BFEA14, CCharString::~CCharString0099EAE0, CAnimFilterBase table0122FFCC and CModeStandBase::~CModeStandBase0073D860. The carried constructor's ten relocations all agree with canonical bindings or exact literal contents. The compiler-generated scalar deleting body independently matches30/30 bytes at007435E0; its vector-deleting name is used by the native table as in the Ego_r PDB.

Native012685F0 has12slots. Shared CModeStandBase omitted AnimUpdate at slot2, GetMovementSpeed at slot7 and IsAllowingPlayerSpecialMovementMoves at slot9. The pass22 declaration-only option now restores all three; AnimUpdate is Ego_r00817670/FableWin0283F510, native00862EC0. The latter two were independently established by assigned row008E3A20. The derived header also omitted GetAnimFilter; native00742E50 at slot4 and FableWin024BEBF0 prove it. The new option declares that real override. VTABLE_REVIEW.json records every emitted symbol and native target. All old header paths are unchanged with pinned assert names.

# Behavior tests

Twenty-five fixtures retain the complete implicit target composition and exact carried constructor body, varying guards0..4 and beams0..4. Virtual deletion through the primary mode base must destroy every guard before every beam, then the filter string, filter base, stand base and mode base. Initial Timer, owner and POWER_UP string are checked. VC7.1 /Od /W3 /GR- without /EHsc and g++ C++03 both pass. Native layout and byte comparison are separate from the portable fixture.

# Merge note

Use this row's CModeStandBase.h as the superset of008E3A20's pass22 option: it adds AnimUpdate to that same new guard. Both target rows must be regraded under the final merged tree; no default or pre-pass22 option changes.
