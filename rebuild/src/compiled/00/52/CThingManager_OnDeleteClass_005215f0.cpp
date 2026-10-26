// CThingManager::OnDeleteClass(CThing& thing) -- class-observer hook: erases the thing's UniqueID from the UID-to-thing map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005215F0, length 38 bytes (CThingManager vtable slot 3).
// Callees: map<unsigned __int64, CThing*>::erase(const unsigned __int64&) @ 0x0052AFB0 (`ret 4`, count dropped;
// manifest "BinaryTree_DeleteNode_Chain").
// The catalog name is the FableWin-style decoration ?OnDeleteClass@CThingManager@@MAEXAAVCClass@@@Z;
// the observed class is CThing (UniqueID, unsigned __int64, at +0x18). UIDToThingMap at +0x58.
// Shape: the key is copied into an 8-byte frame temporary before its address is passed: the id
// comes from an inline accessor returning it BY VALUE (a direct member reference would pass
// &thing.UniqueID without a copy).
#pragma pack(push, 4)                              // the __int64 after the vptr must not realign the class
class CThing { public:
    virtual void v00();
    unsigned char    _pad04[0x14];
    unsigned __int64 UniqueID;                     // +0x18
    unsigned __int64 GetUniqueID() const { return UniqueID; }
};
#pragma pack(pop)
struct CUIDToThingMap {
    unsigned char _body[0x10];
    unsigned long erase(const unsigned __int64& key);   // 0x0052AFB0
};
class CThingManager { public:
    virtual void v00();
    unsigned char  _pad04[0x54];
    CUIDToThingMap UIDToThingMap;                  // +0x58
    void OnDeleteClass(CThing& thing);
};

void CThingManager::OnDeleteClass(CThing& thing)
{
    UIDToThingMap.erase(thing.GetUniqueID());
}