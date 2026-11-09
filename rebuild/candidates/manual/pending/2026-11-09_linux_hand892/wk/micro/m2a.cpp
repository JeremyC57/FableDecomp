#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
bool Term();
struct SS {
    char pad[0x40];
    CGameScriptInterfaceBase* Game;
    std::vector<CScriptThing> V;
    void f(const CScriptThing& spy);
};
void SS::f(const CScriptThing& spy)
{
    for (unsigned int i = 0; i < V.size(); ++i) {
        if (V[i].IsAlive()) {
            if (Term()) return;
            Game->EntitySetThingAsEnemyOfThing(V[i], spy);
            Game->EntitySetThingAsEnemyOfThing(Game->GetHero(), V[i]);
            Game->EntitySetThingAsEnemyOfThing(V[i], Game->GetHero());
        }
    }
}
