// CBankFileAsync::HandleFailedAllocation() -- under the bank lock, walk the async-data set round-robin from just after the last restarted entry; the first finished entry with a completion pending is reset (buffer freed back to its pool), queued in ReadsToRestart and remembered; returns whether one was found
// Grade: DIFFER(305v314) -- register allocation / frame order: retail keeps `this` in esi (spilled to the frame word later reused by the insert result), the count in ebp and the entry in esi, with the lock at the top of the frame and the bool flags read as `mov cl,[esi+44h] ; test cl,cl`; ours keeps `this` in ebp, the count in edi, the entry in ecx (`cmp byte ptr`) and puts the lock low; tried member and accessor flag reads
// Leaf: ?HandleFailedAllocation@CBankFileAsync
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5BF0, length 314 bytes (plain `ret`).
// Callees: CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0, CHandleBasedMemoryPool::Free 0x00A62ED0 (the inlined
//          CBankFileAsyncData::FreeBuffer 0x009D5410), set<CBankFileAsyncData*>::insert 0x009D7CF0 (manifest
//          "BinaryTree_FindOrInsert").
// Layout: Ego_r.pdb CBankFileAsync with 12-byte retail sets: AsyncDataInstances +0x128, ReadsToRestart +0x134,
//          LastRestartingData +0x140, CriticalSection +0x14c; CBankFileAsyncData as in the landed FreeBuffer model
//          (m_finished +0x44, m_completionPending +0x46).
// Shape: STLport upper_bound and _M_increment inline; `begin()` when the cursor reaches end().
struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct CCriticalSectionLock_Retail {            // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;
    bool                    m_locked;
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
};
struct CHandleBasedMemoryPool_Calls {
    void Free(long handle);                      // 0x00A62ED0
};
struct CBankFileAsyncData {
    virtual void v00();
    void*                         m_pAllocation;      // +0x04
    void*                         m_pData;            // +0x08
    void*                         m_pReadBuffer;      // +0x0c
    unsigned char                 _pad10[0x0c];
    CHandleBasedMemoryPool_Calls* m_pPool;            // +0x1c
    long                          m_handle;           // +0x20
    unsigned char                 _pad24[0x20];
    bool                          m_finished;         // +0x44
    bool                          _pad45;
    bool                          m_completionPending;// +0x46
    bool IsFinished() const { return m_finished; }
    bool IsCompletionPending() const { return m_completionPending; }
    void FreeBuffer()                                // 0x009D5410 (inlined here)
    {
        if (m_pPool && m_handle != -1) {
            m_pPool->Free(m_handle);
            m_handle = -1;
            m_pReadBuffer = 0;
            m_pData = 0;
        }
    }
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_tree_node_data : _Rb_tree_node_base { CBankFileAsyncData* _M_value_field; };
__forceinline _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    } else {
        _Rb_tree_node_base* y = node->_M_parent;
        while (node == y->_M_right) {
            node = y;
            y = y->_M_parent;
        }
        if (node->_M_right != y)
            node = y;
    }
    return node;
}
struct CAsyncDataSet {                           // STLport set<CBankFileAsyncData*> (12 bytes)
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    unsigned long size() const { return _M_node_count; }
    _Rb_tree_node_base* begin() const { return _M_header->_M_left; }
    _Rb_tree_node_base* end() const { return _M_header; }
    _Rb_tree_node_base* upper_bound(CBankFileAsyncData* key) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0) {
            if (key < ((_Rb_tree_node_data*)x)->_M_value_field) {
                y = x;
                x = x->_M_left;
            } else
                x = x->_M_right;
        }
        return y;
    }
    struct SInsertResult { _Rb_tree_node_base* it; bool inserted; };
    SInsertResult insert(CBankFileAsyncData* const& value);   // 0x009D7CF0
};

class CBankFileAsync {
public:
    virtual void v00();
    unsigned char          _pad04[0x124];
    CAsyncDataSet          AsyncDataInstances;   // +0x128
    CAsyncDataSet          ReadsToRestart;       // +0x134
    CBankFileAsyncData*    LastRestartingData;   // +0x140
    unsigned char          _pad144[8];
    CRITICAL_SECTION_Fable CriticalSection;      // +0x14c
    bool HandleFailedAllocation();
};

bool CBankFileAsync::HandleFailedAllocation()
{
    CCriticalSectionLock_Retail lock(&CriticalSection, false);
    unsigned long count = AsyncDataInstances.size();
    if (count == 0)
        return false;
    _Rb_tree_node_base* it = AsyncDataInstances.upper_bound(LastRestartingData);
    for (unsigned long i = 0; i < count; ++i) {
        if (it == AsyncDataInstances.end())
            it = AsyncDataInstances.begin();
        CBankFileAsyncData* data = ((_Rb_tree_node_data*)it)->_M_value_field;
        if (data->IsFinished() && data->IsCompletionPending()) {
            data->m_finished = false;
            data->FreeBuffer();
            ReadsToRestart.insert(data);
            LastRestartingData = data;
            return true;
        }
        it = _M_increment(it);
    }
    return false;
}
