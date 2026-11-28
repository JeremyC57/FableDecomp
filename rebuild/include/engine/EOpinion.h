#ifndef FABLE_ENGINE_EOPINION_H
#define FABLE_ENGINE_EOPINION_H
// Both original PDB TPI enum records; native greeting branch constants agree.
enum EOpinion {
    OPINION_MORALITY = 0,
    OPINION_RENOWN = 1,
    OPINION_SCARINESS = 2,
    OPINION_AGREEABLENESS = 3,
    OPINION_ATTRACTIVENESS = 4,
    OPINION_LAST = 5
};
#ifdef FABLE_RETAIL_GET_OPINIONS_PASS30
inline EOpinion& FABLE_FASTCALL operator++(EOpinion& value) { return value = (EOpinion)(value + 1); }
#endif
#endif
