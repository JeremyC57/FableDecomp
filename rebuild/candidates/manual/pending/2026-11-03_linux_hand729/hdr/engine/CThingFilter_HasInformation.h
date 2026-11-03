#ifndef FABLE_ENGINE_CTHINGFILTER_HASINFORMATION_H
#define FABLE_ENGINE_CTHINGFILTER_HASINFORMATION_H
// Ego_r.pdb CThingFilter_HasInformation (empty, 1 byte); ??RCThingFilter_HasInformation@@QBE_NABVCThingAICreature@@@Z.
class CThingAICreature;
class CThingFilter_HasInformation {
public:
    bool operator()(const CThingAICreature&) const;
};
#endif
