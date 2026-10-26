// CAIStateGroup_ShopkeeperWatchPlayer::IsRunnable() -- allow an open shop when its live hero is close or other customers are present; retail 0x00927c80, 287 bytes. Null/dead heroes and strict-distance misses take the other-customer path. The generated state-group header is absent, so the nonvirtual adapter uses shared CAIStateGroupBase/CAIBrain/CThingAICreature/CPlayer/CTCBase headers and documented native views. The creature interface map is at +0x44 (key 0x90); bit 0 at hero +0x91 is the dead flag. Callees: LowerBound 0x40f020, GetShopTC 0x798f50, IsOpeningHours 0x773950, GetAreOtherPeopleInShop 0x799a60, GetPPlayerManager 0x8fcf90, GetMainPlayer 0x449970, GetHeroThing 0x487dc0, GetPosition 0x4c73d0, FindFreeGazePos 0x918880.
#include <math.h>
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCBase.h"
#include "engine/_quarantine/CThing.h"

class CTCShop;
class CTCShopKeeper : public CTCBase { public:
    CTCShop* GetShopTC(); // 0x00798f50
    bool GetAreOtherPeopleInShop(bool); // 0x00799a60
};
class CTCShop : public CTCBase { public: bool IsOpeningHours(); }; // 0x00773950
struct CVectorMapEntry { long Type; CTCBase* Component; };
struct CCreatureInterfaceMap {
    CVectorMapEntry* Start;
    CVectorMapEntry* Finish;
    CVectorMapEntry* EndOfStorage;
    CVectorMapEntry* LowerBound(long* key); // 0x0040f020
};
class C3DVector { public: float X, Y, Z; };
struct CThingView : CThing { C3DVector* GetPosition(); // 0x004c73d0
};
struct CThingLifeStateRetailView {
    unsigned char _nativePrefix[0x91];
    unsigned char AliveFlags;
};
struct CPlayerManagerView : CPlayerManager { CPlayer* GetMainPlayer(); // 0x00449970
};
struct CPlayerView : CPlayer { CThing* GetHeroThing(); // 0x00487dc0
};
struct CWorldView;
extern float ShopkeeperWatchRadius; // 0x0138cb60
struct CAIStateGroup_ShopkeeperWatchPlayer : CAIStateGroupBase {
    CPlayerManager* GetPPlayerManager(); // 0x008fcf90
    CThing* FindFreeGazePos(); // 0x00918880
    bool IsRunnable();
};

bool CAIStateGroup_ShopkeeperWatchPlayer::IsRunnable()
{
    CThingAICreature* creature = PBrain->Creature;
    if ((*(unsigned int*)&creature->_pad_0x20[0x10] & 0x10000) != 0) {
        CCreatureInterfaceMap* interfaces = (CCreatureInterfaceMap*)&creature->_pad_0x20[0x24];   // +0x44 (the base-region pads merge in the retail STLport 4.6 header)
        long key = 0x90;
        CVectorMapEntry* entry = interfaces->LowerBound(&key);
        CVectorMapEntry* end = interfaces->Finish;
        if (entry == end || entry->Type > 0x90)
            entry = end;
        CTCShopKeeper* keeper = (CTCShopKeeper*)entry->Component;
        if (keeper->GetShopTC() != 0) {
            unsigned char open;
            if (keeper->GetShopTC() && keeper->GetShopTC()->IsOpeningHours())
                open = 1;
            else
                open = 0;

            CPlayer* mainPlayer = ((CPlayerManagerView*)GetPPlayerManager())->GetMainPlayer();
            CThing* hero = ((CPlayerView*)mainPlayer)->GetHeroThing();
            if (open) {
                if (hero != 0 && (((CThingLifeStateRetailView*)hero)->AliveFlags & 1) == 0) {
                    CTCShop* shop = keeper->GetShopTC();
                    CThing* shopThing = shop->ParentThing;
                    C3DVector* shopkeeperPosition = ((CThingView*)shopThing)->GetPosition();
                    C3DVector* heroPosition = ((CThingView*)hero)->GetPosition();
                    float dx = heroPosition->X - shopkeeperPosition->X;
                    float dy = heroPosition->Y - shopkeeperPosition->Y;
                    float dz = heroPosition->Z - shopkeeperPosition->Z;
                    float distance = sqrtf(dz * dz + dy * dy + dx * dx);
                    if (distance < ShopkeeperWatchRadius)
                        return FindFreeGazePos() != 0;
                }
                if (keeper->GetAreOtherPeopleInShop(false))
                    return FindFreeGazePos() != 0;
            }
        }
    }
    return false;
}