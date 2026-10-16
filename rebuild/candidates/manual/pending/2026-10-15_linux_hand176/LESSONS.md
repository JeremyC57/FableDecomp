# Chunk 176 lessons
- An `||`/`&&` chain whose bool is reloaded as `cmp byte ptr [ebp-1],0 ; setne al` is stored in a `char` local and returned through its implicit conversion (0xCBE245); a bool local gives `mov al`.
- `""` passed to a `const CCharString&` parameter as an IMPLICIT conversion temporary gives retail's `lea ecx,[tmp] ; push ecx`; `CCharString("")` reuses the ctor's eax.
- strmbase CBaseList members built through an inline `CGenericList<T>(name) : CBaseList(name)` wrapper form `lea ecx` before the NULL-name push (0xCAF500).
- A `_RTL_CRITICAL_SECTION` member modelled as `unsigned char[0x18]` is 1-aligned and shifts the following offsets; model it with its pointer/long members.
- An unused local initialised from an inline by-value helper (`CCharString r(text); return r;`) reproduces VC7.1's build/copy/destroy (no NRVO) where `T d = T(...)` folds into one construction (0xE848E0).
- Switch cases laid out 1,4,2,3 with case 1 identical to the default body: write each case as a full statement in that order; VC7.1 merges case 1 into the default and cross-jumps common tails (0xE82800, 0xE8A140).
- Quest-script class names come from the factory record: the ctor's caller does `push size ; call operator new`, and the factory address is stored next to the "S_V.." / "V_.." CCharString pair in the registration function at 0xCD8xxx.
- CGameScriptInterface slots: 289 SetQuestAsFailed(name, bool, const CWideString&, bool), 425 OpenDoor, 426 CloseDoor, 717/718/721/722 AddRumourCategory / AddNewRumourToCategory / AddGossipVillage / AddGossipFactionToCategory (rtti-vtables.tsv slot -> manifest name; ghidra_out/cgsi_vtable_slots.tsv is offset by 4 rows there).
- Lane gaps seen: tail cross-jump of a shared `lea ecx` dtor setup (0xE6E120/0xE6E1B0), unrotated simple loop (0x409140), size-TU zero register in ebx (0x4F97B3), partial jump threading of a constant switch arm (0xEC3270).
