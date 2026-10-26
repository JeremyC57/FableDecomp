#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "004e9156"
// Copy of the candidate's inline CMemoryDataInputStream::Read model (member access only).
class CMemoryDataInputStream { public:
    void* m_pVtable;
    unsigned long position;
    long sourceLength;
    const unsigned char* chunkBase;
    long chunkPosition;
    long chunkLength;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        for (long i = 0; i < size; ++i) buffer[i] = (unsigned char)(0xA0 + i);
        position += size;
    }
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
static void InitStream(CMemoryDataInputStream& s, const unsigned char* data, long available, unsigned long pos)
{
    s.m_pVtable = 0; s.position = pos; s.sourceLength = 0; s.chunkBase = data; s.chunkPosition = 0;
    s.chunkLength = available; s.overflowCalls = 0;
}
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

// Fake STLport map: clear() and insert() record what the reader does.
namespace std {
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less { };
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    int clears; int count; K keys[8]; V values[8];
    void clear() { ++clears; count = 0; }
    void insert(const value_type& v) { keys[count] = v.first; values[count] = v.second; ++count; }
};
}
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0, OBJECT_AUGMENTATION_FORCE_DWORD = 0x7fffffff };
class CWeaponTrailGraphicSet {
public:
    LONG32 Attack, Knockdown;
    CWeaponTrailGraphicSet() : Attack(-1), Knockdown(-1) { }
};
template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStream& stream, T* out) const
    {
        LONG32 value;
        stream.Read(&value, sizeof(value));
        *out = (T)value;
    }
};
template <> struct CPersistTraits<CWeaponTrailGraphicSet> {
    void TransferBinaryIn(CMemoryDataInputStream& stream, CWeaponTrailGraphicSet* out) const
    {
        stream.Read(&out->Attack, 4);
        stream.Read(&out->Knockdown, 4);
    }
};
__forceinline void ReadCount(CMemoryDataInputStream& stream, LONG32* out)
{
    LONG32 value;
    stream.Read(&value, sizeof(value));
    *out = value;
}
template <class V, class K, class C>
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStream& stream, std::map<K, V, C>& out)
{
    out.clear();
    LONG32 count;
    ReadCount(stream, &count);
    for (; count > 0; --count) {
        std::pair<K, V> value;
        CPersistTraits<K>().TransferBinaryIn(stream, &value.first);
        CPersistTraits<V>().TransferBinaryIn(stream, &value.second);
        out.insert(typename std::map<K, V, C>::value_type(value.first, value.second));
    }
}
int main()
{
    LONG32 words[10] = { 3, 10, 100, 101, 20, 200, 201, 30, 300, 301 };
    unsigned char data[40];
    std::memcpy(data, words, sizeof(data));
    CMemoryDataInputStream s;
    InitStream(s, data, 40, 0);
    std::map<EObjectAugmentationType, CWeaponTrailGraphicSet, std::less<EObjectAugmentationType> > m; m.clears = 0; m.count = 5;
    GFSerialiseMapBinaryIn<CWeaponTrailGraphicSet, EObjectAugmentationType, std::less<EObjectAugmentationType> >(s, m);
    CHECK(m.clears == 1 && m.count == 3 && s.position == 40);
    CHECK(m.keys[0] == (EObjectAugmentationType)10 && m.keys[2] == (EObjectAugmentationType)30 && m.values[0].Attack == 100 && m.values[0].Knockdown == 101 && m.values[2].Knockdown == 301);
    LONG32 zero = 0;                         // an empty record: cleared, nothing inserted
    InitStream(s, (const unsigned char*)&zero, 4, 0);
    m.count = 0;
    GFSerialiseMapBinaryIn<CWeaponTrailGraphicSet, EObjectAugmentationType, std::less<EObjectAugmentationType> >(s, m);
    CHECK(m.clears == 2 && m.count == 0);
    std::printf("004e9156_TEST PASS\n");
    return 0;
}