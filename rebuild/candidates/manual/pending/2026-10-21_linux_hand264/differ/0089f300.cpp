// CGameScriptInterface::CreateCreatureNearby(const CCharString& defName, const C3DVector& position, float radius, const CCharString& scriptName, bool spawnEffect) const -- create the creature at the position, then, if it lives and has a navigation component, move it to a nearby wander position within a random radius (0.8 when the radius is no larger)
// Grade: DIFFER(395v403) -- register allocation: retail keeps `this` in ebx and the handle implementation pointer in edi (reloaded after each call, reused by the return copy); ours keeps `this` in edi and reloads the pointer into ecx, one push fewer. Tried inline IsAlive/GetPThing forms, explicit Data test, merged condition.
// Leaf: CreateCreatureNearby
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089F300, 416 bytes, `ret 0x14` (hidden CScriptThing result).
// Callees: own slot 91 CreateCreature, the handle implementation's slots 11 GetPThing / 75 IsAlive, the const component-map
//   LowerBound 0x004365B0 (TC 7, navigation), CWorld::GetPRandomSeed 0x0049D810, GFFloatRandom 0x004978A0 (fastcall),
//   CThing::GetPosition 0x004C73D0, CTCCreatureNavigation::GetNearbyWanderPosition 0x006CB6E0, the physics component's slot 31
//   ResetPosition, ~CBaseClass 0x0099A2E0 and operator delete 0x00BFE9BC (counted-pointer block).
// Layout: CScriptThing {vptr 0x01238C8C, CCountedPointer PImp +4/+8}; CThing native projection (UsingComponents +0x20,
//   Components +0x44, PhysicsTC +0x60). The generated CGameScriptInterface header flattens the vptr, so `this` is a
//   local projection with the real virtual slots (World at +4 as in the header).
void operator delete(void* p);                           // 0x00BFE9BC
struct CCharString { void* PStringData; };
struct C3DVector { float X, Y, Z; };
struct CWorld { unsigned long* GetPRandomSeed(); };     // 0x0049D810
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const long& key) const;   // 0x004365B0
};
struct CThing {
    void*           VTable;
    unsigned char   Base[0x1c];
    unsigned int    UsingComponents[9];                 // +0x20
    ComponentMap    Components;                         // +0x44
    unsigned char   Lists[0x14];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CTCCreatureNavigation {
    bool GetNearbyWanderPosition(const C3DVector& position, float radius, C3DVector& result, bool b) const;   // 0x006CB6E0
};

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing() { }
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010();
    virtual CThing* GetPThing() const { if (PImp.Data == 0) return 0; return PImp.Data->GetPThing(); }   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const { if (PImp.Data == 0) return false; return PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
};

struct CGameScriptInterface {                            // retail vtable 0x01260F0C
    virtual void v000();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, bool spawnEffect) const;   // slot 91 (+0x16c)
    CWorld* World;                                       // +0x04
    CScriptThing CreateCreatureNearby(const CCharString& defName, const C3DVector& position, float radius, const CCharString& scriptName, bool spawnEffect) const;
};

CScriptThing CGameScriptInterface::CreateCreatureNearby(const CCharString& defName, const C3DVector& position, float radius, const CCharString& scriptName, bool spawnEffect) const
{
    CScriptThing creature = CreateCreature(defName, position, scriptName, spawnEffect);
    if (creature.IsAlive()) {
        CThing* thing = creature.GetPThing();
        if (thing->UsingComponents[0] & 0x80) {
            const ComponentMap* map = &thing->Components;
            const ComponentEntry* it = map->LowerBound(7);
            const ComponentEntry* end = map->Finish;
            const CTCCreatureNavigation* navigation = (const CTCCreatureNavigation*)((it == end || it->Type > 7) ? end : it)->Component;
            float range = 0.8f;
            if (radius > 0.8f)
                range = GFFloatRandom(radius - 0.8f, World->GetPRandomSeed()) + 0.8f;
            C3DVector wanderPosition;
            if (navigation->GetNearbyWanderPosition(creature.GetPThing()->GetPosition(), range, wanderPosition, false))
                creature.GetPThing()->PhysicsTC->ResetPosition(wanderPosition);
        }
    }
    return creature;
}
