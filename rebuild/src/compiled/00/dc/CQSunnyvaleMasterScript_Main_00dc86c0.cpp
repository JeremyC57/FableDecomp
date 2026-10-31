// CQ_SunnyvaleMasterScript::Main @ 0x00dc86c0
// Leaf: Main
// __fastcall(void) modeled as __fastcall(self)

struct CObjWithVtbl;

struct CObjVtbl {
    void (__fastcall *slot0)(CObjWithVtbl*);
    void (__fastcall *slot1)(CObjWithVtbl*);
    void (__fastcall *slot2)(CObjWithVtbl*);
    void (__fastcall *slot3)(CObjWithVtbl*);
    void (__fastcall *slot4)(CObjWithVtbl*);
    void (__fastcall *slot5)(CObjWithVtbl*);
    void (__fastcall *slot6)(CObjWithVtbl*);
    void (__fastcall *slot7)(CObjWithVtbl*);   // +0x1c
};

struct CObjWithVtbl {
    CObjVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void*         f0;
    CObjWithVtbl* f4;   // +0x4
};

// helper @ 0xf35b30 : __fastcall(this) -> char
class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)

void __fastcall CQ_SunnyvaleMasterScript_Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        self->f4->vtbl->slot7(self->f4);
    } while (!((char)((const CEntityScriptBase*)self)->IsActiveThreadTerminating()));
}