# NF320 lessons (2026-11-23, alias-sig signature ports + Clone pilot)
- Triage by header first (tools/triage.py + tools/abscensus.py): 77 of 144 alias-sig entries had the PDB member declared under a landed
  switch; 48 flat GENERATED headers and 8 missing headers cannot take a virtual without a new class model. Do the declared ones first.
- Most "stand-in parameter types" rows are constant-return / empty bodies: the PDB signature changes nothing in the bytes. 45 of the 57
  done rows needed only the definition head and the switch set.
- Minimal switch sets beat precedent preambles: define the class switch, compile with clang, and add only the prerequisite the error
  names (a base's switch, CArray, CDataInputStream, STATICBLOCKS_TEXTURE_HELPERS for the primitive managers). A big precedent block
  drags in unrelated opt-ins.
- Compile HEAD and new under ONE LANDVERIFY_WORK path (tools/runpair.sh renames the dir after each run): no ?A0x false diffs.
- Build the new payload FROM the HEAD payload (tools/mkpay.py): same flags, pass pattern and test on both sides; only source and Leaf
  change.
- A flat-header body that reads "member +N" of a class with a secondary base can be the override's `this` on the base subobject:
  CWorld::DrawGetWorldMap's `[ecx+10h]` is PWorldMap (+0x14) seen from CIDrawWorld (+4) (0x0049E1A0).
- Read virtual-slot callees off the retail vtable (tools/vslot.py) before naming them: slot 10 of CWorldMap is IsMapLoaded.
- A byte-exact move can still ADD pick-any COMDATs (inline array ctors -> ??_H / ??_I; unreferenced CArray::operator[] /
  CCountedPointer::operator* copies): objsec's DEF-set check catches them where the grade cannot.
- Clone ports: the emitted ??_7 can be compared slot by slot with the retail table (tools/vtcmp.py); the header model must declare every
  override retail has (DropGenericBox misses GetAnimExtraFlags), and the TU newly emits ??_G.
- Never write `rm -f $var` even for a scratch error file in your own folder (slip recorded); use clean.py. Never put `sleep` on a launch
  line, even `sleep 0`, and record the PID with `echo $!` on the line after `&`.
- Check the neighbour's hdr/ overlay for headers your rows include (NF319 edits two headers my done rows include): say so for the landing.
- Add-only override declarations (both PDB name sets, a base in the modelled chain declares the identical virtual): insert the opt-in
  block just before the class's closing `};` (no access label leaks onto later members), run pin_asserts.py on the copy (the default
  branch's asserts below move), then ONE exact equiv_check for all the headers plus a small planted-control run restricted to a few
  includers (addr-file) that must flag only the default-branch includers.
- Grade the new side with FABLE_HDR_OVERLAY=<full new tree> and the HEAD side on the repo headers (runpair.sh <tag> <tree>).
