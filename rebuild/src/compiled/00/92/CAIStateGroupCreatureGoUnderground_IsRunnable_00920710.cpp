// Leaf: ?CAIStateGroup_CreatureGoUnderground_IsRunnable@@YIDPAURN@@@Z
char __fastcall h_920710(void* nested);
struct RN { void* f0; char* f4; };
char __fastcall CAIStateGroup_CreatureGoUnderground_IsRunnable(RN* self){ char* p4 = self->f4; void* nested = *(void**)(p4 + 0x18); return h_920710(nested) == 0; }
// Stand-in (owner rule 2026-11-05): CAIStateGroup_CreatureGoUnderground_IsRunnable for CAIStateGroup_CreatureGoUnderground::IsRunnable; RTTI vtable slot ?IsRunnable@CAIStateGroup_CreatureGoUnderground@@MAE_NXZ + PDB name set (egor/fablewin).
