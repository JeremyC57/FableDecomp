# NF317 (w22) lessons (one line each)
- A scripted objsec reject that differs only in `?A0x` anonymous-namespace hashes is a compile-path artefact (HEAD / new built in different dirs): compile both at one path before judging (8 of 112 here were already correct).
- A static name derived from the self (`__ehhandler$<sym>`) follows the self rename; objsec must map derived names too.
- VC7.1 aligns the first member after an introduced vfptr to the class alignment (8 with doubles): shrink the leading pad from +0x08, not +0x04.
- Making a local hierarchy polymorphic also respells qualified base calls (`CThingPhysical::OnDie()` QAE -> MAE): census those callees and move every other binder of the old form in the same landing, or the move creates a split.
- A binders1 entry's table witness is only one of the binders: the exe census of 0x00856E50 found three more QAE binders.
- Screen every callee you respell against the T3_PORT_QUEUE reason column too: 0x008C61C0's nf299 identity-research line voided a friend fix that compiled ALL_EQ.
- `grep "Address = '<UPPER>'"` misses catalog blocks written in lowercase: look rows up through nf.src_of (case-insensitive) before saying a callee has no landed row.
