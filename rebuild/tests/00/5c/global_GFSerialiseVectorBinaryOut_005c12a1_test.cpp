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
struct CItem {
    long ItemType;
    long NumberOfItems;
    long AverageBoughtPrice;
    CItem() : ItemType(0), NumberOfItems(0), AverageBoughtPrice(0) {}
};
template <> struct CPersistTraits<CItem> {
    CItem DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, CItem* out) const { out->ItemType = stream.Next(); out->NumberOfItems = stream.Next(); out->AverageBoughtPrice = stream.Next(); }
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const CItem* value) const { stream.WriteLong(value->ItemType); stream.WriteLong(value->NumberOfItems); }
};
int main()
{
    bool ok = true;
    CVector<CItem> value;
    value.resize(2);
    value[0].ItemType = 7; value[0].NumberOfItems = 1; value[1].ItemType = 8; value[1].NumberOfItems = 4;
    CDataOutputStreamWriter stream;
    SerialiseVectorBinaryOut(stream, value);
    const CWriteLog& log = stream.Log;
    if (log.Count != 5 || log.Value[0] != 2 || log.Value[1] != 7 || log.Value[2] != 1 || log.Value[3] != 8 || log.Value[4] != 4) ok = false;
    if (!ok) { std::printf("005c12a1_TEST FAIL\n"); return 1; }
    std::printf("005c12a1_TEST PASS\n");
    return 0;
}