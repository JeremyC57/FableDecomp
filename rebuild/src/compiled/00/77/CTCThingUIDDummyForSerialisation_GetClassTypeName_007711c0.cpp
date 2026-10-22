// CTCThingUIDDummyForSerialisation::GetClassTypeName @ 007711c0
// __fastcall, returns CCharString by value (hidden retptr on stack, ret 4)

struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

struct CTCThingUIDDummyForSerialisation {
    int dummy;
    CCharString GetClassTypeName();
};

CCharString CTCThingUIDDummyForSerialisation::GetClassTypeName()
{
    return CCharString("SomeActionName", -1);
}