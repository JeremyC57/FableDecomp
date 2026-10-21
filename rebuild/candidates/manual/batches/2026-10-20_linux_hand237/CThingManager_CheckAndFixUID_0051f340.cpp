// CThingManager::CheckAndFixUID(unsigned __int64& uid) -- keep the UID counters ahead of a loaded UID, and re-allocate the UID when another thing already owns it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051F340, length 306 bytes (+ int3 padding), `ret 4`.
// Callees: CWorldMap::GetMapNumberFromMapUID(unsigned long) const @ 0x004FB920 (landed), the CWorldMap virtuals
// IsMapLoaded (slot 10, +0x28) and GetMap (slot 18, +0x48, returns the CMap whose UID counter sits at +0x1D30, as the
// landed CMap::AllocateMapThingUID 0x0081E780), map<unsigned __int64, CThing*>::find @ 0x00527D80 (catalog
// "BSTNode_FindLowerBound"; returns the node, compared with the header node at map +0), CThingManager::
// AllocateNewThingUID @ 0x0051E100.
// Layout (retail, as the landed AllocateNewThingUID / OnDeleteClass rows): WorldMap +0x28, UIDToThingMap +0x58,
// CurrentUniqueID +0x68, CurrentLocalThingUniqueID +0x70; the map node keeps the value pair at +0x10 (second +0x18).
// Shape: a UID is <24-bit map tag : 40-bit counter>; the tag 0xFFFFFD is the global counter, 0xFFFFFF the plain one,
// anything else names a map. Both 64-bit pieces are computed before the zero test; the map tag stays 64-bit, so its
// later `== 0xFFFFFD` folds the high compare into `xor ecx,ecx`; the map number and the bool are named locals
// (map number fetched first, the bool lands in the dead uid argument slot).
#pragma pack(push, 4)
class CMap {
public:
    unsigned char    _pad[0x1d30];
    unsigned __int64 NextThingUID;                             // +0x1d30
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long mapNumber) const;            // slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17();
    virtual CMap& GetMap(long mapNumber);                      // slot 18 (+0x48)
    long GetMapNumberFromMapUID(unsigned long uid) const;      // 0x004FB920
};

class CThing;
struct CUIDToThingNode {
    unsigned char    _links[0x10];                             // colour, parent, left, right
    unsigned __int64 first;                                    // +0x10
    CThing*          second;                                   // +0x18
};
struct CUIDToThingMap {                                        // STLport map<unsigned __int64, CThing*>
    CUIDToThingNode* _M_header;                                // +0x00 (end())
    unsigned long    _M_node_count;
    unsigned char    _pad08[0x10 - 0x08];
    CUIDToThingNode* find(const unsigned __int64& key);        // 0x00527D80
    CUIDToThingNode* end() { return _M_header; }
};

class CThingManager {
public:
    void*            __vftable;                                // +0x00
    unsigned char    _pad04[0x28 - 0x04];
    CWorldMap*       WorldMap;                                 // +0x28
    unsigned char    _pad2c[0x58 - 0x2c];
    CUIDToThingMap   UIDToThingMap;                            // +0x58
    unsigned __int64 CurrentUniqueID;                          // +0x68
    unsigned __int64 CurrentLocalThingUniqueID;                // +0x70
    unsigned __int64 AllocateNewThingUID(long mapNumber, bool global, bool levelLess);   // 0x0051E100
    void CheckAndFixUID(unsigned __int64& uid);
};
#pragma pack(pop)

void CThingManager::CheckAndFixUID(unsigned __int64& uid)
{
    unsigned __int64 counter = uid & 0xFFFFFFFFFFui64;
    unsigned __int64 tag = uid >> 40;
    if (uid == 0)
        return;
    if (tag == 0xFFFFFD) {
        if (counter >= CurrentLocalThingUniqueID)
            CurrentLocalThingUniqueID = counter + 1;
    } else if (tag == 0xFFFFFF) {
        if (counter >= CurrentUniqueID)
            CurrentUniqueID = counter + 1;
    } else {
        long mapNumber = WorldMap->GetMapNumberFromMapUID((unsigned long)tag);
        if (WorldMap->IsMapLoaded(mapNumber)) {
            CMap& map = WorldMap->GetMap(mapNumber);
            if (counter >= map.NextThingUID)
                map.NextThingUID = counter + 1;
        }
    }
    if (uid != 0) {
        unsigned __int64 key = uid;
        CUIDToThingNode* it = UIDToThingMap.find(key);
        if (it != UIDToThingMap.end() && it->second) {
            long mapNumber = WorldMap->GetMapNumberFromMapUID((unsigned long)tag);
            bool global = tag == 0xFFFFFD;
            uid = AllocateNewThingUID(mapNumber, global, false);
        }
    }
}
