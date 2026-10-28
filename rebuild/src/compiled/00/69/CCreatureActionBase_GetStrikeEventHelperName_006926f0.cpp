#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    CCharString(const char* lit, long len);
    int dummy;
};

struct CCreatureActionBase_Methods : CCreatureActionBase {
    CCharString GetStrikeEventHelperName();
};

CCharString CCreatureActionBase_Methods::GetStrikeEventHelperName()
{
    return CCharString((const char*)0x122d70e, -1);
}