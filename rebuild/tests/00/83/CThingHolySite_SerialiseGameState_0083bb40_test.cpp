// Behaviour test for 0083bb40: the row body verbatim over fixture models of the shared-header interfaces (CThing::PeekDefinitionManager, CDefinitionManager::GetDef<T>, the counted CDefPointer, the base-class calls the row makes by name).
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixture models of the shared-header interfaces: a counted def (count starts at 1 for the manager's own reference, Delete counted),
// the counted CDefPointer handle, and the definition manager's GetDef<T> (fills the handle with an add-ref, returns whether found).
class CDefPointeeBase { public:
    long Refs; int Deleted;
    CDefPointeeBase() : Refs(1), Deleted(0) { }
    void Delete() { ++Deleted; }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { if (Object) { if (--Object->Refs == 0) Object->Delete(); } }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Set(CDefPointeeBase* p) { if (Object && --Object->Refs == 0) Object->Delete(); Object = p; if (p) ++p->Refs; }
};
class CGameDefinitionManager { public:
    CDefPointeeBase* Table[16]; mutable long Calls; mutable long LastIndex;
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        ++Calls; LastIndex = index;
        if (index > 0 && index < 16 && Table[index]) { out->Set(Table[index]); return true; }
        return false;
    }
};
static CGameDefinitionManager g_manager;
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
class C3DVector { public: float X, Y, Z; };
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
class CPersistContext { public: long Mode; bool IsSaving() const { return Mode == 1 || Mode == 3; } };
class CThingObjectDef : public CDefPointeeBase { public: float Health; long Material; };
class CThingHolySiteDef : public CDefPointeeBase { public: bool PlayerStart; };
class CThingCreatureDef : public CDefPointeeBase { public: };
class CThingPhysicalSwitchDef : public CDefPointeeBase { public: };
class CThingMarkerDef : public CDefPointeeBase { public: CEngineGraphic Graphic; unsigned long Properties; };
class CThingVillageDef : public CDefPointeeBase { public: CEngineGraphic Graphic; };
class CAppearanceDef : public CDefPointeeBase { public: CEngineGraphic Graphic; };
class CMaterialDef : public CDefPointeeBase { };
static CAppearanceDef* g_appearance = 0;
class CThingBaseDef : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const { if (!g_appearance) return false; out->Set(g_appearance); return true; }
};
class CThing { public:
    CDefPointer<const CThingBaseDef> PBaseDef;
    unsigned short DefGlobalIndex; float MaxHealth, Health; bool BaseResult; int BaseCalls; long LastDef; const char* LastScript;
    long LastPlayer; unsigned long LastFlags; const char* LastName; CThing* LastCreator; CThing* LastOriginal; bool LastFlag;
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_manager; }
};
typedef CThing CThingPhysicalBase;
class CThingPhysical : public CThing { public:
    bool SerialiseGameState(CPersistContext&) { ++BaseCalls; return BaseResult; }
};
class CThingGameObject : public CThingPhysical { public:
    bool ConstructFromLevelScript(const char* s) { ++BaseCalls; LastScript = s; return BaseResult; }
    bool ConstructFromParams(long d, const C3DVector&, long player, unsigned long flags, const CCharString& name, CThingPhysical* creator,
                             CThingPhysical* original, bool flag)
    {
        ++BaseCalls; LastDef = d; LastPlayer = player; LastFlags = flags; LastName = name.Text; LastCreator = creator; LastOriginal = original;
        LastFlag = flag; DefGlobalIndex = (unsigned short)d; return BaseResult;
    }
};
class CThingObject : public CThingGameObject { public:
    CDefPointer<const CThingObjectDef> PDef;
    const CThingObjectDef& PeekDefinition() const { return *PDef; }
    bool SerialiseGameState(CPersistContext& context);
    bool ConstructFromLevelScript(const char* script);
    CDefPointer<const CMaterialDef> PeekPMaterial() const;
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, unsigned long flags, const CCharString& scriptName);
};
class CThingHolySite : public CThingGameObject { public:
    CDefPointer<const CThingHolySiteDef> Def; unsigned char PlayerStart;
    const CThingHolySiteDef& PeekDefinition() const { return *Def; }
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, bool playerStart);
    bool SerialiseGameState(CPersistContext& context);
    bool ConstructFromLevelScript(const char* script);
};
class CThingCreatureBase : public CThingGameObject { public:
    CDefPointer<const CThingCreatureDef> PDef;
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, CThingPhysical* creator,
                             CThingPhysical* originalCreator, const CCharString& scriptName, bool flag);
    bool SerialiseGameState(CPersistContext& context);
    bool ConstructFromLevelScript(const char* script);
};
class CThingDeadCreature : public CThingGameObject { public: const CThingCreatureDef& PeekDefinition() const; };
class CThingPhysicalSwitch : public CThingGameObject { public: const CThingPhysicalSwitchDef& PeekDefinition() const; };
class CThingMarker : public CThingGameObject { public:
    CDefPointer<const CThingMarkerDef> PeekDefinition() const
    {
        CDefPointer<const CThingMarkerDef> def;
        PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
        return def;
    }
    CEngineGraphic GetCurrentGraphic() const;
    unsigned long GetProperties() const;
};
class CThingVillage : public CThingGameObject { public:
    mutable CEngineGraphic CurrentGraphic;
    CDefPointer<const CThingVillageDef> PeekDefinition() const
    {
        CDefPointer<const CThingVillageDef> def;
        PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
        return def;
    }
    const CEngineGraphic* GetCurrentGraphic() const;
};
static void Reset() { memset(&g_manager, 0, sizeof(g_manager)); g_appearance = 0; }
template <class T> static void Zero(T& t) { memset(&t, 0, sizeof(t)); }
// ---- row body (verbatim) ----
bool CThingHolySite::SerialiseGameState(CPersistContext& context)
{
    if (CThingPhysical::SerialiseGameState(context)) {
        if (!context.IsSaving()) {
            PeekDefinitionManager().GetDef(DefGlobalIndex, &Def);
            PlayerStart = PeekDefinition().PlayerStart != 0;
        }
        return true;
    }
    return false;
}
// ---- harness ----
int main()
{
    Reset(); CThingHolySiteDef def; def.PlayerStart = true; g_manager.Table[7] = &def; CPersistContext ctx;
    CThingHolySite h; Zero(h); h.BaseResult = true; h.DefGlobalIndex = 7;
    ctx.Mode = 3; CHECK(h.SerialiseGameState(ctx)); CHECK(h.Def.Object == 0 && h.PlayerStart == 0);
    ctx.Mode = 0; CHECK(h.SerialiseGameState(ctx)); CHECK(h.Def.Object == &def && h.PlayerStart == 1);
    h.BaseResult = false; h.PlayerStart = 9; CHECK(!h.SerialiseGameState(ctx)); CHECK(h.PlayerStart == 9);
    h.Def.Object = 0;
    if (fails == 0) printf("0083bb40_TEST PASS\n");
    return fails;
}
