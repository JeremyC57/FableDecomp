# NF107 lessons (one line each)
- The K016 vector define set needs ROUGHAREA_TYPEDLOCAL for the Calls rows: a same-size DIFFER with only `mov [esp+38h],reg` before the first filter call is that opt-in, not a body problem (13 rows first form after it).
- Port a Calls row with its landed CALLER's define / include set when one exists (same retail TU): it carries the right filter headers and anonymous-namespace declarations; then add only the opt-ins the instance needs.
- MAP_VALIDATE (targeting TU) makes CThingMap::Cells protected: under it drop ROUGHAREA_CELL_BYTEOFFSET and let UPDATE_LIST's PeekCell branch stand.
- An instance mask test that compiles to `xor dl,dl ; jne` means T::StatGetTypeFlags fell back to CThing's 0: define T's STATTYPEFLAGS opt-in (or add one) before fitting the body.
- callers.py on each filter callee before moving it into `namespace {}`: if the callee row spells (CThing*) the binder never matched it, so the move creates no split.
- Before a python header edit, check how many times the anchor text occurs (BESTROUGH and FIRSTROUGH share lines) and never chain a grade after a possibly failing edit.
