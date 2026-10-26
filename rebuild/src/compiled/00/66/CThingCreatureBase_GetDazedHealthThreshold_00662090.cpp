struct CDazedDef {
    char pad[0x84];
    int dazedThreshold; /* +0x84 */
};
class CThingCreatureBase { public:
    char pad[0xe0];
    CDazedDef* pDef; /* +0xe0 */
};
float __fastcall CThingCreatureBase_GetDazedHealthThreshold(CThingCreatureBase* creature)
{
    return (float)creature->pDef->dazedThreshold;
}