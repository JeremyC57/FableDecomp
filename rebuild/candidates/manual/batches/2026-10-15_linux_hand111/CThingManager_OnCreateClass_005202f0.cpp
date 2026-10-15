// CThingManager::OnCreateClass(CThing& thing) -- class-observer hook: registers the new thing in the UID-to-thing map (insert of {UniqueID, &thing})
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005202F0, length 49 bytes.
// Callees: map<unsigned __int64, CThing*>::insert(const value_type&) @ 0x0052A200 (returns pair<iterator, bool>
// through the hidden pointer, `ret 8`; manifest "BinaryTree_Insert_WithComparator").
// The catalog name is the FableWin-style decoration ?OnCreateClass@CThingManager@@MAEXAAVCClass@@@Z
// (protected virtual void OnCreateClass(CClass&)); the observed class is CThing here (UniqueID,
// an unsigned __int64, at +0x18 as in the PDB). Layout: UIDToThingMap at +0x58 (PDB +0x6c).
// Shape: the 16-byte value_type temporary (key, pointer, pad) and the 8-byte insert result share
// a 0x18-byte frame; the result must be a non-POD (user constructors on the iterator) to come
// back through a hidden pointer; the pair is built by its constructor from the key and &thing.
#pragma pack(push, 4)                              // the __int64 after the vptr must not realign the class
struct CThing {
    virtual void v00();
    unsigned char    _pad04[0x14];
    unsigned __int64 UniqueID;                     // +0x18
};
#pragma pack(pop)
struct SUIDNode;
struct SUIDMapIterator {
    SUIDNode* m_pNode;
    SUIDMapIterator() { }
};
struct SUIDMapInsertResult {
    SUIDMapIterator first;
    bool            second;
};
struct SUIDMapValue {
    const unsigned __int64 first;
    CThing*                second;
    SUIDMapValue(const unsigned __int64& key, CThing* const& value) : first(key), second(value) { }
};
struct CUIDToThingMap {
    unsigned char _body[0x10];
    SUIDMapInsertResult insert(const SUIDMapValue& value);   // 0x0052A200
};
struct CThingManager {
    virtual void v00();
    unsigned char  _pad04[0x54];
    CUIDToThingMap UIDToThingMap;                  // +0x58
    void OnCreateClass(CThing& thing);
};

void CThingManager::OnCreateClass(CThing& thing)
{
    UIDToThingMap.insert(SUIDMapValue(thing.UniqueID, &thing));
}
