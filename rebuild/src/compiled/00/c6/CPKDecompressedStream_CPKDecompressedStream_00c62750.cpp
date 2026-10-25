// CPKDecompressedStream::~CPKDecompressedStream() -- release the decompressor and close the source object, nulling both
// Leaf: ??1CPKDecompressedStream@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62750, 41 bytes (thiscall, non-virtual: callers 0x00C47F85 / 0x00C48186 call
//   it then MSVCR71 `operator delete` 0x00BFE9BC, and the scalar deleting wrapper at 0x00C48750 calls it too).
// Callees: CDecompressorInterface::Release 0x00C73D90 (logs "Null decompressor" from .\CDecompressorInterface.cpp line
//   0x3a when called on null, else deletes through vtable slot 0), the +0 object's vtable slot 3 (+0xc).
// Identity: the manifest's "IUnknown_Release" is a propagated label (slot 3, not IUnknown's slot 2). Same object as
//   Seek 0x00C626A0. Descriptive class name.
class CDecompressorInterface {
public:
    virtual ~CDecompressorInterface();
    virtual long GetUnitSize();                        // slot 1 (+0x04) (name inferred)
    virtual long Seek(long position, bool relative);   // slot 2 (+0x08) (bool name inferred)
    virtual void v03();
    virtual long GetRate();                            // slot 4 (+0x10) (name inferred: scaled by 1/1000)
    void Release();                                    // 0x00C73D90
};
class CPKSourceObject {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Close();                              // slot 3 (+0x0c)
};
class CPKDecompressedStream {
public:
    CPKSourceObject*        m_pSource;                 // +0x00
    CDecompressorInterface* m_pDecompressor;           // +0x04
    long                    m_position;                // +0x08
    long                    m_userData;                // +0x0c
    bool                    m_bFlag;                   // +0x10
    long Seek(long mode, float amount, long userData);
    ~CPKDecompressedStream();
};

CPKDecompressedStream::~CPKDecompressedStream()
{
    if (m_pDecompressor)
    {
        m_pDecompressor->Release();
        m_pDecompressor = 0;
    }
    if (m_pSource)
    {
        m_pSource->Close();
        m_pSource = 0;
    }
}