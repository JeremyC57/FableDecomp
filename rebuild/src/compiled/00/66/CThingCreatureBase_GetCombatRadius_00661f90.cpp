// NF261 (2026-11-22, q12742): retail reads 0x0122F59C (`fmul dword ptr [0x0122F59C]` at 0x00661F9C), the VC7.1 pooled float literal 0.5f
//   (0x3F000000; 1,568 references in Fable.exe, read by 498 landed rows), not a named global: the made-up extern `g_scale` (?g_scale@@3MA, this
//   row its only spelling) is the literal 0.5f, which binds __real@3f000000 there. g_thresh (0x01230A5C) is left as it was (not this entry).
#pragma optimize("g",on)
struct Inner { char pad[0x134]; float f134; };
struct Other { char pad[0x34]; float f34; };
class CThingCreatureBase { public:
    char pad0[0x60];
    Other* q;
    char pad1[0xe0 - 0x64];
    Inner* p;
};
extern float g_thresh;

float __fastcall CThingCreatureBase_GetCombatRadius(const CThingCreatureBase* creature)
{
    float t = creature->p->f134;
    if (t * 0.5f > g_thresh)
        return t * 0.5f;
    return creature->q->f34;
}