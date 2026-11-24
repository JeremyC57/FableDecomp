# Landing nf378 b2: five more stand-in selves get their PDB names

0x00A0AE10 DrawCurrentBuffersAsTris, 0x009F3390 CSurface::GetPitch and three CThingSearchTools::GetPBestThing instances (0x0093F8A0, 0x00914B20, 0x006CD3B0, three distinct decorations). Header opt-ins CSurface (FABLE_RETAIL_CSURFACE_GETPITCH) and CThingSearchTools (FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_DECL), proven with b1's header run (0 differing of 57,158).
- Ruling NF378-R2 (advisor decision, owner delegated): 0x00A0AE10's `long` -> `void` is a respell, not a type change: both PDBs give `QAEXKK`, the bytes are equal (retail sets no return value) and no landed binder uses the old spelling. 0x006B0190 / 0x00A0AB50 qualify as T2 when the same three hold.
