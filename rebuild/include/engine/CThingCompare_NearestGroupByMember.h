#ifndef FABLE_ENGINE_NEARESTGROUPBYMEMBER_H
#define FABLE_ENGINE_NEARESTGROUPBYMEMBER_H
// Retail opt-in (Codex pass18): both PDB name sets and native00935040 caller
// establish the original member template. Pos is the original12-byte member.
class CThingCompare_NearestGroupByMember {
public:
    C3DVector Pos;                                                  // +0x00
    template <class T> T* operator()(T* a, T* b) const
    {
        if (!a)
            return b;
        if (!b)
            return a;
        const CTCCreatureGroup* groupA;
        if(a->PeekTC(&groupA)) {
            const CTCCreatureGroup* groupB;
            if(b->PeekTC(&groupB)) {
                const CThingAICreature* nearestA=groupA->GetNearestMember(Pos);
                const CThingAICreature* nearestB=groupA->GetNearestMember(Pos);
                if(!nearestA)return b;
                if(!nearestB)return a;
                if((nearestA->GetPos()-Pos).GetSquaredMagnitude() < (nearestB->GetPos()-Pos).GetSquaredMagnitude())return a;
                return b;
            }
            return a;
        }
        return b;
    }
};
#endif
