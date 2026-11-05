// Leaf: ??RCThingFilter_HasDefName@@QBE_NABVCThing@@@Z
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

class CDefString { public:
    void* p;
    bool operator==(const CCharString& rhs) const;
};

class CThingFilter_HasDefName { public:
    CCharString m_name;
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasDefName::operator()(const CThing& thing) const
{
    return thing.GetDefName() == m_name;
}