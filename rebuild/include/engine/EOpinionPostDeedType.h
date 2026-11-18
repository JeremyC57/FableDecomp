#ifndef FABLE_ENGINE_EOPINIONPOSTDEEDTYPE_H
#define FABLE_ENGINE_EOPINIONPOSTDEEDTYPE_H
// New header (egod3a, row 0x00803280): enum EOpinionPostDeedType with the Ego_d.pdb enumerators (types.txt LF_FIELDLIST, the
// recipient / witness bit set PostDeed 0x006C6990 takes by value). Do not include together with fable_nscript_forward.h, which
// declares an empty stand-in of the same enum.
enum EOpinionPostDeedType {
    OPINION_POST_DEED_TYPE_NULL = 0,
    OPINION_POST_DEED_TYPE_RECIPIENT = 1,
    OPINION_POST_DEED_TYPE_WITNESSES = 2,
    OPINION_POST_DEED_TYPE_RECIPIENT_AND_WITNESSES = 3,
    OPINION_POST_DEED_TYPE_VILLAGE = 4,
    OPINION_POST_DEED_TYPE_ALL = 7,
    OPINION_POST_DEED_TYPE_GUARDS = 16
};
#endif
