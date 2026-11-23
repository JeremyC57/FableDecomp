// Leaf: ?GetDazedHealthThreshold@CThingCreatureBase@@QBEMXZ
struct CDazedDef {
    char pad[0x84];
    int dazedThreshold; /* +0x84 */
};
class CThingCreatureBase { public:
    char pad[0xe0];
    CDazedDef* pDef; /* +0xe0 */
    public: float GetDazedHealthThreshold() const;
};
float CThingCreatureBase::GetDazedHealthThreshold() const
{
    CThingCreatureBase* creature = const_cast<CThingCreatureBase*>(this);
    return (float)creature->pDef->dazedThreshold;
}