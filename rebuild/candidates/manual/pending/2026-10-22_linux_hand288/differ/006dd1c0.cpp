// CHeroExperienceDef::ReadFromStream(CMemoryDataInputStream* stream, map<EDamageAttribute, CHeroStatIncreaseData>* out) -- persist reader of the stat-increase-per-hit-type map: clear the map, read the entry count, then per entry read the key (CPersistTraits<EDamageAttribute>) and the value (CPersistTraits<CHeroStatIncreaseData>) and insert_unique the pair
// Grade: DIFFER(520v520) -- every instruction matches; only frame offsets differ: retail packs the inlined count read's local, the key-traits temporary and the 8-byte insert_unique result into one 8-byte slot at the bottom of the frame (sub esp,34h); ours keeps the result in its own 8-aligned slot (sub esp,38h)
// Leaf: ReadFromStream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DD1C0, length 520 bytes (`__fastcall`: ecx = stream, edx = map).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport node allocator), outer _Rb_tree::_M_erase 0x004E6E43,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CPersistTraits<EDamageAttribute>::TransferBinaryIn 0x00459083,
//   CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn 0x006DC070, inner tree copy ctor 0x004F9960,
//   outer insert_unique 0x006DCE90, inner _Rb_tree::_M_erase 0x004DA252.
// Layout: STLport _Rb_tree is 12 bytes {header node*, node count, empty less<> padded to 4} (an 8-byte model
//   makes VC7.1 realign the frame with `and esp,-8`); header nodes are 0x18 bytes; CPersistTraits<T> holds
//   a T DefaultVal (so the value traits object is a constructed/destroyed map, and the key traits is 4 bytes).
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;
    unsigned long m_position;
    long StreamSize;
    const unsigned char* m_pData;
    long SourceChunkStreamPos;
    long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct SLess { };                                   // std::less<long>: empty, one padded byte

struct SNode {
    bool m_color;
    SNode* m_parent;
    SNode* m_left;
    SNode* m_right;
};

struct CStatTree {                                  // CHeroStatIncreaseData's map (node 0x18)
    SNode* m_header;
    unsigned int m_count;
    SLess m_compare;
    __forceinline CStatTree()
    {
        m_header = 0;
        m_header = (SNode*)malloc(0x18);
        m_count = 0;
        m_header->m_color = false;
        m_header->m_parent = 0;
        m_header->m_left = m_header;
        m_header->m_right = m_header;
    }
    CStatTree(const CStatTree& other);              // 0x004F9960
    void Erase(SNode* node);                        // 0x004DA252
    __forceinline void clear()
    {
        if (m_count != 0) {
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_count = 0;
        }
    }
    __forceinline ~CStatTree()
    {
        clear();
        if (m_header)
            free(m_header);
    }
};

struct CHeroStatIncreaseData {
    CStatTree Map;
};

struct SStatPair {                                  // pair<const EDamageAttribute, CHeroStatIncreaseData>
    long first;
    CHeroStatIncreaseData second;
    SStatPair(long k, const CHeroStatIncreaseData& v) : first(k), second(v) { }
};

struct SLoopPair {                                  // pair<EDamageAttribute, CHeroStatIncreaseData>
    long first;
    CHeroStatIncreaseData second;
    SLoopPair() : first(0), second() { }
};

struct SIterator {
    SNode* m_node;
    SIterator(SNode* node) : m_node(node) { }
    SIterator(const SIterator& other) : m_node(other.m_node) { }
};
struct SPairIB {
    SIterator first;
    bool second;
    SPairIB(const SIterator& it, bool inserted) : first(it), second(inserted) { }
};

struct CStatMapTree {                               // map<EDamageAttribute, CHeroStatIncreaseData>
    SNode* m_header;
    unsigned int m_count;
    SLess m_compare;
    void Erase(SNode* node);                        // 0x004E6E43
    SPairIB InsertUnique(const SStatPair& value);   // 0x006DCE90
    __forceinline void clear()
    {
        if (m_count != 0) {
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_count = 0;
        }
    }
};

__forceinline void ReadCount(CMemoryDataInputStream* stream, long* out)
{
    long value;
    stream->Read(&value, 4);
    *out = value;
}

struct CPersistTraitsAttr {
    long DefaultVal;
    CPersistTraitsAttr() { }
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);   // 0x00459083
};
struct CPersistTraitsStat {
    CHeroStatIncreaseData DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream* stream, CHeroStatIncreaseData* value);   // 0x006DC070
};

void __fastcall ReadFromStream(CMemoryDataInputStream* stream, CStatMapTree* out)
{
    CPersistTraitsStat valueTraits;
    out->clear();
    long count;
    ReadCount(stream, &count);
    for (long i = count; i > 0; --i) {
        SLoopPair value;
        CPersistTraitsAttr().TransferBinaryIn(stream, &value.first);
        valueTraits.TransferBinaryIn(stream, &value.second);
        out->InsertUnique(SStatPair(value.first, value.second));
    }
}
