// Behaviour test for CEngineWaterRenderer::ProcessSeaLoading (0x00B6DC40): a standalone fixture models the members the row touches
// (request / loaded ids, the bank file, the counted async pointers, the two sea buffers) and runs the row's own body verbatim through
// every arm: empty request, unchanged loaded sea, missing bank, missing entry, read start, read pending, request change, read finished.
#include <stdio.h>
#include <stddef.h>
#include <string>

struct CCharString {
    std::string Text;
    CCharString(const char* s = "") : Text(s) {}
    bool IsEmpty() const { return Text.empty(); }
    bool operator!=(const CCharString& s) const { return Text != s.Text; }
    CCharString operator+(const CCharString& s) const
    {
        CCharString result;
        result.Text = Text + s.Text;
        return result;
    }
};
template<class T> struct CCountedPointer {
    T* Ptr;
    unsigned Releases;
    CCountedPointer(T* ptr = 0) : Ptr(ptr), Releases(0) {}
    T* Get() const { return Ptr; }
    T* operator->() const { return Ptr; }
    CCountedPointer& operator=(const CCountedPointer& src)
    {
        Ptr = src.Ptr;
        return *this;
    }
    void ReleaseData() { if (Ptr) ++Releases; Ptr = 0; }
    void Reset(T* p) { ReleaseData(); Ptr = p; }
};
enum EThreadedFilePriority { PRIORITY_LOW, PRIORITY_HIGH };
EThreadedFilePriority g_HighLoadingPriority = PRIORITY_HIGH;
struct CBankFileAsyncData {
    static unsigned Discards;
    bool Finished;
    const char* Bytes;
    CBankFileAsyncData() : Finished(false), Bytes("sea data") {}
    static void DiscardAsyncData(CCountedPointer<CBankFileAsyncData>&) { ++Discards; }
    bool IsFinished() const { return Finished; }
    const char* GetData() const { return Bytes; }
    unsigned long GetLength() const { return 8; }
};
unsigned CBankFileAsyncData::Discards = 0;
struct CBankFileAsyncEntry {
    CBankFileAsyncData Data;
    unsigned Reads;
    bool BadArgument;
    CBankFileAsyncEntry() : Reads(0), BadArgument(false) {}
    unsigned long GetDataSize() const { return 8; }
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size,
        EThreadedFilePriority priority)
    {
        ++Reads;
        if (offset != 0 || size != 8 || priority != PRIORITY_HIGH)
            BadArgument = true;
        return CCountedPointer<CBankFileAsyncData>(&Data);
    }
};
struct CStaticMapBankFile {
    CBankFileAsyncEntry Entry;
    CCharString Lookup;
    bool HaveEntry, BadIndex;
    CStaticMapBankFile() : HaveEntry(true), BadIndex(false) {}
    unsigned long FindIndexBySymbol(const CCharString& symbol)
    {
        Lookup = symbol;
        return HaveEntry ? 22 : 0;
    }
    CCountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long index)
    {
        if (index != 22)
            BadIndex = true;
        return CCountedPointer<CBankFileAsyncEntry>(&Entry);
    }
};
struct CMemoryDataInputStream {
    const char* Data;
    unsigned long Size;
    CMemoryDataInputStream(const char* data, unsigned long size) : Data(data), Size(size) {}
};
struct CBuffer {
    unsigned Uninitialises;
    CBuffer() : Uninitialises(0) {}
    void Uninitialise() { ++Uninitialises; }
};
struct CEngineWaterRenderer {
    CCharString RequestedSeaBodyID, LoadedOrLoadingSeaBodyID, StaticMapSeaEntryPrefix;
    CStaticMapBankFile* StaticMapBankFile;
    CCountedPointer<CBankFileAsyncData> LoadingSea;
    CCountedPointer<CBankFileAsyncEntry> LoadingSeaEntry;
    CBuffer SeaBackgroundVertexBuffer, SeaBackgroundIndexBuffer;
    unsigned SeaBackgroundVertexCount, SeaBackgroundPolyCount, Loads;
    bool HaveBackgroundSea, BadStream;
    CEngineWaterRenderer() : StaticMapSeaEntryPrefix("SEA_"), StaticMapBankFile(0), SeaBackgroundVertexCount(23),
        SeaBackgroundPolyCount(8), Loads(0), HaveBackgroundSea(true), BadStream(false) {}
    void ReleaseSeaBuffers();
    void LoadSea(CMemoryDataInputStream& stream)
    {
        HaveBackgroundSea = true;
        ++Loads;
        if (stream.Size != 8 || std::string(stream.Data, stream.Size) != "sea data")
            BadStream = true;
    }
    bool ProcessSeaLoading();
};

void CEngineWaterRenderer::ReleaseSeaBuffers()
{
    SeaBackgroundVertexBuffer.Uninitialise();
    SeaBackgroundVertexCount = 0;
    SeaBackgroundIndexBuffer.Uninitialise();
    SeaBackgroundPolyCount = 0;
    HaveBackgroundSea = false;
}

bool CEngineWaterRenderer::ProcessSeaLoading()
{
    if (RequestedSeaBodyID.IsEmpty()) {
        if (LoadingSea.Get()) {
            CBankFileAsyncData::DiscardAsyncData(LoadingSea);
            LoadingSea.Reset(NULL);
            LoadingSeaEntry.Reset(NULL);
        }
        if (HaveBackgroundSea) {
            ReleaseSeaBuffers();
        }
        return false;
    } else if (!HaveBackgroundSea || RequestedSeaBodyID != LoadedOrLoadingSeaBodyID) {
        if (HaveBackgroundSea) {
            ReleaseSeaBuffers();
        }
        if (RequestedSeaBodyID != LoadedOrLoadingSeaBodyID) {
            if (LoadingSea.Get()) {
                CBankFileAsyncData::DiscardAsyncData(LoadingSea);
                LoadingSea.Reset(NULL);
                LoadingSeaEntry.Reset(NULL);
            }
        }
        if (!StaticMapBankFile) {
            return false;
        }
        if (!LoadingSea.Get()) {
            unsigned long smap_index = StaticMapBankFile->FindIndexBySymbol(StaticMapSeaEntryPrefix + RequestedSeaBodyID);
            if (smap_index) {
                LoadedOrLoadingSeaBodyID = RequestedSeaBodyID;
                LoadingSeaEntry = StaticMapBankFile->GetAsyncEntry(smap_index);
                LoadingSea = LoadingSeaEntry->BeginReadDataAutoAlloc(0, LoadingSeaEntry->GetDataSize(), g_HighLoadingPriority);
                return true;
            } else {
                return false;
            }
        }
        if (LoadingSea->IsFinished()) {
            CMemoryDataInputStream stream(LoadingSea->GetData(), LoadingSea->GetLength());
            LoadSea(stream);
            LoadingSea.Reset(NULL);
            LoadingSeaEntry.Reset(NULL);
            return false;
        } else {
            return true;
        }
    }
    return false;
}

int main()
{
    CEngineWaterRenderer s;
    CStaticMapBankFile bank;
    s.StaticMapBankFile = &bank;
    s.LoadedOrLoadingSeaBodyID = CCharString("A");
    s.LoadingSea = CCountedPointer<CBankFileAsyncData>(&bank.Entry.Data);
    s.LoadingSeaEntry = CCountedPointer<CBankFileAsyncEntry>(&bank.Entry);
    if (s.ProcessSeaLoading() || CBankFileAsyncData::Discards != 1 || s.LoadingSea.Get() || s.LoadingSeaEntry.Get() ||
        s.HaveBackgroundSea || s.SeaBackgroundVertexCount || s.SeaBackgroundPolyCount ||
        s.SeaBackgroundVertexBuffer.Uninitialises != 1 || s.SeaBackgroundIndexBuffer.Uninitialises != 1)
        return 1;

    s.RequestedSeaBodyID = CCharString("A");
    s.HaveBackgroundSea = true;
    if (s.ProcessSeaLoading() || s.SeaBackgroundVertexBuffer.Uninitialises != 1 || !s.HaveBackgroundSea)
        return 2;
    s.RequestedSeaBodyID = CCharString("B");
    s.StaticMapBankFile = 0;
    if (s.ProcessSeaLoading() || s.SeaBackgroundVertexBuffer.Uninitialises != 2 || s.HaveBackgroundSea)
        return 3;
    s.StaticMapBankFile = &bank;
    bank.HaveEntry = false;
    if (s.ProcessSeaLoading() || bank.Entry.Reads || bank.Lookup.Text != "SEA_B")
        return 4;
    bank.HaveEntry = true;
    if (!s.ProcessSeaLoading() || bank.Entry.Reads != 1 || s.LoadedOrLoadingSeaBodyID.Text != "B")
        return 5;
    if (!s.ProcessSeaLoading() || bank.Entry.Reads != 1 || s.Loads)
        return 6;
    s.RequestedSeaBodyID = CCharString("C");
    if (!s.ProcessSeaLoading() || CBankFileAsyncData::Discards != 2 || bank.Entry.Reads != 2 ||
        s.LoadedOrLoadingSeaBodyID.Text != "C" || bank.Lookup.Text != "SEA_C")
        return 7;
    bank.Entry.Data.Finished = true;
    if (s.ProcessSeaLoading() || s.Loads != 1 || s.LoadingSea.Get() || s.LoadingSeaEntry.Get() || !s.HaveBackgroundSea ||
        bank.Entry.BadArgument || bank.BadIndex || s.BadStream)
        return 8;
    puts("00b6dc40_TEST PASS");
    return 0;
}