# Batch `2026-10-14_linux_eh19` — hand-authored small EH-framed bodies: 13 bodies (13 landed)

Nineteenth hand pass (Wine lane, `/EHsc`): a second `CTCFishingSpot` destructor layout, the three
`CAIStateGroup_BirdFlyToFood::Update` bodies (a 0x40-byte action-request record built on the
stack with an intelligent pointer at +0x14, filled through the tiny `lea eax,[ecx+N]` slot
accessors), two entity walks over the inventory array, the `CStackOwner` destructor (polymorphic
member at +0x34, vector released through a non-throwing operator delete), a typed-container
destructor whose items carry a string member (`delete item` inlines the member destructor), the
`CGuiVarTransferFloat` constructor (by-value float wrapper argument used as the bind object, a
12-byte stack functor), the `CTeleportDataOwner` destructor (two raw blocks freed with one stack
clean-up, pair member destroyed inline), the EAX audio-context destructor (lazy import resolve,
muted volume through a function pointer), the hero-stats listener creation (class operator new
shared by two listener kinds) and the inactive-action allocation under the lock.

Lessons kept: a non-throwing `operator delete(void*) throw()` declaration removes the EH state of
an inline vector release; a member whose destructor is empty but user-declared registers an EH
state without emitting code; an array whose elements have a constructor but no destructor is
built with an inline counted loop. Parked (`docs/journal/FINDINGS_LOG.md`): 0x00C372C0 (store
order of three zeros and a flag around the EH state), 0x00C47050 / 0x00C470E0 (entry load
register), 0x00C28DC0 / 0x00C28E50 / 0x00C13840 / 0x00C138F0 (`std::length_error` throw: the
DDK's STL headers pick the two-argument string constructor), 0x00C38570 and 0x00C2CBD0 (root
base owner argument loaded before the `this` parking), 0x00C22100 (empty inline destructor local:
the DDK drops the EH frame), 0x00C17260 (LTCG register arguments).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets23`.
