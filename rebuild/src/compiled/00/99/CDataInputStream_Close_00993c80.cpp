// Leaf: ?CDataInputStream_Close@@YIXPAUDS@@@Z
struct DS { char pad[4]; int f4; int f8; int fc; int f10; int f14; };
void __fastcall CDataInputStream_Close(DS* self){ self->f4=0; self->f8=0; self->f10=0; self->f14=0; self->fc=0; }
// Stand-in (owner rule 2026-11-05): CDataInputStream_Close for CDataInputStream::Close; RTTI vtable slot ?Close@CDataInputStream@@UAEXXZ + PDB name set (egor/fablewin).
