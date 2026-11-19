# Landing hand4b b6 (2026-11-19): NSpeechGainManager::OnDialogueStart (0x006E31E0, 596 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS; 9 grades. Shared-header port with Ego_d locals in FableWin order; the parked "x87 term order"
closed by FableWin / retail's single condition `layer == it->Layer && IS_THING_ALIVE(it->ThingSpeaking)` (fixes the ebx/ebp swap and the y,z,x magnitude
order with the default GetMagnitude). New opt-ins (`hdr_README.md`): stlport/fable_containers.h LIST_CREATE_NODE_NOINLINE (retail calls 0x006E4CB0 out of
line; this TU's copy grades RELOCATION_MATCH there), engine/NSpeechGainManager.h ACTIVEDIALOGUEINFO (EDialogueLayer with the PDB enumerators,
CActiveDialogueInfo with its inline (CThing&, ..) ctor, GetTargetGain declaration). Name fix: 0x006E4CB0 is STLport
list<NSpeechGainManager::CActiveDialogueInfo>::_M_create_node.
Recheck of the 8 landed NSpeechGainManager.h users: 5 good; 0x006E4560 / 0x006E49A0 / 0x006E4A00 (CResponseTimes fill / uninitialized_copy /
uninitialized_fill_n) grade DIFFER one byte short -- PRE-EXISTING (same result with the previous headers); handled with the catalog-health repair.
