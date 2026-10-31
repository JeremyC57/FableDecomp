struct F4;
// Leaf: Main
struct F4Vtbl { void (__fastcall *slot[8])(F4*); };
struct F4 { F4Vtbl* vtbl; };
struct CQ_SunnyvaleMasterScript { void* f0; F4* f4; };

class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)

void __fastcall Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        F4* p = self->f4;
        p->vtbl->slot[7](p);
    } while (!((char)((const CEntityScriptBase*)self)->IsActiveThreadTerminating()));
}