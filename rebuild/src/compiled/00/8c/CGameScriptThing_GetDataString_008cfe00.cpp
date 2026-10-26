struct Arg {
    void Consume(void* p);
};

class CGameScriptThing { public:
    char pad[0x24];
    char data;
};

void* __fastcall GetDataString(CGameScriptThing* thing, int /*edx*/, void* arg)
{
    ((Arg*)arg)->Consume(&thing->data);
    return arg;
}