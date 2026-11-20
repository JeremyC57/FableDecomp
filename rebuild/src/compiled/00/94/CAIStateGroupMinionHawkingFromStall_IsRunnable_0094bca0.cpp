// CAIStateGroup_MinionHawkingFromStall::IsRunnable() -- once the next hawk frame is reached, a silent shopkeeper in work hours with a shop hawks from the shop thing; with no shop thing (and a stall-less shop) it hawks on the spot unless the creature dislikes the player (attitude 3, 4 or 8)
// Leaf: ?IsRunnable@CAIStateGroup_MinionHawkingFromStall@@UAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retry 10b (getsw sweep, FIXES B-15): the opinion reaction manager is read through an inline getter `GetOpinionReactionManager()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BCA0, length 336 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame @ 0x0049D870 (static), CVectorMap::LowerBound
//          @ 0x0040F020 (inlined GetInterface 0x90 = CTCShopKeeper, bit 16 of +0x30 tested in memory), IsSpeaking
//          @ 0x008644E0 (__fastcall), CAIStateGroup_WorkBase::IsInWorkHours @ 0x0094BEF0, CTCShopKeeper::GetShopTC
//          @ 0x00798F50 / GetShopThing @ 0x00798D80, CIntelligentPointer<const CThing>(CThing*) @ 0x00668B60 and
//          operator= @ 0x00668B80 (both out of line), ~CBaseIntelligentPointer @ 0x00A01C10, GetPItem @ 0x00A01B50,
//          CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0, COpinionReactionManager::GetThingAttitudeToPlayer @ 0x00701840.
// Shape: HasTC + entry pick (keeper in edi); each PStall assignment is an implicit conversion (the frame temporary's
//          address is re-taken for the out-of-line operator=); the result is
//          `PStall.GetPItem() != 0` (neg/sbb/neg).
// Layout: no engine header for this class; modelled locally like the landed OnReplace / IsStillValid rows (next hawk
//          frame +0x14, PStall +0x18); the shop's +0x50 flag is descriptive.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase;
namespace NCreatureAI {
bool __fastcall IsSpeaking(const CThingCreatureBase& creature);   // 0x008644E0 (NF35: Ego_r + FableWin ?IsSpeaking@NCreatureAI@@YI_NABVCThingCreatureBase@@@Z)
}
class CTCShop { public:
    unsigned char _pad[0x50];
    bool OnTheSpot;                             // +0x50 (descriptive)
};
// NF168: the shop-keeper component (interface 0x90) is the shared engine/CTCShopKeeper.h class: Ego_r.pdb / FableWin.pdb
//   ?GetShopTC@CTCShopKeeper@@QBEPAVCTCShop@@XZ (const, 0x00798F50; opt-in FABLE_RETAIL_CTCSHOPKEEPER_SHOP) and
//   ?GetCustomer@CTCShopKeeper@@QBEPAVCThingCreatureBase@@XZ (const, 0x00798D80: reads Customer +0x0C), replacing the local model's
//   GetShopTC / GetShopThing.
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#include "engine/CTCShopKeeper.h"
enum EOpinionAttitudeType;                  // MS forward enum (Ego_r.pdb result type)
class COpinionReactionManager { public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840 Ego_r.pdb + FableWin.pdb ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z (name-fix 2026-11-12)
};
class CWorld { public:
    unsigned char _pad[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74
    COpinionReactionManager* GetOpinionReactionManager() const { return POpinionReactionManager; }
    static long __fastcall GetFrame();                // 0x0049D870
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
namespace N_00668b60 { struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    CIntelligentPointerConstThing(CThing* item);    // 0x00668B60
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other);   // 0x00668B80
}; } using N_00668b60::CIntelligentPointerConstThing;
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// NF51 (round 7): the AI state-group base and its work-group layer come from the shared headers (were a local CAIStateGroupBase model
// and a derived-class IsInWorkHours stand-in for 0x0094BEF0): RTTI .?AVCAIStateGroup_MinionHawkingFromStall@@ derives
// CAIStateGroup_WorkBase<CAIStateGroupBase> (ghidra_out/rtti_bases.tsv), and the callee 0x0094BEF0 is that template's protected const
// ?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ (Ego_r.pdb 0x009702A0; engine/CAIStateGroup_WorkBase.h,
// declaration only), was `IsInWorkHours`; PeekWorld 0x008FCFA0 / GetWorld 0x008FCFC0 are the base's protected members (_PEEK,
// _GOTOBED_WORLD); IsRunnable overrides the base's virtual slot (Ego_r.pdb UAE).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_WorkBase.h"

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroup_WorkBase<CAIStateGroupBase> {
    CThing* GetCreature() const { return PBrain->PCreature; }   // (the row's helper, moved here from its local base model)
    long NextHawkFrame;                         // +0x14 (descriptive)
    CIntelligentPointerConstThing PStall;       // +0x18
    bool IsRunnable();
};

bool CAIStateGroup_MinionHawkingFromStall::IsRunnable()
{
    if (GetWorld().GetFrame() < NextHawkFrame)
        return false;
    CThing* creature = GetCreature();
    if (creature->HasTC(TCI_SHOP_KEEPER)) {
        CTCShopKeeper* keeper = ((CTCShopKeeper*)&creature->GetTC(TCI_SHOP_KEEPER));
        if (!NCreatureAI::IsSpeaking(*(const CThingCreatureBase*)creature) && StayInShop()) {
            CTCShop* shop = keeper->GetShopTC();
            if (shop) {
                PStall = keeper->GetCustomer();
                if (!((CThing*)PStall.GetPItem()) && shop->OnTheSpot) {
                    PStall = GetCreature();
                    CThing* self = GetCreature();
                    long attitude = PeekWorld().GetOpinionReactionManager()->GetThingAttitudeToPlayer(*self);
                    if (attitude == 3 || attitude == 4 || attitude == 8)
                        PStall = (CThing*)0;
                }
                return ((CThing*)PStall.GetPItem()) != 0;
            }
        }
    }
    return false;
}