// Leaf: ?CGameScriptThing_IsOpenDoor@@YI_NPAX@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CScriptThingEntity;
struct MapTreeNode;

struct DoorMap {
    MapTreeNode* LowerBound(int* keyPtr);  // thiscall: ecx=this, stack arg
};


bool __fastcall CGameScriptThing_IsOpenDoor(void* thisptr)
{
    char* subobj = (char*)thisptr + 0xc;
    char* ent;

    if (((CScriptThingEntity*)(((CBaseIntelligentPointer*)(subobj))->GetPItem()))
        && !(*(unsigned char*)((char*)((CScriptThingEntity*)(((CBaseIntelligentPointer*)(subobj))->GetPItem())) + 0x91) & 0x1)
        && (ent = (char*)((CScriptThingEntity*)(((CBaseIntelligentPointer*)(subobj))->GetPItem())), (*(unsigned char*)(ent + 0x24) & 0x40)))
    {
        DoorMap* mapObj = (DoorMap*)(ent + 0x44);

        int key = 0x26;
        MapTreeNode* node = mapObj->LowerBound(&key);
        MapTreeNode* endNode = *(MapTreeNode**)((char*)mapObj + 0x4);

        if (node != endNode && *(int*)node <= 0x26)
            ;
        else
            node = endNode;

        char* value = *(char**)((char*)node + 0x4);

        return *(unsigned char*)(value + 0x12) == 0
            && *(unsigned char*)(value + 0x13) == 0
            && *(unsigned char*)(value + 0x10) != 0;
    }
    return false;
}