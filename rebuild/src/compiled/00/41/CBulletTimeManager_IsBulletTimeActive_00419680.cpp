// CBulletTimeManager::IsBulletTimeActive() const -- bullet time is on if overridden or any bullet-time source is registered
// Leaf: IsBulletTimeActive@CBulletTimeManager
// Retail: Fable.exe @ 0x00419680, 23 bytes; Ego_r.pdb ?IsBulletTimeActive@CBulletTimeManager@@QBE_NXZ.
// Layout: retail BulletTimeInfoMap is a 12-byte STLport map (header +0x00, node count +0x04), so Override sits at +0x0c
// (Ego_r.pdb / engine/CBulletTimeManager.h: 16-byte map, Override +0x10).
// Shape: `a || b` returning bool gives the `mov eax,1 / xor eax,eax` returns under /O2.
class CBulletTimeInfoMap {                               // STLport map<void*, CBulletTimeInfo>
public:
    void*         _M_header;                             // +0x00
    unsigned long _M_node_count;                         // +0x04
    unsigned long _M_key_compare_pad;                    // +0x08
    bool empty() const { return _M_node_count == 0; }
};
class CBulletTimeManager {
public:
    CBulletTimeInfoMap BulletTimeInfoMap;                // +0x00
    bool               Override;                         // +0x0c
    bool IsBulletTimeActive() const;
};
bool CBulletTimeManager::IsBulletTimeActive() const
{
    return Override || !BulletTimeInfoMap.empty();
}