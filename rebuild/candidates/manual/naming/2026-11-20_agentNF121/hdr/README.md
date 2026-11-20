# NF121 overlay: none

No overlay header. Every changed row was respelled in place inside the local CTCCarrying model it already had, so no opt-in was
needed (the planned add-only IsCarriedThingInPrimarySlot opt-in for engine/CTCCarrying.h was not added: no finished row uses it).
The temporary unchanged copy of engine/CTCCarrying.h used for the first grade was byte-identical to the repo and has been removed;
recheck_final.log was run with this empty overlay. Landed includers of any header: unaffected (no header changed).
