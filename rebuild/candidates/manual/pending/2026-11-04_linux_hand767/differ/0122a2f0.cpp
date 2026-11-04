// `dynamic atexit destructor for 'interaction'' -- CCharString static at scope 43 of PracticeAgainstApprentice (not authored)
// Grade: DIFFER(not attempted) -- needs the owner: the static's mangled name carries the owner's block number (Ego_r scope 43, mangled ?interaction@?CL@??PracticeAgainstApprentice... (43 = 0x2B)) and the owner is a 0x4B0-byte body; an abridged owner cannot reach that scope honestly
// Leaf: _$E2
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122A2F0, 10 bytes: `mov ecx,013BB6A4h ; jmp 0x0099EAE0` (~CCharString on the static 0x013BB6A4).
// Next idea: author PracticeAgainstApprentice itself (its TU emits this _$E2 with the right scope), as this chunk did for the
//   GetText-family statics with the landed owner TUs (rows 0x01229160 / 0x01229DD0 / 0x01229DE0 / 0x01229E10).
