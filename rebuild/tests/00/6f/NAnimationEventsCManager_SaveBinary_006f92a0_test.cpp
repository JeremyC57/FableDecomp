// Behaviour test for NAnimationEvents::CManager::SaveBinary @ 0x006F92A0: the serialised value sequence and the file write.
#include <cstdio>

static unsigned long g_out[64]; static int g_n = 0;
static int g_fileWrites = 0; static long g_fileSize = 0;
class CWideString { public: int dummy; };
class CMemoryDataOutputStream { public:
    void WriteEBOOL(bool v) { g_out[g_n++] = v ? 1 : 0; }
    void WriteULONG(unsigned long v) { g_out[g_n++] = v; }
    void WriteSLONG(long v) { g_out[g_n++] = (unsigned long)v; }
    void WriteFLOAT(float v) { g_out[g_n++] = (unsigned long)(v * 10.0f); }
    long GetSize() const { return g_n * 4; }
    const void* PeekData() const { return g_out; }
};
class CDiskFileWin32 { public:
    CDiskFileWin32(const CWideString&, int access, unsigned long flags) { if (access != 4 || flags != 2) g_fileWrites = -100; }
    void Write(const void* data, long size, void* process) { if (data == g_out && !process) { ++g_fileWrites; g_fileSize = size; } }
};
struct CEvent { unsigned long Packed; float Time; };
struct CEventList {
    CEvent* Data; long Count;
    long size() const { return Count; }
    const CEvent& operator[](long n) const { return Data[n]; }
};
struct CAnimEvents { unsigned long AnimID; CEventList Events; };
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
};
struct CManager {
    vector<CAnimEvents> ExtraAnimEvents;
    void SaveBinary(const CWideString& fileName);
};
void CManager::SaveBinary(const CWideString& fileName)
{
    CMemoryDataOutputStream stream;
    stream.WriteSLONG(ExtraAnimEvents.size());
    for (unsigned long i = 0; i < ExtraAnimEvents.size(); ++i) {
        stream.WriteSLONG(ExtraAnimEvents[i].AnimID);
        const CEventList& events = ExtraAnimEvents[i].Events;
        stream.WriteSLONG(events.size());
        for (long j = 0; j < events.size(); ++j) {
            stream.WriteULONG(events[j].Packed & 0x3fffffff);
            stream.WriteEBOOL((events[j].Packed & 0x40000000) == 0x40000000);
            stream.WriteEBOOL((events[j].Packed & 0x80000000) == 0x80000000);
            stream.WriteFLOAT(events[j].Time);
        }
    }
    CDiskFileWin32 file(fileName, 4, 2);
    file.Write(stream.PeekData(), stream.GetSize(), 0);
}
int main()
{
    CEvent ev[2] = { { 0x40000007UL, 1.5f }, { 0x80000009UL, 2.0f } };
    CAnimEvents anims[2];
    anims[0].AnimID = 11; anims[0].Events.Data = ev; anims[0].Events.Count = 2;
    anims[1].AnimID = 12; anims[1].Events.Data = 0; anims[1].Events.Count = 0;
    CManager m; m.ExtraAnimEvents._M_start = anims; m.ExtraAnimEvents._M_finish = anims + 2; m.ExtraAnimEvents._M_end_of_storage = anims + 2;
    CWideString name;
    m.SaveBinary(name);
    static const unsigned long expect[] = { 2, 11, 2, 7, 1, 0, 15, 9, 0, 1, 20, 12, 0 };
    int ok = g_n == 13 && g_fileWrites == 1 && g_fileSize == 52;
    for (int i = 0; ok && i < 13; ++i) if (g_out[i] != expect[i]) ok = 0;
    if (!ok) { std::printf("006f92a0_TEST FAIL\n"); return 1; }
    std::printf("006f92a0_TEST PASS\n");
    return 0;
}