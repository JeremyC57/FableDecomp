// st.h: the shared CScriptThing model (CBaseClass virtual dtor out of line, CCountedPointer release, plain-inline virtual dtor)
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
struct CScriptThingImpl { virtual bool IsAlive() const; };
class CScriptThing : public CBaseClass { public:
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing() {}
    bool IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
};
bool Term(); bool Test(int); void G(int); void Use(const CScriptThing&); CScriptThing Get();
