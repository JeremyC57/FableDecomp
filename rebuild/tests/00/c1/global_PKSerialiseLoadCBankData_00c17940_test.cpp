// Behaviour test for PKSerialiseLoad<CBankData> (0x00C17940): load the source into a stream, build the object under an
// auto-delete owner, transfer it; success hands the object out, failure deletes it and returns null.
#include <cstdio>

static int g_live, g_traces, g_streamsDestroyed;
struct IRemoteSource { bool ok; };
struct CMemoryStream {
    IRemoteSource* src;
    CMemoryStream(IRemoteSource* s) : src(s) { }
    ~CMemoryStream() { ++g_streamsDestroyed; }
};
struct PKSerialiseCParser {
    CMemoryStream* m_pReader; void* m_pStream;
    PKSerialiseCParser(CMemoryStream* reader) : m_pReader(reader), m_pStream(0) { }
};
struct CBankData {
    int loaded;
    CBankData() : loaded(0) { ++g_live; }
    ~CBankData() { --g_live; }
    bool Transfer(PKSerialiseCParser& parser) { if (!parser.m_pReader->src->ok) return false; loaded = 1; return true; }
};
template <class T> class PKCAutoDelete {
public:
    T* m_pObject;
    PKCAutoDelete(T* object) : m_pObject(object) { }
    ~PKCAutoDelete() { if (m_pObject) delete m_pObject; }
    T* GetChecked() { if (m_pObject == 0) ++g_traces; return m_pObject; }
    T* Release() { if (m_pObject == 0) ++g_traces; T* o = m_pObject; m_pObject = 0; return o; }
    T* operator->() { return GetChecked(); }
};
template <class T> T* PKSerialiseLoad(IRemoteSource* source)
{
    CMemoryStream stream(source);
    PKCAutoDelete<T> object(new T);
    if (object.m_pObject == 0) ++g_traces;
    PKSerialiseCParser parser(&stream);
    if (!object->Transfer(parser))
        return 0;
    return object.Release();
}

int main()
{
    IRemoteSource good = { true }, bad = { false };
    CBankData* d = PKSerialiseLoad<CBankData>(&good);
    if (!d || !d->loaded || g_live != 1 || g_streamsDestroyed != 1) { std::printf("00c17940_TEST FAIL\n"); return 1; }
    delete d;
    CBankData* e = PKSerialiseLoad<CBankData>(&bad);
    if (e || g_live != 0 || g_streamsDestroyed != 2 || g_traces != 0) { std::printf("00c17940_TEST FAIL\n"); return 1; }
    std::printf("00c17940_TEST PASS\n");
    return 0;
}