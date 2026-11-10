#ifndef FABLE_ENGINE_DIRECTION_NEAREST_COMPARATOR_H
#define FABLE_ENGINE_DIRECTION_NEAREST_COMPARATOR_H
// Retail opt-in (Codex pass 18): original anonymous-namespace class in both
// cover TUs; Ego_r/FableWin name sets and Pos/Dir type records establish this identity.
namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {
public:
    C2DVector Pos;                              // +0x00
    C2DVector Dir;                              // +0x08
    float GetWeightFromParams(float dot, float distance) const
    {
        return (float)pow(dot, (float)((long)distance / 4 * 2 + 1));
    }
    bool operator()(const CThing* a, const CThing* b) const;
};
}
#endif
