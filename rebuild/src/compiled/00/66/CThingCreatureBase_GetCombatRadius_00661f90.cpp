#pragma optimize("g",on)
struct Inner { char pad[0x134]; float f134; };
struct Other { char pad[0x34]; float f34; };
class CThingCreatureBase { public:
    char pad0[0x60];
    Other* q;
    char pad1[0xe0 - 0x64];
    Inner* p;
};
extern float g_scale;
extern float g_thresh;

float __fastcall CThingCreatureBase_GetCombatRadius(const CThingCreatureBase* creature)
{
    float t = creature->p->f134;
    if (t * g_scale > g_thresh)
        return t * g_scale;
    return creature->q->f34;
}