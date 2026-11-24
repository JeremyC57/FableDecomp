# NF308 lessons (2026-11-23)

- **Read the owner's HEAD object before any binder fix.** Three of the targets I touched had owners that define the stand-in themselves: 0x006EA2C0 had a bare Leaf, 0x009E9F20 and 0x0097E330 had no Leaf at all. A bare or missing Leaf is the warning sign (GOTCHAS 513).
  - A binder-only fix there turns a resolved reference into an unresolved one.
  - The fix is owner plus every old-spelling binder together (NF304 Q6), or nothing. Entry 179 is held for exactly this reason.
- **A free function in the wrong namespace is the cheapest in-place shape.**
  - Write `namespace N { decl } using N::name;` on the declaration's own line. Nothing else in the TU changes, so objsec is ALL_EQ.
  - mdiff.py tags these OTHER or SCOPE+SIG, so diff the parameter lists by eye. Most stand-ins of the NCreatureMode / NCreatureAI kind also flip `CThing*` to `const CThing&`, and those are ports.
- **A "held for a sibling" entry can have gone stale.** Entry 19's source had been fixed by an earlier wave. Only the table line still had the old witness, and one HEAD compile per caller settled it.
- **Re-scoping a local base of a class with two polymorphic bases renames the emitted vftable.** MSVC names it `??_7Derived@@6BBase@@@`. Check for multiple inheritance before calling a base re-scope "no vftable".
- **Old respells can leave access labels at the END of a declaration line** (`void F(); protected:`). That label applies to the next member, not this one: read the line before trusting callcensus's "public".
- **Precheck with absolute paths:** tools/pre.sh `cd`s into work/, so a relative rows/ path "fails" with file-not-found.
