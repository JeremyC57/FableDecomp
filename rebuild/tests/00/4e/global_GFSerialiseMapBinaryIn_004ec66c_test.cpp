#include <cstdio>
#include <cstring>
// A memory input stream over a byte buffer (the retail CMemoryDataInputStream's Read: copy and advance).
struct CStream {
    const unsigned char* Data;
    long Position;
    CStream(const unsigned char* data) : Data(data), Position(0) {}
    void Read(void* out, long size) { std::memcpy(out, Data + Position, size); Position += size; }
    void ReadString(char* out) { long i = 0; while ((out[i] = (char)Data[Position++]) != 0) ++i; }
};
// Test-side writer that lays the serialised bytes out the way the reader expects them.
struct CWriter {
    unsigned char Data[512];
    long Size;
    CWriter() : Size(0) {}
    void Long(long v) { int x = (int)v; std::memcpy(Data + Size, &x, 4); Size += 4; }
    void Float(float v) { std::memcpy(Data + Size, &v, 4); Size += 4; }
    void Bool(bool v) { Data[Size++] = v ? 1 : 0; }
    void String(const char* s) { long n = (long)std::strlen(s) + 1; std::memcpy(Data + Size, s, n); Size += n; }
};
inline long ReadLong(CStream& stream) { int x; stream.Read(&x, 4); return x; }
class CCharString { public:
    char Text[32];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strcpy(Text, s); }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); }
    const CCharString& operator=(const CCharString& o) { std::strcpy(Text, o.Text); return *this; }
    bool operator==(const CCharString& o) const { return std::strcmp(Text, o.Text) == 0; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
// Map stand-in: unique keys (insert of an existing key is ignored) unless Multi.
template <class K, class V, bool Multi> struct CMap {
    typedef pair<const K, V> value_type;
    K Keys[16];
    V Values[16];
    long Count;
    bool Cleared;
    CMap() : Count(3), Cleared(false) {}
    void clear() { Count = 0; Cleared = true; }
    void insert(const value_type& v)
    {
        long i = 0;
        if (!Multi)
            for (; i < Count; ++i)
                if (Keys[i] == v.first)
                    return;
        Keys[Count] = v.first;
        Values[Count] = v.second;
        ++Count;
    }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferBinaryIn(CStream& stream, T* out) const { *out = (T)ReadLong(stream); }
};
template <> struct CPersistTraits<bool> {
    void TransferBinaryIn(CStream& stream, bool* out) const { unsigned char b; stream.Read(&b, 1); *out = b != 0; }
};
template <> struct CPersistTraits<CCharString> {
    void TransferBinaryIn(CStream& stream, CCharString* out) const { char text[32]; stream.ReadString(text); *out = CCharString(text); }
};
inline void ReadCount(CStream& stream, long* out) { *out = ReadLong(stream); }
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0, OBJECT_AUGMENTATION_FORCE_DWORD = 0x7fffffff };
class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) {}
    void TransferBinaryIn(CStream& stream) { TablePos = ReadLong(stream); }
};
template <> struct CPersistTraits<CDefString> {
    void TransferBinaryIn(CStream& stream, CDefString* out) const { out->TransferBinaryIn(stream); }
};
// The candidate logic: clear, read the count, then key + value per entry and insert the pair.
template <class K, class V, class TMap>
void SerialiseMapBinaryIn(CStream& stream, TMap& out)
{
    CPersistTraits<K> keyTraits;
    CPersistTraits<V> valueTraits;
    out.clear();
    long count;
    ReadCount(stream, &count);
    for (; count > 0; --count) {
        pair<K, V> value;
        keyTraits.TransferBinaryIn(stream, &value.first);
        valueTraits.TransferBinaryIn(stream, &value.second);
        out.insert(typename TMap::value_type(value.first, value.second));
    }
}
int main()
{
    bool ok = true;
    CWriter w;
    w.Long(2); w.Long(3); w.Long(40); w.Long(5); w.Long(41);
    CStream stream(w.Data);
    CMap<EObjectAugmentationType, CDefString, false> out;
    SerialiseMapBinaryIn<EObjectAugmentationType, CDefString>(stream, out);
    if (!out.Cleared || stream.Position != w.Size)
        ok = false;
    if (out.Count != 2 || out.Keys[0] != 3 || out.Values[0].TablePos != 40 || out.Keys[1] != 5 || out.Values[1].TablePos != 41) ok = false;
    if (CDefString().TablePos != -1) ok = false;
    if (!ok) { std::printf("004ec66c_TEST FAIL\n"); return 1; }
    std::printf("004ec66c_TEST PASS\n");
    return 0;
}