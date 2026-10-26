// CTCMapwho::GetClassTypeName @ 0071a7f0
class CCharString { public:
    char* storage_;
    CCharString(const char* text, long length);
};

class CTCMapwho { public:
    int dummy;
    CCharString GetClassTypeName();
};

CCharString CTCMapwho::GetClassTypeName()
{
    return CCharString("ACTION_DO_CREATURE_ACTION", -1);
}