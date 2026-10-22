// Behaviour test for 006dd1c0: final.cpp copied verbatim minus its same-TU stand-in bodies (replaced by functional stubs
// below), the out-of-line callees stubbed so the clear/count/key/value/insert sequence is observable.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* CopyBytes(void* d, const void* s, unsigned int n) { return std::memcpy(d, s, n); }
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 int64_t
#define long int
extern "C" void* fable_memcpy(void* d, const void* s, unsigned int n) { return CopyBytes(d, s, n); }
#define memcpy fable_memcpy
extern "C" void* fable_malloc(unsigned int n) { return std::malloc(n < 64 ? 64 : n); }   // the model sizes its 0x18-byte nodes for 32-bit pointers
extern "C" void fable_free(void* p) { std::free(p); }
#define malloc fable_malloc
#define free fable_free
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

// ---- model (final.cpp without the stand-in bodies) ----
// CHeroExperienceDef::ReadFromStream(CMemoryDataInputStream* stream, map<EDamageAttribute, CHeroStatIncreaseData>* out) -- persist reader of the stat-increase-per-hit-type map: clear the map, read the entry count, then per entry read the key (CPersistTraits<EDamageAttribute>) and the value (CPersistTraits<CHeroStatIncreaseData>) and insert_unique the pair
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

// ---- stubs ----
static unsigned char g_stream[256];
static unsigned long g_streamLen = 0;
static int g_overflow = 0, g_keyReads = 0, g_valueReads = 0, g_inserts = 0, g_outerErase = 0, g_innerErase = 0, g_copies = 0;
static long g_keys[16]; static unsigned int g_valueCounts[16];
void CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflow;
    CopyBytes(buffer, g_stream + m_position, (unsigned int)size);
    m_position += size; m_pData = g_stream + m_position; m_remaining = (long)(g_streamLen - m_position);
}
void CPersistTraitsAttr::TransferBinaryIn(CMemoryDataInputStream* stream, long* value)
{
    ++g_keyReads; long v; stream->Read(&v, 4); *value = v;
}
void CPersistTraitsStat::TransferBinaryIn(CMemoryDataInputStream* stream, CHeroStatIncreaseData* value)
{
    ++g_valueReads; long n; stream->Read(&n, 4); value->Map.m_count = (unsigned int)n;   // the value's node count stands in for its contents
}
CStatTree::CStatTree(const CStatTree& other)
{
    ++g_copies; m_header = (SNode*)malloc(0x18); m_count = other.m_count;
    m_header->m_color = false; m_header->m_parent = 0; m_header->m_left = m_header; m_header->m_right = m_header;
}
void CStatTree::Erase(SNode*) { ++g_innerErase; }
void CStatMapTree::Erase(SNode*) { ++g_outerErase; }
SPairIB CStatMapTree::InsertUnique(const SStatPair& value)
{
    g_keys[g_inserts] = value.first; g_valueCounts[g_inserts] = value.second.Map.m_count; ++g_inserts; ++m_count;
    return SPairIB(SIterator(m_header), true);
}
static void PutLong(long v) { CopyBytes(g_stream + g_streamLen, &v, 4); g_streamLen += 4; }
static bool Run(long chunkBytes, bool expectOverflow)
{
    g_streamLen = 0; PutLong(3); PutLong(7); PutLong(70); PutLong(8); PutLong(80); PutLong(9); PutLong(90);
    g_overflow = g_keyReads = g_valueReads = g_inserts = g_outerErase = g_innerErase = g_copies = 0;
    CMemoryDataInputStream in; in.m_pVtable = 0; in.m_position = 0; in.StreamSize = (long)g_streamLen; in.m_pData = g_stream;
    in.SourceChunkStreamPos = 0; in.m_remaining = chunkBytes;
    static SNode header; CStatMapTree out; out.m_header = &header; out.m_count = 5; header.m_parent = &header; header.m_left = 0; header.m_right = 0;
    ReadFromStream(&in, &out);
    bool ok = g_outerErase == 1 && header.m_left == &header && header.m_right == &header && header.m_parent == 0;
    ok = ok && g_keyReads == 3 && g_valueReads == 3 && g_inserts == 3 && out.m_count == 3;
    ok = ok && g_keys[0] == 7 && g_keys[1] == 8 && g_keys[2] == 9 && g_valueCounts[0] == 70 && g_valueCounts[1] == 80 && g_valueCounts[2] == 90;
    ok = ok && g_copies == 3 && in.m_position == g_streamLen && (expectOverflow ? g_overflow > 0 : g_overflow == 0);
    return ok;
}
int main()
{
    CHECK(Run(256, false));
    CHECK(Run(0, true));
    {   // empty map, zero count: no erase, nothing read
        g_streamLen = 0; PutLong(0); g_outerErase = g_inserts = 0;
        CMemoryDataInputStream in; in.m_pVtable = 0; in.m_position = 0; in.StreamSize = 4; in.m_pData = g_stream; in.SourceChunkStreamPos = 0; in.m_remaining = 256;
        static SNode header; CStatMapTree out; out.m_header = &header; out.m_count = 0;
        ReadFromStream(&in, &out);
        CHECK(g_outerErase == 0 && g_inserts == 0 && in.m_position == 4);
    }
    if (fails) { printf("006dd1c0_TEST FAIL (%d)\n", fails); return 1; }
    printf("006dd1c0_TEST PASS\n");
    return 0;
}