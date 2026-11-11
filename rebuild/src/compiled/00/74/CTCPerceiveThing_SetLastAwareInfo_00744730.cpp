// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
struct Vec3 { float x, y, z; };

struct CObj34 { char pad[0x44]; float f44; };

class CTCPerceiveThing { public:
    char pad0[4];      // +0x00
    int field4;        // +0x04
    char pad8[4];      // +0x08
    char field_c[0xc]; // +0x0c getter subobject (address taken)
    float f18;         // +0x18
    char pad1c[4];     // +0x1c
    Vec3 v20;          // +0x20 (12 bytes -> +0x2c)
    char pad2c[8];     // +0x2c
    CObj34* p34;       // +0x34
};

struct GObj {
    void NotifyA(int arg);   // 0xffcf31c0
    void NotifyB(int arg);   // 0xffcf31d0
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
extern "C" Vec3* __fastcall Getter_d82ca0(void* self);   // 0xffd82ca0
extern GObj* g_obj_13b8790;                              // ds:0x13b8790

void __fastcall CTCPerceiveThing_SetLastAwareInfo(CTCPerceiveThing* self)
{
    void* sub = ((CBaseIntelligentPointer*)(self->field_c))->GetPItem();
    Vec3* v = Getter_d82ca0(sub);
    self->v20 = *v;

    float val = self->p34->f44;
    self->f18 = val;

    if (val != 0.0f) {
        if (((CBaseIntelligentPointer*)(self->field_c))->GetPItem()) {
            g_obj_13b8790->NotifyA(self->field4);
        } else {
            g_obj_13b8790->NotifyB(self->field4);
        }
    }
}