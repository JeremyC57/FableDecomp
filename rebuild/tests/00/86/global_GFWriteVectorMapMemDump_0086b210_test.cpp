#include <cstdio>
#include <cstring>
// Output stream stand-in: logs every write as (kind, value) so the test can check the order.
struct CWriteLog {
    char Kind[64];
    long Value[64];
    const char* Text[64];
    long Count;
    CWriteLog() : Count(0) {}
    void Add(char kind, long value, const char* text) { Kind[Count] = kind; Value[Count] = value; Text[Count] = text; ++Count; }
};
struct CCharString {
    const char* Text;
    CCharString() : Text("") {}
    CCharString(const char* text, long) : Text(text) {}
    CCharString(const CCharString& o) : Text(o.Text) {}
};
struct CDataOutputStreamWriter {
    CWriteLog Log;
    void WriteLong(long value) { Log.Add('L', value, 0); }
    void WriteFloat(float value) { Log.Add('F', (long)(value * 100.0f), 0); }
    void WriteBool(bool value) { Log.Add('B', value ? 1 : 0, 0); }
    void WriteString(const CCharString& text) { Log.Add('S', 0, text.Text); }
    void WriteNullTerminatedString(const CCharString& text) { Log.Add('Z', 0, text.Text); }
    void Write(const void* data, long size) { Log.Add('W', size, (const char*)data); }
};
struct CMemoryDataInputStreamReader {
    const long* Data;
    long Position;
    CMemoryDataInputStreamReader(const long* data) : Data(data), Position(0) {}
    void Read(void* out, long size) { std::memcpy(out, &Data[Position], size); ++Position; }
    long Next() { return Data[Position++]; }
};
template <class T> struct CVector {
    T Storage[8];
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CVector() : _M_start(Storage), _M_finish(Storage), _M_end_of_storage(Storage + 8) {}
    void resize(unsigned int n) { _M_finish = _M_start + n; }
    T* begin() { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct CPersistTraits;
inline void ReadCount(CMemoryDataInputStreamReader& stream, long* out) { long value; stream.Read(&value, sizeof(value)); *out = value; }
// The candidate logic (both directions).
template <class T> void SerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, CVector<T>& out)
{
    CPersistTraits<T> traits;
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
template <class T> void SerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, CVector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}
struct CTextureMorph { long A; long B; };
template <class K, class V> struct CKeyPairCompareLess {};
template <class V, class K, class C> struct CVectorMap {
    typedef pair<K, V> value_type;
    CVector<value_type> Entries;
    C Compare;
    bool Dirty;
    unsigned int size() const { return Entries.size(); }
    const void* GetData() const { return size() ? (const void*)Entries._M_start : (const void*)this; }
};
// The candidate logic: count, byte size, raw entry bytes (the map itself when empty), dirty flag.
template <class V, class K, class C> void WriteVectorMapMemDump(CDataOutputStreamWriter& stream, const CVectorMap<V, K, C>& map)
{
    stream.WriteLong(map.size());
    stream.WriteLong(map.size() * sizeof(typename CVectorMap<V, K, C>::value_type));
    stream.Write(map.GetData(), map.size() * sizeof(typename CVectorMap<V, K, C>::value_type));
    stream.WriteBool(map.Dirty);
}
typedef CVectorMap<CTextureMorph, unsigned long, CKeyPairCompareLess<unsigned long, CTextureMorph> > CMorphMap;
int main()
{
    bool ok = true;
    CMorphMap map;
    map.Dirty = true;
    map.Entries.resize(3);
    CDataOutputStreamWriter stream;
    WriteVectorMapMemDump(stream, map);
    const long bytes = 3 * (long)sizeof(CMorphMap::value_type);
    const CWriteLog& log = stream.Log;
    if (log.Count != 4 || log.Value[0] != 3 || log.Value[1] != bytes || log.Value[2] != bytes || log.Text[2] != (const char*)map.Entries._M_start || log.Value[3] != 1) ok = false;
    CMorphMap empty;
    empty.Dirty = false;
    CDataOutputStreamWriter stream2;
    WriteVectorMapMemDump(stream2, empty);
    if (stream2.Log.Count != 4 || stream2.Log.Value[1] != 0 || stream2.Log.Text[2] != (const char*)&empty || stream2.Log.Value[3] != 0) ok = false;
    if (!ok) { std::printf("0086b210_TEST FAIL\n"); return 1; }
    std::printf("0086b210_TEST PASS\n");
    return 0;
}