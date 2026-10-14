# Batch `2026-10-14_linux_eh11` — hand-authored small EH-framed bodies: 11 bodies (11 landed)

Eleventh hand pass (Wine lane, `/EHsc`): `CDisplayManager`'s destructor (its own virtual slot 5
through the constant vtable slot, a COM device released, a no-argument import), two block-freeing
destructors, a critical-section COM release returning 0, two scoped-lock setters, `CClickable::OnRightUnclicked`
(two stack visitors, EH state rising 0 to 1), the `CAIStateGroup_CreatureUseCoverBase` constructor
pairing with the eh9 destructor (body assignments to base members), a repeated-mesh constructor
allocating its index block, a landscape-patch destructor whose node-list member frees its nodes
inline (`for (node = head; node; ) { cur = node; node = node->next; operator delete(cur); }` is
the form that keeps the walker in esi), a stack helper queried and destroyed, and a two-argument
factory. Parked: `CDrawGuildSeal::~CDrawGuildSeal` @ 0x00C60930 (96 v 98: the retail loop copies
the head through ecx before testing it).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets15`.
