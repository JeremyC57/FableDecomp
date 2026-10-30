// CGameScriptInterface::TurnCreatureInto(const CScriptThing& thing, const CCharString& defName) const -- replace a live creature by a new one built from the named def: an unchanged def returns the thing itself; an AI creature (type 1) is re-created with its script name and game-state persistence; the player creature (type 2) is re-created persisted, takes copies of its component interfaces, and becomes the main player's character and controlled creature; the old thing is killed either way and the new script thing returned
// Leaf: ?TurnCreatureInto@CGameScriptInterface@@UBE?AVCScriptThing@@ABV2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089F4A0, 1134 bytes (+ int3 padding), `ret 0Ch` (hidden result + 2).
// Callees: CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0, CScriptThing::operator= 0x008AB980 / dtor 0x004AA840 (the
//   GetScriptThing temporaries), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, GetDef<CThingCreatureDef> 0x00513160,
//   CThingAICreatureInit ctor 0x004FACD0, CCharString 0x0099EBF0 / 0x0099EAE0 / operator=(const CCharString&) 0x0099EFB0,
//   CThing::GetPos 0x004C73D0 / IsGameStatePersisted 0x004C8280 / SetAsGameStatePersisted 0x004C7880 / GetScriptThing
//   0x004C7E50 / AddAndCopyTC 0x004C9F60 / CopyTCIfExists 0x004C8FB0 / Kill 0x004C9B80, CThingAICreature::Create 0x00833800 and
//   CThingPlayerCreature::Create 0x006AC910 (static __fastcall), interface-map LowerBound 0x004365B0 (const), g_pPlayerGui
//   0x013B8790 GetPHeroInfoScreensManager 0x00437CD0, CManager::DeregisterAllScreens 0x0066EBE0, CPlayerManager::GetMainPlayer
//   0x00449970, CPlayer 0x00487B80 (takes the new creature before SetControlledCreature; label propagated from
//   CVillageTaskCarryCrate::AssignCreatureToTask, a byte-identical body) / SetControlledCreature 0x00487CF0, operator delete 0x00BFE9BC;
//   CScriptThing slots 11 GetPThing, 75 IsAlive.
// Shape: one shared `tc` out-variable for every interface copy (the landed 0x005B37F7 GetInterface idiom) keeps each
//   component load in eax; separate per-call temporaries rotate the scratch registers.
// Layout: CGameScriptInterface DefinitionManager +0x10, PlayerManager +0x14 (Ego_r.pdb). Retail CThing (the landed 0x00836260
//   view): type byte +0x10 (here 1 = an AI creature, 2 = the player creature), component-present bits +0x20, interface map +0x44,
//   ScriptName +0x74, DefGlobalIndex +0x8c, PlayerNumber +0x90 (PDB +0x84/+0x9c/+0xa0, the 0x10 retail CThing shift).
//   CThingPlayerCreatureInit as Ego_r.pdb (12 bytes).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer { public:
    void* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
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
class CThing;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
struct CDefVirtual {
    virtual void v000();
    virtual void DestroyDef();                          // slot 1 (+4)
};
struct CRefCounted {
    long m_refCount;
};
class CDefPointeeBase : public CDefVirtual, public CRefCounted { public:
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
};
class CThingCreatureDef : public CDefPointeeBase { public: };
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);             // 0x009AD410
    bool GetDef(long index, CDefPointer<CThingCreatureDef>& out);        // 0x00513160 (GetDef<CThingCreatureDef>)
};
void* FindDef_CGameDefinitionManager_GetDef_F4A0();
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer<CThingCreatureDef>& out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_F4A0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingPhysical;
class CThingBuilding;
class CThingAICreatureInit { public:
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString     ScriptName;                         // +0x08
    CCharString     OverridingBrainName;                // +0x0c
    bool            PlayerCharacter;                    // +0x10
    bool            Drawable;                           // +0x11
    bool            PlaceAtEndOfList;                   // +0x12
    bool            EnableCreatureAutoPlacing;          // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable);   // 0x004FACD0
};
class CThingPlayerCreatureInit { public:
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    bool            PlaceAtEndOfList;                   // +0x08
    CThingPlayerCreatureInit() : OriginalThing(0), PreviousThing(0), PlaceAtEndOfList(true) { }
};
class CThing {
public:
    void* __vftable;
    unsigned char _pad04[0x10 - 0x04];
    unsigned char m_type;                               // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x74 - 0x4c];
    CCharString ScriptName;                             // +0x74
    unsigned char _pad78[0x8c - 0x78];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned char _pad8e[0x90 - 0x8e];
    signed char PlayerNumber;                           // +0x90
    const C3DVector& GetPos() const;               // 0x004C73D0
    bool IsGameStatePersisted() const;                  // 0x004C8280
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    CScriptThing GetScriptThing();                      // 0x004C7E50
    void AddAndCopyTC(const CTCBase* tc, bool a);       // 0x004C9F60
    void CopyTCIfExists(const CTCBase* tc);             // 0x004C8FB0
    void Kill(bool immediately);                        // 0x004C9B80
    __forceinline bool GetInterface(const CTCBase** out, long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = ((it == map->last || wanted < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingAICreature : public CThing { public:
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                               const CThingAICreatureInit& init);        // 0x00833800
};
class CThingPlayerCreature : public CThing { public:
    static CThingPlayerCreature* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                                   const CThingPlayerCreatureInit& init);  // 0x006AC910
};
struct CManager {
    void DeregisterAllScreens();                        // 0x0066EBE0
};
class CPlayerGui { public:
    CManager* GetPHeroInfoScreensManager() const;       // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
class CPlayer { public:
    void SetPlayerCharacter_00487B80(CThingPlayerCreature* creature);   // 0x00487B80
    void SetControlledCreature(CThingPlayerCreature* creature);         // 0x00487CF0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::TurnCreatureInto(const CScriptThing& thing, const CCharString& defName) const
{
    CScriptThing result;
    if (thing.IsAlive()) {
        long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
        CThing* old = thing.GetPThing();
        if (old->m_defGlobalIndex == index) {
            result = thing;
        } else {
            if (old->m_type == 1) {
                CThingAICreatureInit init(0, 0, false, "", "", true);
                init.ScriptName = old->ScriptName;
                CDefPointer<CThingCreatureDef> def;
                if (DefinitionManager->GetDef(index, def)) {
                    CThingAICreature* creature = CThingAICreature::Create(index, old->GetPos(), old->PlayerNumber, init);
                    if (creature) {
                        creature->SetAsGameStatePersisted(old->IsGameStatePersisted());
                        result = creature->GetScriptThing();
                        const CTCBase* tc;
                    }
                }
            } else if (old->m_type == 2) {
                CDefPointer<CThingCreatureDef> def;
                if (DefinitionManager->GetDef(index, def)) {
                    g_pPlayerGui->GetPHeroInfoScreensManager()->DeregisterAllScreens();
                    CThingPlayerCreatureInit init;
                    CThingPlayerCreature* creature = CThingPlayerCreature::Create(index, old->GetPos(), old->PlayerNumber, init);
                    if (creature) {
                        creature->SetAsGameStatePersisted(true);
                        result = creature->GetScriptThing();
                        const CTCBase* tc;
                        if (old->GetInterface(&tc, 0x29))
                            creature->AddAndCopyTC(tc, true);
                        if (old->GetInterface(&tc, 0x04))
                            creature->AddAndCopyTC(tc, true);
                        if (old->GetInterface(&tc, 0x68))
                            creature->AddAndCopyTC(tc, true);
                        if (old->GetInterface(&tc, 0x1b))
                            creature->AddAndCopyTC(tc, true);
                        if (old->GetInterface(&tc, 0xfc))
                            creature->AddAndCopyTC(tc, true);
                        if (old->GetInterface(&tc, 0x11))
                            creature->CopyTCIfExists(tc);
                        if (old->GetInterface(&tc, 0x13))
                            creature->CopyTCIfExists(tc);
                        if (old->GetInterface(&tc, 0x12))
                            creature->CopyTCIfExists(tc);
                    }
                    PlayerManager->GetMainPlayer().SetPlayerCharacter_00487B80(creature);
                    PlayerManager->GetMainPlayer().SetControlledCreature(creature);
                }
            }
            old->Kill(false);
        }
    }
    return result;
}