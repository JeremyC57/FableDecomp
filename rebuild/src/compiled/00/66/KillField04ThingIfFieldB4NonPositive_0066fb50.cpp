struct CThing_0066fb50
{
    unsigned char prefix[0xb4];
    float field_b4;

};

struct Object_0066fb50
{
    unsigned char prefix[4];
    CThing_0066fb50* thing;
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004C9B80 ?Kill@CThing@@QAEX_N@Z (was ?Kill@CThing_0066fb50@@QAEX_N@Z)
class CThing { public: void Kill(bool); };
extern "C" void __fastcall
KillField04ThingIfFieldB4NonPositive_0066fb50(
    Object_0066fb50* self)
{
    CThing_0066fb50* thing = self->thing;

    if (thing != 0 && thing->field_b4 <= 0.0f)
        reinterpret_cast<CThing*>(thing)->Kill(true);
}
