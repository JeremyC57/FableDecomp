// CGameScriptInterface::CreateCreatureOnEntity(const CCharString& defName, const CScriptThing& entity, const CCharString& brainName) const -- when the entity is alive, create an AI creature of that def at the entity's position (with the overriding brain name), give it the entity's orientation and return its handle; otherwise an empty handle
// Leaf: CreateCreatureOnEntity
// Retail: Fable.exe (TLC, VC7.1) @ 0x00894500, 466 bytes, `ret 0x10` (hidden CScriptThing result).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0,
//   CDefinitionManager::GetDef<T>(long, CDefPointer&) 0x00513160, CThingAICreature::Create 0x00833800 (static fastcall),
//   CThing::GetScriptThing 0x004C7E50, CScriptThing::operator= 0x008AB980 / ~CScriptThing 0x004AA840, operator delete
//   0x00BFE9BC; the entity's slots 6 GetPos / 11 GetPThing / 75 IsAlive, physics slots 72 GetOrientation / 32.
// Layout: CThingAICreatureInit is the PDB 28-byte struct (default-argument CCharString("") names); CThing PhysicsTC +0x60;
//   the generated CGameScriptInterface header flattens the vptr, so `this` is a local projection (DefinitionManager +0x10).
#include "engine/CCountedPointer.h"
void operator delete(void* p);                           // 0x00BFE9BC
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void SetOrientation(const CRightHandedSet& orientation);   // slot 32 (+0x80)
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
};
struct CThingPhysical;
struct CThingBuilding;
struct CThingAICreatureInit {                            // PDB, 28 bytes
    CThingPhysical* OriginalThing;
    CThingPhysical* PreviousThing;
    CCharString     ScriptName;
    CCharString     OverridingBrainName;
    bool            PlayerCharacter;
    bool            Drawable;
    bool            PlaceAtEndOfList;
    bool            EnableCreatureAutoPlacing;
    CThingBuilding* HomeBuilding;
    CThingBuilding* WorkBuilding;
    CThingAICreatureInit(const CCharString& scriptName = CCharString(""), const CCharString& brainName = CCharString(""))
        : OriginalThing(0), PreviousThing(0), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(false), Drawable(true), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) { }
};
struct CDefBase {
    virtual void v0();
    virtual void DeleteThis();                           // slot 1
    long RefCount;                                       // +0x04
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CCreatureDef : CDefBase {
    unsigned char Data[0x114];
    long          PlayerNumber;                          // +0x11c (descriptive)
};
template <class T> struct CDefPointer {
    T* PDef;
    CDefPointer() : PDef(0) { }
    ~CDefPointer() { PDef->Release(); }
    T* operator->() const { return PDef; }
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
    bool GetDef(long index, CDefPointer<CCreatureDef>& out) const;   // 0x00513160
};
struct CBaseClass {
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CThing;
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    CScriptThing& operator=(const CScriptThing& other);  // 0x008AB980
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetPThing() const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
struct CScriptThingTemp : CScriptThing {
    ~CScriptThingTemp();
};
struct CThing {
    void*           VTable;
    unsigned char   Base[0x5c];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    CScriptThing GetScriptThing();                       // 0x004C7E50
};
struct CThingAICreature {
    static CThing* __fastcall Create(long defIndex, const C3DVector& position, long playerNumber, const CThingAICreatureInit& init);   // 0x00833800
};

struct CGameScriptInterface {                            // retail vtable 0x01260F0C
    virtual void v000();
    void*               World;                           // +0x04
    void*               Component;                       // +0x08
    void*               DisplayEngine;                   // +0x0c
    CDefinitionManager* DefinitionManager;               // +0x10
    CScriptThing CreateCreatureOnEntity(const CCharString& defName, const CScriptThing& entity, const CCharString& brainName) const;
};

CScriptThing CGameScriptInterface::CreateCreatureOnEntity(const CCharString& defName, const CScriptThing& entity, const CCharString& brainName) const
{
    CScriptThing result;
    if (entity.IsAlive()) {
        long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
        CThingAICreatureInit init;
        init.OverridingBrainName = brainName;
        CDefPointer<CCreatureDef> def;
        DefinitionManager->GetDef(index, def);
        CThing* creature = CThingAICreature::Create(index, entity.GetPos(), def->PlayerNumber, init);
        if (creature) {
            CTCPhysicsBase* physics = creature->PhysicsTC;
            if (physics) {
                CTCPhysicsBase* entityPhysics = entity.GetPThing()->PhysicsTC;
                if (entityPhysics)
                    physics->SetOrientation(entityPhysics->GetOrientation());
            }
            result = creature->GetScriptThing();
        }
    }
    return result;
}
