# Batch `2026-10-14_linux_eh16` — hand-authored small EH-framed bodies: 16 bodies (16 landed)

Sixteenth hand pass (Wine lane, `/EHsc`): a shutdown under the lock (release on `this`, two
member releases, an optional advance), the slot-1 observer notification with a refresh, the
four-argument member factory feeding the landed UI component constructor, an event-queue drain
(`for (item = Next(); item; item = Next()) Process(item)` on a polymorphic member), a `__stdcall`
helper call with a flag update and an inline listener poke on the owner, a three-term sum of
out-of-line queries, a `C3DVector(x, y, z)` frame copy passed by reference, the
`CSubPrimitiveList` constructor (two 0x30-byte nodes, zeroed counters and seven flags), the
`CTCFishingSpot` destructor (four separate 0x14-byte members, states 3..0, inlined base
destructor), the -1-defaulting reward lookup and its six-argument forwarder, `CSBAgents`
creation (`Resolve(name, &name)` then `new`), the `CScriptModuleBase` constructor, two typed
container destructors (drain loop through `__forceinline` virtual helpers, then the inlined
container and root destructors) and a two-container owner destructor.

Lessons kept: a destructor body in a `__declspec(novtable)` most-derived class that calls its
own virtuals needs `__forceinline` helpers to keep the calls virtual; an inline accessor on the
owner (`m_pOwner->PokeListener()`) keeps the owner in esi; `(long)` ternary into a named local
gives `or esi,-1`. Parked (see `docs/journal/FINDINGS_LOG.md`): 0x00C14BC0 (no-call scoped-lock
body, DDK elides the frame), 0x00C26CD0 / 0x00C27130 (vector insert temporary homed in the dead
argument slot, store/load order), 0x00C0A6D0 (DDK drops the dead EH state-1 store between two
inlined member constructors), 0x00C46860 (DDK rotates the member-drain loop), 0x00CAEFB0
(null-checked base conversions in inlined destructors: one check short), 0x00C16D90 (LTCG
register argument in edi).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets20`.
