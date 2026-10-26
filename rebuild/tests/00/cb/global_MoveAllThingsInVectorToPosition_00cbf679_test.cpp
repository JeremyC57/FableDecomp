// Behaviour test for MoveAllThingsInVectorToPosition (0x00CBF679): every alive thing is put under scripted control
// (priority 4) through one shared resource object, which then walks it to the position.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:
    int id;
    bool alive;
    bool IsAlive() const { return alive; }
};
struct CThingVector {
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    const CScriptThing* begin() const { return _M_start; }
    const CScriptThing* end() const { return _M_finish; }
};
enum EScriptEntityMoveType { MOVE_TYPE_WALK = 0 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
static int g_objects = 0;
class CScriptGameResourceObjectScriptedThingBase { public:
    int current;
    int moves;
    int badMoves;
    CScriptGameResourceObjectScriptedThingBase() : current(-1), moves(0), badMoves(0) { ++g_objects; }
    ~CScriptGameResourceObjectScriptedThingBase() { --g_objects; }
    void MoveToPosition(const C3DVector& p, float speed, EScriptEntityMoveType type, bool a, bool b)
    {
        ++moves;
        if (p.X != 1.0f || p.Y != 2.0f || p.Z != 3.0f || speed != 0.0f || type != MOVE_TYPE_WALK || a || !b) ++badMoves;
    }
};
class CGameScriptInterface { public:
    int started[8];
    int count;
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority)
    {
        if (priority == SCRIPT_AI_PRIORITY_4) started[count++] = thing.id;
        object.current = thing.id;
        return true;
    }
};
CGameScriptInterface* g_pGameScriptInterface = 0;
static int g_maxObjects = 0;

void MoveAllThingsInVectorToPosition(const CThingVector& things, const C3DVector& position, int* movesOut, int* badOut)
{
    CScriptGameResourceObjectScriptedThingBase object;
    if (g_objects > g_maxObjects) g_maxObjects = g_objects;
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it) {
        if (it->IsAlive()) {
            g_pGameScriptInterface->StartScriptingEntity(*it, object, SCRIPT_AI_PRIORITY_4);
            object.MoveToPosition(position, 0.0f, MOVE_TYPE_WALK, false, true);
        }
    }
    *movesOut = object.moves;
    *badOut = object.badMoves;
}

int main()
{
    CScriptThing t[3];
    t[0].id = 1; t[0].alive = true;
    t[1].id = 2; t[1].alive = false;
    t[2].id = 3; t[2].alive = true;
    CThingVector v; v._M_start = t; v._M_finish = t + 3;
    CGameScriptInterface gsi; gsi.count = 0;
    g_pGameScriptInterface = &gsi;
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    int moves = 0, bad = 0;
    MoveAllThingsInVectorToPosition(v, p, &moves, &bad);
    if (gsi.count != 2 || gsi.started[0] != 1 || gsi.started[1] != 3 || moves != 2 || bad || g_objects != 0 || g_maxObjects != 1) {
        std::printf("00cbf679_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbf679_TEST PASS\n");
    return 0;
}