// CQ_SunnyvaleMasterScript::Main @ 0x00dec570
// Leaf: Main
// __fastcall void Main(This*)

struct Sub;

struct SubVtbl {
    void (__fastcall *slot0)(Sub*);
    void (__fastcall *slot1)(Sub*);
    void (__fastcall *slot2)(Sub*);
    void (__fastcall *slot3)(Sub*);
    void (__fastcall *slot4)(Sub*);
    void (__fastcall *slot5)(Sub*);
    void (__fastcall *slot6)(Sub*);
    void (__fastcall *slot7)(Sub*);   // +0x1c
};

struct Sub {
    SubVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;      // +0x00
    Sub*  f4;      // +0x04
};

// helper @ 0xf35b30, __fastcall(this) -> bool
class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        Sub* p = self->f4;
        p->vtbl->slot7(p);
    } while (!((const CEntityScriptBase*)self)->IsActiveThreadTerminating());
}