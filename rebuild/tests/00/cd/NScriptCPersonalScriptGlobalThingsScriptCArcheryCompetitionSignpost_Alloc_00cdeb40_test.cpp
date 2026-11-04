// Behaviour test for CreateArcheryCompetitionSignpost (0x00CDEB40): the entity-script factory builds the signpost
// script (Game from the parent's +0x40 interface, Me copied, ParentClass, PMasterData) and wraps it in a counted
// pointer (count block {1, object}).
#include <cstdio>

class CGameScriptInterfaceBase { public: int id; };
class CScriptDataBase { public: int id; };
static int g_thingCopies = 0;
class CScriptThing {
public:
    int id;
    CScriptThing(int i) : id(i) { }
    CScriptThing(const CScriptThing& o) : id(o.id) { ++g_thingCopies; }
};
class CPersonalScript_GlobalThingsScript {
public:
    CGameScriptInterfaceBase* Game;
};
class CEntityScriptBase {
public:
    CEntityScriptBase(CGameScriptInterfaceBase* game, const CScriptThing& me) : Game(game), Me(me) { }
    virtual ~CEntityScriptBase() { }
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
};
struct CCountBlock { long references; CEntityScriptBase* object; };
class CEntityPointer {
public:
    CEntityScriptBase* PObject;
    CCountBlock* PCount;
    CEntityPointer(CEntityScriptBase* object) : PObject(object)
    {
        if (object) { PCount = new CCountBlock; PCount->references = 1; PCount->object = object; }
        else PCount = 0;
    }
};
class CArcheryCompetitionSignpost : public CEntityScriptBase {
public:
    CArcheryCompetitionSignpost(CPersonalScript_GlobalThingsScript* parent, CScriptDataBase* data, const CScriptThing& me)
        : CEntityScriptBase(parent->Game, me), ParentClass(parent), PMasterData(data) { }
    CPersonalScript_GlobalThingsScript* ParentClass;
    CScriptDataBase* PMasterData;
};

CEntityPointer CreateArcheryCompetitionSignpost(CPersonalScript_GlobalThingsScript* parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CEntityPointer(new CArcheryCompetitionSignpost(parent, data, me));
}

int main()
{
    CGameScriptInterfaceBase game; game.id = 3;
    CPersonalScript_GlobalThingsScript parent; parent.Game = &game;
    CScriptDataBase data; data.id = 4;
    CScriptThing signpost(55);
    CEntityPointer p = CreateArcheryCompetitionSignpost(&parent, &data, signpost);
    CArcheryCompetitionSignpost* s = (CArcheryCompetitionSignpost*)p.PObject;
    if (s == 0 || p.PCount == 0 || p.PCount->references != 1 || p.PCount->object != s || s->Game != &game
        || s->Me.id != 55 || s->ParentClass != &parent || s->PMasterData != &data || g_thingCopies != 1) {
        std::printf("00cdeb40_TEST FAIL\n");
        return 1;
    }
    delete p.PCount;
    delete s;
    std::printf("00cdeb40_TEST PASS\n");
    return 0;
}