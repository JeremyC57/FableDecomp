// CAIStateGroupBase::CreateItem(CCharString&, CCharString&) -- look up the creature's carry slot, create the named thing at its position, then put it in the slot; retail 0x008fd0a0 (266 bytes), using CAIBrain/CThingAICreature headers and the opaque +0x44 map view.
#if defined(_MSC_VER)
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#define CThingAICreature CThingAICreaturePdb
#include "engine/CThingAICreature.h"              // layout only (sizeof); the local class below carries the CThing accessor
#undef CThingAICreature
#include "engine/CPlayerManager.h"
#include "fable_string.h"
class C3DVector;
class CThing { public: const C3DVector& GetPos() const; };   // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb)
class CThingAICreature : public CThing { public: unsigned char storage[sizeof(CThingAICreaturePdb)]; };
#else
#define __fastcall
class CCharString { public:
    CCharString();
    CCharString(const char*, long);
    ~CCharString();
};
class C3DVector;
class CThing { public: const C3DVector& GetPos() const; };   // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb)
class CThingAICreature : public CThing { public: unsigned char storage[0x100]; };
class CPlayer;
class CPlayerManager { public: long MainPlayer; };
class CAIBrain { public: CThingAICreature* Creature; CPlayerManager* PlayerManager; };
class CAIStateGroupBase { public: CAIBrain* PBrain; };
#endif

class C3DVector { public: float x, y, z; };
class CThing;
class CThingObject;
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(CCharString*); // 0x009ad410
};
// Retail getter 0x00449700 reads native MainPlayer at +0x1c; the donor header
// places it at +0x20 because its Players vector is four bytes larger.
struct CPlayerManagerAccess : CPlayerManager { long GetMainPlayer(); };
class CTCCarrying { public:
    void PutThingInCarrySlot(CThingObject*, long, bool); // 0x006a1f80
};

struct CAIStateGroupBaseCreateItem : CAIStateGroupBase {
    void CreateItem(CCharString*, CCharString*);
};

struct CLongPointerPair { long key; void* value; };
struct CVectorMapLong {
    CLongPointerPair* first;
    CLongPointerPair* last;
    CLongPointerPair* capacity;
    unsigned long flags;
    CLongPointerPair* LowerBound(const long& key); // 0x0040f020
};

CGameDefinitionManager* __fastcall GetGameDefinitionManager(); // 0x0044c6b0
CThingObject* __fastcall CreateObject(long definition, C3DVector* position, long playerID,
                                      float* orientation, unsigned long flags,
                                      const CCharString& name); // 0x00703210

void CAIStateGroupBaseCreateItem::CreateItem(CCharString* definitionName, CCharString* itemName)
{
    CAIBrain* brain = PBrain;
    CThingAICreature* creature = brain->Creature;
    if ((*(unsigned char*)((char*)creature + 0x28) & 0x40) != 0)
    {
        CVectorMapLong* carries = (CVectorMapLong*)((char*)creature + 0x44);
        long key = 0x46;
        CLongPointerPair* entry = carries->LowerBound(key);
        if (entry == carries->last || entry->key > 0x46)
            entry = carries->last;
        CTCCarrying* carry = (CTCCarrying*)entry->value;

        long playerID = ((CPlayerManagerAccess*)brain->PlayerManager)->GetMainPlayer();
        C3DVector position = PBrain->Creature->GetPos();
        float orientation[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

        long definition = GetGameDefinitionManager()->GetDefGlobalIndexFromName(definitionName);
        if (definition != 0)
        {
            CThingObject* thing;
            {
                CCharString createName("", -1);
            thing = CreateObject(definition, &position, playerID, orientation, 0, createName);
            }
            long item = GetGameDefinitionManager()->GetDefGlobalIndexFromName(itemName);
            carry->PutThingInCarrySlot(thing, item, true);
        }
    }
}