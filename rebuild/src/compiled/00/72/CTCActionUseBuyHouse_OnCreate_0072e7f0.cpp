// CTCActionUseBuyHouse::OnCreate  @ retail 0x0072e7f0
// Layout inferred from disasm:
//   push esi ; mov esi,ecx            -> save this
//   mov ecx,[esi+4]                   -> A = this->field4
//   mov ecx,[ecx+0x70]                -> B = A->field70
//   lea eax,[esi+0x14] ; push eax     -> arg = &this->field14
//   call 0x72ef60                     -> B->Sub(&this->field14)  (thiscall)
//   mov ecx,esi                       -> this
//   pop esi
//   jmp 0x7eac10                      -> tail base::OnCreate(this)

struct Field14 { int a, b, c, d; };

struct BObj {
    // thiscall method at 0x72ef60
    void Sub(Field14* p);
};

struct AObj {
    char pad[0x70];
    BObj* field70;   // +0x70
};

struct BaseAction {
};

struct CTCActionUseBuyHouse {
    void* vtbl;        // +0
    AObj* field4;      // +4
    char pad[0x14 - 8];
    Field14 field14;   // +0x14
    void OnCreate();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x007EAC10 ?OnCreate@CTCActionUseBase@@UAEXXZ (was ?OnCreate@BaseAction@@QAEXXZ)
class CTCActionUseBase { public: virtual void OnCreate(); };
void CTCActionUseBuyHouse::OnCreate()
{
    this->field4->field70->Sub(&this->field14);
    reinterpret_cast<CTCActionUseBase*>(((BaseAction*)this))->CTCActionUseBase::OnCreate();
}