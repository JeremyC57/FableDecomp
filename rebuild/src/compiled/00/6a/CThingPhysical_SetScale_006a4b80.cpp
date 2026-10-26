struct Sub64;
extern void __fastcall sub_4bfa50(Sub64* p, int edx, float scale);

class CThingPhysical { public:
    char pad[0x64];
    Sub64* f64;      // +0x64
    char pad2[0x9c - 0x64 - 4];
    float f9c;       // +0x9c
};

void __fastcall CThingPhysical_SetScale(CThingPhysical* physical, int edx, float scale)
{
    physical->f9c = scale;
    if (physical->f64)
        sub_4bfa50(physical->f64, edx, scale);
}