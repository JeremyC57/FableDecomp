#ifndef FABLE_ENGINE_CTHINGFILTER_ENEMYCREATURE_H
#define FABLE_ENGINE_CTHINGFILTER_ENEMYCREATURE_H
// Ego_r.pdb CThingFilter_EnemyCreature (4 bytes: CThing* SourceThing +0); ctor ??0CThingFilter_EnemyCreature@@QAE@ABVCThing@@@Z
// (inline in retail: 0x00603000 stores the hero pointer in place); operators ??R..@@QBE_NABVCThing@@@Z / QBE_NPBVCThing@@@Z.
class CThing;
class CThingFilter_EnemyCreature {
    CThing* SourceThing;
public:
    CThingFilter_EnemyCreature(const CThing& sourceThing):SourceThing(const_cast<CThing*>(&sourceThing)){}
    bool operator()(const CThing&) const;
    bool operator()(const CThing*) const;
};
#endif
