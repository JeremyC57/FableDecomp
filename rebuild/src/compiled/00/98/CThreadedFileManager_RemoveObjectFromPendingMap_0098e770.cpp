// CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject*) -- removes a file object from its device/priority pending queue: when the queue's next-object cursor points at it the cursor is stepped on first, then the entry is erased by key
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098E770, length 91 bytes.
// Callees: the pending map's find @ 0x0098FE70 (manifest "_Tree<...>::_Lbound": lower bound + the
// `key < *it` check, the node comes back in eax), the map iterator's operator++ @ 0x0098F9D0 (manifest
// "BinaryTree_RotateOrRebalance", propagated), the map's erase(const key&) @ 0x00990CD0.
// Layout: PDB CThreadedFileManager: DeviceQueue (CPendingObjectMap[2][7]) at +4, 20 bytes each in the
// PDB, 16 in retail (the VC6-era Dinkumware map is 12 bytes: allocator byte, head, size; then the
// NextObject iterator at +0x0c of the entry -- the `lea ebp,[eax+10h]` is this + 4 + index*16 + 0x0c).
// CFileObject: Priority at +0x14, the owning CThreadedFile at +0x28 (DeviceID at +4 of PDB CThreadedFile).
// The sort key wraps the object pointer (its operator< compares File->+0x14, then +4, then the pointer).
// Shape: the device row (`DeviceID * 7`) is a named local kept in esi across both calls; the cursor is a
// reference local to `DeviceQueue[d][p].NextObject` and the map is indexed again in the compare, so both
// addresses come from one `this + index*16` (`lea ecx,[eax+4]` / `lea ebp,[eax+10h]`; a reference to the
// whole queue folds the +4 first); the erase indexes again after the calls (Priority re-read). find
// returns the node pointer in eax (an iterator class result goes through a hidden pointer). The key is one
// named local homed in the dead parameter slot (`mov [esp+18h],edi` after the push of its address).
struct CThreadedFile {
    void* m_vtable;                                // +0x00
    long  DeviceID;                                // +0x04 EThreadedFileDeviceID
};

struct CFileObject {
    unsigned char  _pad_0x00[0x14];
    long           Priority;                       // +0x14
    unsigned char  _pad_0x18[0x10];
    CThreadedFile* File;                           // +0x28
};

struct CPendingObjectSortKey {
    CFileObject* Object;
    CPendingObjectSortKey(CFileObject* object) : Object(object) { }
};

struct SPendingNode;

struct CPendingIterator {                          // the map iterator: one node pointer
    SPendingNode* m_node;
    CPendingIterator& operator++();                // 0x0098F9D0
};

struct CPendingMap {                               // map<CPendingObjectSortKey, CFileObject*>, 12 bytes
    unsigned char m_allocator;
    SPendingNode* m_head;
    unsigned long m_size;
    SPendingNode* find(const CPendingObjectSortKey& key);       // 0x0098FE70 (node returned in eax)
    unsigned long erase(const CPendingObjectSortKey& key);      // 0x00990CD0
};

struct CPendingObjectMap {                         // 16 bytes in retail
    CPendingMap      Objects;                      // +0x00
    CPendingIterator NextObject;                   // +0x0c
};

struct CThreadedFileManager {
    void*             m_vtable;                    // +0x00
    CPendingObjectMap DeviceQueue[2][7];           // +0x04
    void RemoveObjectFromPendingMap(CFileObject* object);
};

void CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject* object)
{
    long device = object->File->DeviceID;
    CPendingObjectSortKey key(object);
    CPendingIterator& next = DeviceQueue[device][object->Priority].NextObject;
    if (next.m_node == DeviceQueue[device][object->Priority].Objects.find(key))
        ++next;
    DeviceQueue[device][object->Priority].Objects.erase(key);
}