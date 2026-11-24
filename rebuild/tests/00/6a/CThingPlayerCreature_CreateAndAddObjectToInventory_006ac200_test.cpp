// Exercise the production member with observable item, inventory and creation dependencies.
#include <stdio.h>
enum ETCInterfaceType { TCI_OTHER, TCI_INVENTORY };
struct CCharString { int id; };
namespace NInventory {
class CTCInventoryBase { public: ETCInterfaceType type; ETCInterfaceType GetInterfaceType()const{return type;} };
}
class CTCInventory : public NInventory::CTCInventoryBase {
public:
    long button,assigned;int queries,sets;
    long GetQuickAccessItemButtonIndex(long index){if(index==73)++queries;return button;}
    void SetThingAsQuickAccessItem(long index,long slot,bool flag){if(index==73&&!flag)++sets; assigned=slot;}
};
struct CTCInventoryItem {
    bool success,quick;int adds;CTCInventory* expected;
    ETCInterfaceType GetInventoryType()const{return TCI_INVENTORY;}
    bool AddToInventory(NInventory::CTCInventoryBase& inventory,bool first,bool fast,long zero,bool silent,bool last){
        if (&inventory==expected&&!first&&zero==0&&silent&&!last)++adds;
        quick=fast;return success;
    }
};
struct CThingObject {
    CTCInventoryItem* item; static CThingObject* result;static int creates;
    static CThingObject* Create(long index,int position,int player,int first,int second,const CCharString& name){
        if(index==29&&position==17&&player==3&&!first&&!second&&name.id==9)++creates;return result;
    }
    void GetTC(CTCInventoryItem** output){*output=item;}
    long GetDefGlobalIndex()const{return 73;}
};
CThingObject* CThingObject::result=0;int CThingObject::creates=0;
struct CTCHeroReceiveItems {CThingObject* got;int calls;void GiveItem(CThingObject& object,bool flag){if(!flag)++calls;got=&object;}};
struct CThingPlayerCreature {
    CTCInventory* inv;CTCHeroReceiveItems* receiver;int inventory_queries,hero_queries;
    int GetPlayerNumber()const{return 3;} int GetPos()const{return 17;}
    void GetTC(ETCInterfaceType type,NInventory::CTCInventoryBase** output){if(type==TCI_INVENTORY)++inventory_queries;*output=inv;}
    void GetTC(CTCHeroReceiveItems** output){++hero_queries;*output=receiver;}
    CThingObject* CreateAndAddObjectToInventory(long,long,bool,const CCharString&);
};
CThingObject* CThingPlayerCreature::CreateAndAddObjectToInventory(long def_index, long quick_access_slot,
                                                                  bool silent, const CCharString& script_name)
{
    CThingObject* pobject = CThingObject::Create(def_index, GetPos(), GetPlayerNumber(), 0, 0, script_name);
    if (pobject) {
        CTCInventoryItem* ptc_inventory_item = 0;
        pobject->GetTC(&ptc_inventory_item);
        if (ptc_inventory_item) {
            NInventory::CTCInventoryBase* ptc_inventory = 0;
            GetTC(ptc_inventory_item->GetInventoryType(), &ptc_inventory);
            bool added = false;
            if (silent) {
                bool add_to_quick_access = quick_access_slot != -1;
                added = ptc_inventory_item->AddToInventory(*ptc_inventory, false, add_to_quick_access, 0, silent, false);
            } else {
                CTCHeroReceiveItems* ptc_hero_receive_items = 0;
                GetTC(&ptc_hero_receive_items);
                ptc_hero_receive_items->GiveItem(*pobject, false);
                added = true;
            }
            if (added && quick_access_slot >= 0 && ptc_inventory->GetInterfaceType() == TCI_INVENTORY) {
                CTCInventory* inventory = static_cast<CTCInventory*>(ptc_inventory);
                if (inventory->GetQuickAccessItemButtonIndex(pobject->GetDefGlobalIndex()) == -1)
                    inventory->SetThingAsQuickAccessItem(pobject->GetDefGlobalIndex(), quick_access_slot, false);
            }
        }
    }
    return pobject;
}

int main() {
    const long slots[] = {-2,-1,0,5};
    for(int mask=0;mask<64;++mask) for(int i=0;i<4;++i) {
        bool exists=(mask&1)!=0, present=(mask&2)!=0, silent=(mask&4)!=0;
        bool success=(mask&8)!=0, normal=(mask&16)!=0, occupied=(mask&32)!=0;
        CTCInventory inventory;inventory.type=normal?TCI_INVENTORY:TCI_OTHER;
        inventory.button=occupied?2:-1;inventory.queries=inventory.sets=0;inventory.assigned=-100;
        CTCInventoryItem item;item.success=success;item.quick=false;item.adds=0;item.expected=&inventory;
        CThingObject object;object.item=present?&item:0;
        CThingObject::result=exists?&object:0;CThingObject::creates=0;
        CTCHeroReceiveItems receiver;receiver.got=0;receiver.calls=0;
        CThingPlayerCreature player;player.inv=&inventory;player.receiver=&receiver;player.inventory_queries=player.hero_queries=0;
        CCharString name;name.id=9;
        CThingObject* value=player.CreateAndAddObjectToInventory(29,slots[i],silent,name);
        bool has=exists&&present, receive=has&&!silent, add=has&&silent;
        bool query=has&&(!silent||success)&&slots[i]>=0&&normal;
        if(value!=CThingObject::result||CThingObject::creates!=1||player.inventory_queries!=int(has))return 1;
        if(item.adds!=int(add)||(add&&item.quick!=(slots[i]!=-1)))return 2;
        if(player.hero_queries!=int(receive)||receiver.calls!=int(receive)||receiver.got!=(receive?&object:0))return 3;
        if(inventory.queries!=int(query)||inventory.sets!=int(query&&!occupied))return 4;
        if(inventory.assigned!=(query&&!occupied?slots[i]:-100))return 5;
    }
    puts("006ac200_TEST PASS");return 0;
}