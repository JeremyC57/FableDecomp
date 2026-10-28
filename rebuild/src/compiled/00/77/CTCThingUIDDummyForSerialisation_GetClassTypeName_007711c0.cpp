// CTCThingUIDDummyForSerialisation::GetClassTypeName @ 007711c0
// __fastcall, returns CCharString by value (hidden retptr on stack, ret 4)

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

class CTCThingUIDDummyForSerialisation { public:
    int dummy;
    CCharString GetClassTypeName();
};

CCharString CTCThingUIDDummyForSerialisation::GetClassTypeName()
{
    return CCharString("SomeActionName", -1);
}