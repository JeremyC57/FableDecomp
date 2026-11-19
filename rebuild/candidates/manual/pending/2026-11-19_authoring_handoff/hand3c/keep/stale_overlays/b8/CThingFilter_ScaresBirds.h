#ifndef FABLE_ENGINE_CTHINGFILTER_SCARESBIRDS_H
#define FABLE_ENGINE_CTHINGFILTER_SCARESBIRDS_H
// CThingFilter_ScaresBirds (new shared header, hand3c, row 0x00914650 CAIStateGroup_BirdStartled::IsRunnable). Ego_r.pdb: 4 bytes,
// Bird (CThingAICreature*) +0; no vtable (the row builds it as one stored pointer, 0x0091473B). FableWin.pdb
// ??0CThingFilter_ScaresBirds@@QAE@ABVCThingAICreature@@@Z (0x02B8D8A0, inline in retail) and the test
// ??RCThingFilter_ScaresBirds@@QBE_NABVCThingCreatureBase@@@Z (0x02B8E030; inline in retail, expanded in the search instance
// 0x009147E0). The test is declared only here.
class CThingAICreature;
class CThingCreatureBase;
class CThingFilter_ScaresBirds {
public:
    CThingFilter_ScaresBirds(const CThingAICreature& bird) : Bird(const_cast<CThingAICreature*>(&bird)) { }
    bool operator()(const CThingCreatureBase& thing) const;
private:
    CThingAICreature* Bird;                                              // +0x00
};
#endif // FABLE_ENGINE_CTHINGFILTER_SCARESBIRDS_H
