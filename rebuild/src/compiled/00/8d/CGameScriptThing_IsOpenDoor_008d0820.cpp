class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
struct CScriptThingEntity;
struct MapTreeNode;

struct DoorMap {
    MapTreeNode* LowerBound(int* keyPtr);  // thiscall: ecx=this, stack arg
};


bool __fastcall IsOpenDoor(void* thisptr)
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