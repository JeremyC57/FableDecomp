// NThingManager::CEntityRuntimePersistence::Clear() -- empty both persistent entity-script arrays (releasing their storage) and the persisted-dead-entity map
// Leaf: ?Clear@CEntityRuntimePersistence@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005236F0, length 158 bytes. The catalog label "CDrawQuestInfo::OracleSymbolPopBack" is
// propagated: the body clears two 24-byte-element arrays and a red-black tree, and its callers are CEntityRuntimePersistence
// ::LoadFromGameStateScript (0x005248B0) and its neighbours (0x00524340, 0x00524ECE), all with the persistence object as `this`.
// Callees (per array, twice): `__fastcall _Move(first, last, dest, tag, bool)` @ 0x0052BD10, `__fastcall _Destroy(first, last, tag)`
// @ 0x0052BD70, CArray<CEntityScriptInfo>::DeallocateMemory() @ 0x0052C580; the map's _Erase(node*) @ 0x00527860.
// Layout: PDB NThingManager::CEntityRuntimePersistence: LevelPersistentEntityScripts +0, GamePersistentEntityScripts +0x10,
// PersistedDeadEntities +0x20 -> retail +0, +0xc, +0x18 (12-byte arrays; the map is head pointer +0x18, node count +0x1c, as the
// landed AddPersistedDeadEntity 0x00520330). Tree nodes: parent +4, leftmost/left +8, rightmost/right +0xc.
// Shape: each array clear is the inlined VC `erase(begin(), end())` (last read before first, the moved-to end kept in a callee-saved
// register, both empty tag temporaries homed in the high byte of the one `push ecx` frame slot, no empty-range test) followed by
// the out-of-line DeallocateMemory; the map clear only runs for a non-empty map: erase from the root, then leftmost = head,
// root = 0, rightmost = head (the head re-read for every store), count = 0.
struct SMoveTag_005236f0 { SMoveTag_005236f0() { } };
struct SAllocatorTag_005236f0 { SAllocatorTag_005236f0() { } };

namespace NThingManager {

struct CEntityScriptInfo { unsigned char _body[0x18]; };   // 24 bytes

CEntityScriptInfo* __fastcall Move_0052bd10(CEntityScriptInfo* first, CEntityScriptInfo* last, CEntityScriptInfo* dest,
                                            const SMoveTag_005236f0& tag, bool overlap);   // 0x0052BD10
void __fastcall Destroy_0052bd70(CEntityScriptInfo* first, CEntityScriptInfo* last,
                                 const SAllocatorTag_005236f0& allocator);              // 0x0052BD70

struct CArray_EntityScriptInfo {                   // CArray<CEntityScriptInfo>: retail vector, three words
    CEntityScriptInfo* m_first;
    CEntityScriptInfo* m_last;
    CEntityScriptInfo* m_end;
    void DeallocateMemory();                       // 0x0052C580
    void Clear()
    {
        CEntityScriptInfo* last = m_last;
        CEntityScriptInfo* first = m_first;
        CEntityScriptInfo* kept = Move_0052bd10(last, last, first, SMoveTag_005236f0(), false);
        Destroy_0052bd70(kept, m_last, SAllocatorTag_005236f0());
        m_last = kept;
        DeallocateMemory();
    }
};

struct SDeadEntityNode {
    long             _color;                       // +0x00
    SDeadEntityNode* _parent;                      // +0x04
    SDeadEntityNode* _left;                        // +0x08
    SDeadEntityNode* _right;                       // +0x0c
};

struct CRetailDeadEntityMap {                      // map<unsigned __int64, CPersistedDeadEntity>: head node, size
    SDeadEntityNode* m_pHead;
    unsigned long    m_size;
    SDeadEntityNode*& _Root() { return m_pHead->_parent; }
    SDeadEntityNode*& _Lmost() { return m_pHead->_left; }
    SDeadEntityNode*& _Rmost() { return m_pHead->_right; }
    void _Erase(SDeadEntityNode* node);            // 0x00527860
    void clear()
    {
        if (m_size != 0) {
            _Erase(_Root());
            _Lmost() = m_pHead;
            _Root() = 0;
            _Rmost() = m_pHead;
            m_size = 0;
        }
    }
};

class CEntityRuntimePersistence { public:
    CArray_EntityScriptInfo LevelPersistentEntityScripts;   // +0x00
    CArray_EntityScriptInfo GamePersistentEntityScripts;    // +0x0c
    CRetailDeadEntityMap    PersistedDeadEntities;          // +0x18
    long                    SaveFormatVersion;              // +0x20
    void Clear();
};

void CEntityRuntimePersistence::Clear()
{
    LevelPersistentEntityScripts.Clear();
    GamePersistentEntityScripts.Clear();
    PersistedDeadEntities.clear();
}

}