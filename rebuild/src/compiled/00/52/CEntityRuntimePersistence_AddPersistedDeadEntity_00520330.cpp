// CEntityRuntimePersistence::AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC) -- record a dead entity by UID in the persisted-dead map (ignoring a zero UID): a CPersistedDeadEntity is filled in and inserted keyed by the UID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00520330, length 95 bytes (the oracle row carries 1 byte of int3 padding).
// Callees: the map's insert(const value_type&) @ 0x0052A130 (manifest "BinarySearchTree_Insert": a VC6 Dinkumware _Tree::insert comparing
// the 64-bit key high word first; it returns pair<iterator,bool> through a hidden pointer that VC7.1 homes in the dead slot of the by-value
// UID argument, `lea ecx,[esp+40h]`). Shape: `or ecx,edx` is the 64-bit zero test; the 24-byte pair temporary is built at [esp+20h]
// (first = uid, second = the 16-byte record copied as four words: the uid words straight from esi/edi, the bool as the whole padded word
// `mov edx,[esp+1Ch]`, the CRC from ebx), so only the record's flag byte store survives. The map sits at +0x18 (PDB +0x20: the two
// CArray<CEntityScriptInfo> before it are 12 bytes in retail); the PDB record is CPersistedDeadEntity {UID +0, GamePersistent +8,
// SectionNameCRC +0xc}. Names from the NThingManager::CEntityRuntimePersistence PDB layout.
struct CPersistedDeadEntity {                      // 16 bytes
    unsigned __int64 UID;                          // +0x00
    bool             GamePersistent;               // +0x08
    unsigned long    SectionNameCRC;               // +0x0c
};

struct SDeadEntityPair_00520330 {                  // pair<unsigned __int64 const, CPersistedDeadEntity>
    unsigned __int64     first;
    CPersistedDeadEntity second;
    SDeadEntityPair_00520330(unsigned __int64 key, const CPersistedDeadEntity& value) : first(key), second(value) { }
};

struct SDeadEntityIterator_00520330 {              // 4-byte iterator (a user constructor makes the result pair non-POD)
    void* m_pNode;
    SDeadEntityIterator_00520330() { }
};

struct SDeadEntityInsertResult_00520330 {          // pair<iterator, bool>, returned through a hidden pointer
    SDeadEntityIterator_00520330 first;
    bool                         second;
};

struct CRetailDeadEntityMap_00520330 {             // map<unsigned __int64, CPersistedDeadEntity>: head node, size
    void*         m_pHead;
    unsigned long m_size;
    SDeadEntityInsertResult_00520330 insert(const SDeadEntityPair_00520330& value);   // 0x0052A130
};

struct CEntityRuntimePersistence {
    unsigned char                 _pad0[0x18];     // LevelPersistentEntityScripts, GamePersistentEntityScripts (12-byte CArrays)
    CRetailDeadEntityMap_00520330 PersistedDeadEntities;   // +0x18
    long                          SaveFormatVersion;       // +0x20
    void AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC);
};

void CEntityRuntimePersistence::AddPersistedDeadEntity(unsigned __int64 uid, bool gamePersistent, unsigned long sectionNameCRC)
{
    if (uid != 0) {
        CPersistedDeadEntity entity;
        entity.UID = uid;
        entity.GamePersistent = gamePersistent;
        entity.SectionNameCRC = sectionNameCRC;
        PersistedDeadEntities.insert(SDeadEntityPair_00520330(uid, entity));
    }
}