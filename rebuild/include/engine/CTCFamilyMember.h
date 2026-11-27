#ifndef FABLE_ENGINE_CTCFAMILYMEMBER_H
#define FABLE_ENGINE_CTCFAMILYMEMBER_H
#ifdef FABLE_RETAIL_AI_RELATIONSHIPS_PASS29
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"
class CThing;
class CTCFamilyMember:public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_FAMILY_MEMBER;}
    void AddRelative(const CThing&);
private:
    std::vector<const CThing*> FamilyMembers;
};
FABLE_STATIC_ASSERT(sizeof(CTCFamilyMember)==24);
#endif
#endif
