// Grade: DIFFER(773v771) -- one slot from a match: retail evaluates GetEntryDataSize before loading the read priority global (and reloads LoadingSeaEntry), and pops edi after the last UnloadSea store; every form that fixes the argument order loses ebp for &RequestedSeaBodyID
// CEngineWaterRenderer::ProcessSeaLoading() -- drive the asynchronous sea-body load: drop everything when no sea is requested, unload the old sea when the request changed, start the bank read for the requested body, and build the sea once the read finishes (true while a load is in flight)
// Leaf: ?ProcessSeaLoading@CEngineWaterRenderer
// Layout: retail CEngineWaterRenderer: HaveBackgroundSea +0x276, LoadedOrLoadingSeaBodyID +0x278, RequestedSeaBodyID +0x27c,
// StaticMapBankFile +0x5a8, StaticMapSeaEntryPrefix +0x5ac, LoadingSeaEntry +0x5b0, LoadingSea +0x5b8 (Ego_r.pdb names, retail offsets).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
struct CCountedItem { };
struct CCPPointerInfo {
    long m_count;
    void (CCountedItem::*m_pfnDestroy)();
    CCountedItem* m_pItem;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    ~CCountedPointer();
    __forceinline void Release()
    {
        if (Info && --Info->m_count == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            operator delete(Info);
        }
        Info = 0;
        Data = 0;
    }
};
struct CStringData { long RefCount; long Length; };
struct CCharString {
    CStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    bool IsEmpty() const { return !PStringData || PStringData->Length == 0; }
    bool operator!=(const CCharString& other) const;        // 0x0099E900
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
struct CBankFile { unsigned long GetEntryDataSize(unsigned long index); };   // 0x009CC240
struct CBankFileAsyncData {
    unsigned char _pad[0x10];
    unsigned long Size;                                     // +0x10
    bool IsFinished();                                      // 0x009D54E0
    void* PeekData();                                       // 0x009D5530
};
struct CBankFileAsyncEntry {
    void*         _v;
    CBankFile*    BankFile;                                 // +0x04
    unsigned long Index;                                    // +0x08
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, long priority);   // 0x009D6100
};
template <> CCountedPointer<CBankFileAsyncData>::~CCountedPointer();    // 0x009A4400
template <> CCountedPointer<CBankFileAsyncEntry>::~CCountedPointer();   // 0x009C9FC0
struct CAsyncDataPointer : CCountedPointer<CBankFileAsyncData> {
    CAsyncDataPointer& operator=(const CCountedPointer<CBankFileAsyncData>& other);   // 0x009A4440
};
struct CAsyncEntryPointer : CCountedPointer<CBankFileAsyncEntry> {
    CAsyncEntryPointer& operator=(const CCountedPointer<CBankFileAsyncEntry>& other);   // 0x00A5F6D0
};
void __fastcall DiscardAsyncData(CCountedPointer<CBankFileAsyncData>* data);   // 0x009D68A0
struct CStaticMapBankFile {
    unsigned long FindEntry(const CCharString& name);       // 0x009CCDC0
    CCountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long index);   // 0x009D58D0
};
extern long g_SeaLoadPriority;                              // 0x0139A728
struct CMemoryDataInputStream {
    unsigned char _body[0x1c];
    CMemoryDataInputStream(void* data, unsigned long size); // 0x00994930
    ~CMemoryDataInputStream();                              // 0x00993B10
};
struct CIndexBuffer { unsigned char _b[0x14]; void ReleaseVolume(); };      // 0x00AC1EE0
struct CVertexBufferWin32 { unsigned char _b[0x34]; void Uninitialise(); }; // 0x00A63200
struct CEngineWaterRenderer {
    unsigned char       _pad[0x68];
    CIndexBuffer        SeaBackgroundIndexBuffer;           // +0x68
    CVertexBufferWin32  SeaBackgroundVertexBuffer;          // +0x7c
    long                SeaBackgroundVertexCount;           // +0xb0
    long                SeaBackgroundPolyCount;             // +0xb4
    unsigned char       _padb8[0x1be];
    bool                HaveBackgroundSea;                  // +0x276
    unsigned char       _pad277;
    CCharString         LoadedOrLoadingSeaBodyID;           // +0x278
    CCharString         RequestedSeaBodyID;                 // +0x27c
    unsigned char       _pad280[0x328];
    CStaticMapBankFile* StaticMapBankFile;                  // +0x5a8
    CCharString         StaticMapSeaEntryPrefix;            // +0x5ac
    CAsyncEntryPointer  LoadingSeaEntry;                    // +0x5b0
    CAsyncDataPointer   LoadingSea;                         // +0x5b8
    void LoadSea(CMemoryDataInputStream* stream);           // 0x00B6D420
    __forceinline void CancelSeaLoad()
    {
        if (LoadingSea.Data) {
            DiscardAsyncData(&LoadingSea);
            LoadingSea.Release();
            LoadingSeaEntry.Release();
        }
    }
    __forceinline void UnloadSea()
    {
        if (HaveBackgroundSea) {
            SeaBackgroundVertexBuffer.Uninitialise();
            SeaBackgroundVertexCount = 0;
            SeaBackgroundIndexBuffer.ReleaseVolume();
            SeaBackgroundPolyCount = 0;
            HaveBackgroundSea = false;
        }
    }
    bool ProcessSeaLoading();
};

bool CEngineWaterRenderer::ProcessSeaLoading()
{
    if (RequestedSeaBodyID.IsEmpty()) {
        CancelSeaLoad();
        UnloadSea();
        return false;
    }
    if (HaveBackgroundSea) {
        if (!(RequestedSeaBodyID != LoadedOrLoadingSeaBodyID))
            return false;
        UnloadSea();
    }
    if (RequestedSeaBodyID != LoadedOrLoadingSeaBodyID)
        CancelSeaLoad();
    if (!StaticMapBankFile)
        return false;
    if (!LoadingSea.Data) {
        unsigned long entry = StaticMapBankFile->FindEntry(StaticMapSeaEntryPrefix + RequestedSeaBodyID);
        if (!entry)
            return false;
        LoadedOrLoadingSeaBodyID = RequestedSeaBodyID;
        LoadingSeaEntry = StaticMapBankFile->GetAsyncEntry(entry);
        LoadingSea = LoadingSeaEntry->BeginReadDataAutoAlloc(0, LoadingSeaEntry->BankFile->GetEntryDataSize(LoadingSeaEntry->Index), g_SeaLoadPriority);
        return true;
    }
    if (!LoadingSea->IsFinished())
        return true;
    CMemoryDataInputStream stream(LoadingSea->PeekData(), LoadingSea->Size);
    LoadSea(&stream);
    LoadingSea.Release();
    LoadingSeaEntry.Release();
    return false;
}
