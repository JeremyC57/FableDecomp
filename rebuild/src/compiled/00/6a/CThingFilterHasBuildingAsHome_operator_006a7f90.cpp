// CThingFilter_HasBuildingAsHome::operator()  @ 006a7f90
// return ((Subobject*)((char*)creature + 0x170))->GetId() == this->m_expected;

struct Subobject
{
    int GetId(); // thiscall @ 0xa01b50, ecx=this, returns int
};

class CThingAICreature
{ public:
    char pad[0x174];
};

class CThingFilter_HasBuildingAsHome
{ public:
    int m_expected; // +0x00

    bool operator()(CThingAICreature* creature);
};

bool CThingFilter_HasBuildingAsHome::operator()(CThingAICreature* creature)
{
    Subobject* s = (Subobject*)((char*)creature + 0x170);
    return s->GetId() == m_expected;
}