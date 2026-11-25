// CTCActionUseReadable::OnCreate  @ 0x007e8140
struct Sub;
struct Consumer;

struct Consumer {
    void Consume(int* arg);   // direct thiscall target 0x7e8200
};
struct Sub {
    char      pad[0x70];
    Consumer* f70;      // +0x70
};
struct Owner {
    void* f0;
    Sub*  ParentThing;           // +4
    char  pad[0x14 - 8];
    int   f14;          // +0x14 (arg passed by address)
    void  OnCreate();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x007EAC10 ?OnCreate@CTCActionUseBase@@UAEXXZ (was ?Base_OnCreate@Owner@@QAEXXZ)
class CTCActionUseBase { public: virtual void OnCreate(); };
void Owner::OnCreate()
{
    reinterpret_cast<CTCActionUseBase*>(this)->CTCActionUseBase::OnCreate();
    this->ParentThing->f70->Consume(&this->f14);
}