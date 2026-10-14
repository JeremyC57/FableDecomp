# Batch `2026-10-14_linux_eh9` — hand-authored small EH-framed bodies: 11 bodies (11 landed)

Ninth hand pass (Wine lane, `/EHsc`): block-holder and COM-release destructors, `CBankFile`'s
progress object created through the class operator new (stored at +0xe4, returns 0) and deleted
through its virtual deleting destructor in `~CBankFile`, an AI state-group destructor with EH state
1 over a cleanup call, a `novtable` destructor over six 0x17c-byte elements (`__ehvec_dtor`), two
null-checked scoped-lock updates, `CCombatActionBase::Clone` (`new T(*this)` with two epilogues),
`CGuiWindowManager::RemoveExclusiveForegroundWindow` (critical-section lock, COM `AddRef`, the
pointer re-read for the return), and the `CVolume` destructor pair (raw `::operator delete` of a
block in the body, then the members; the derived releases a COM interface and calls the base).
Parked: `CParticleEffect::~CParticleEffect` @ 0x00C3F3D0 (the retail `shr eax,1Fh ; not al ;
test al,1` test of a bit-31 bitfield comes out as `test eax,eax ; js` here).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets13`.
