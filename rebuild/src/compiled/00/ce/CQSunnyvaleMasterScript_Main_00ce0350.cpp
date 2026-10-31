// CQ_SunnyvaleMasterScript::Main @ 00ce0350
// Leaf: Main
struct F4 {
    struct VT {
        void (__fastcall *slot0)(F4*);
        void (__fastcall *slot1)(F4*);
        void (__fastcall *slot2)(F4*);
        void (__fastcall *slot3)(F4*);
        void (__fastcall *slot4)(F4*);
        void (__fastcall *slot5)(F4*);
        void (__fastcall *slot6)(F4*);
        void (__fastcall *slot7)(F4*);
    };
    VT* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* pad;   // +0
    F4*   f4;    // +4
};

class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)

void __fastcall Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        F4* p = self->f4;
        p->vtbl->slot7(p);
    } while (!((const CEntityScriptBase*)self)->IsActiveThreadTerminating());
}