// CMemoryStream::CMemoryStream(IRemoteSource* source) -- EH-framed constructor of the memory-backed read stream: asks the source for the data size, allocates the buffer and loads the whole data into it (a failed size query or load leaves the remaining count 0)
// Leaf: ??0CMemoryStream
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31F60, 183 bytes, __fastcall `ret 4`, eax = this. Vtable 0x012B4E08 (slot 0 Read
//   0x00C31EB0, slot 1 the deleting destructor 0x00C32040 over the landed destructor 0x00C31F00); base vtable 0x012B4E00
//   (slot 0 _purecall, slot 1 0x00C31E90), whose inline destructor 0x00C31E80 is the EH state-0 unwind action.
//   Descriptive name (no PDB covers this library): the catalog's `CResourceContainer_InitializeWithCapacity` is
//   propagated; the landed destructor 0x00C31F00 is filed as CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch.
// Callees: the source's slot-0 size query, scalar operator new 0x00BFEA1A, CRemoteLoader::LoadDataFromRemote 0x00C32060
//   (row of this chunk) on a one-word loader local homed in the source's dead argument slot.
struct IRemoteSource {
    virtual bool ReadSize(unsigned long* size);     // slot 0
};

struct CRemoteLoader {
    IRemoteSource* m_pSource;
    bool LoadDataFromRemote(unsigned long offset, unsigned long size, void* dest);   // 0x00C32060
};

struct IDataStream {                        // vtable 0x012B4E00
    virtual bool Read(void* dest, unsigned long size) = 0;   // slot 0
    virtual ~IDataStream() { }              // slot 1
};

struct CMemoryStream : IDataStream {        // vtable 0x012B4E08
    char* m_pBuffer;                        // +0x04
    char* m_pCursor;                        // +0x08
    unsigned long m_remaining;              // +0x0C
    CMemoryStream(IRemoteSource* source);
    virtual bool Read(void* dest, unsigned long size);   // 0x00C31EB0
    virtual ~CMemoryStream();                            // 0x00C31F00
};

CMemoryStream::CMemoryStream(IRemoteSource* source)
    : m_pBuffer(0), m_pCursor(0), m_remaining(0)
{
    if (!source->ReadSize(&m_remaining)) {
        m_remaining = 0;
        return;
    }
    if (m_remaining) {
        m_pBuffer = (char*)::operator new(m_remaining);
        m_pCursor = m_pBuffer;
        CRemoteLoader loader;
        loader.m_pSource = source;
        if (!loader.LoadDataFromRemote(0, m_remaining, m_pBuffer))
            m_remaining = 0;
    }
}