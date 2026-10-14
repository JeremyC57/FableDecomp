# Batch `2026-10-14_linux_eh20` — hand-authored small EH-framed bodies: 35 bodies (35 landed)

Twentieth hand pass (Wine lane, `/EHsc`), the largest so far: the `CShaderResourceArray` copy
constructor (source array parked in the dead argument slot), two critical-section render-effect
and audio-device bodies (one-word scoped section holders, COM `__stdcall` slots with `this`
pushed, `Sleep` / `SetEvent` imports), name-table setters whose string temporary is homed in the
dead index slot, a `__fastcall` processor with a stack request, item rebind / equip / pop bodies,
the `CPredicateOwner` constructor and destructor pair (two inline containers plus a registry),
the `CCombatScript` destructor (drain loop, polymorphic member, script root vtable last), two
`CMailboxQueue` message bodies, three clothing-suit resolvers and the clamped table query, the
`PKCTRArray.cpp(24)` not-morphable warning (chained `__fastcall` log builders), the audio
primitive add/remove/collect bodies (reference parameters cast to the enclosing primitive
without null checks), several inventory binding walks with counted or conditional actions, the
`CParticleTypePair` and `CLandscapeBackgroundTree` constructors, and container / theme-element
destructors and processors.

Lessons kept: a member destructor declared `throw()` as the last call of a destructor drops the
trailing `-1` state store; a reference parameter makes a downcast to the enclosing object
null-check free; `movsx` of a signed byte member feeds a long parameter; `setge` into a `short`
member is `m = r >= 0`; chained builder calls through eax/ecx are `__fastcall` members returning
`this`. Parked: 0x00C6AFC0 (two adjacent immediate stores swapped), 0x00C26B20 / 0x00C26F80
(the DDK folds `size() != 0` into a mask test), 0x00C28AE0 (frame kept with no state stores),
0x00C12680 / 0x00C12720 (register allocation), 0x00CA4F90 (ebx/ebp roles), 0x00C11520 (one store
scheduled around a push), 0x00C27550 (`this` reused for the pushed argument).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets24`.
