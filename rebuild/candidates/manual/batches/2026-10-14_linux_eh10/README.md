# Batch `2026-10-14_linux_eh10` — hand-authored small EH-framed bodies: 11 bodies (11 landed)

Tenth hand pass (Wine lane, `/EHsc`): three scoped-lock queries through virtual slot 15 of an
embedded member, `CLandscapeBackgroundPatch`'s destructor (raw `::operator delete` of the texture
block), the `CParticleSet` constructor pairing with the eh9 destructor (six elements through
`__ehvec_ctor`), a null-tested scoped-lock rebuild, `CAInputProcess` destructors (a handle released
when a device is set; an owned object deleted through its out-of-line destructor), an audio
manager clearing a pending pointer under its critical section and notifying through virtual slot
24, and two more scoped-lock forwarders (a returned result, a forwarded argument).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets14`.
