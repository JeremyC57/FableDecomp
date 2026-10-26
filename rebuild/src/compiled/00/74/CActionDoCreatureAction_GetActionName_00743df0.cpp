class CCharString { public:
// Leaf: GetActionName
    CCharString(const char* lit, int len);
    int dummy;
};

class CCreatureActionBase { public:
    CCharString GetStrikeEventHelperName();
};

CCharString CCreatureActionBase::GetStrikeEventHelperName()
{
    return CCharString((const char*)0x122d70e, -1);
}