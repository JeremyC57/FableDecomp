// Behaviour test for CWaterPatchTesselatorBase<CTVertexSeaBackground>::SetBridgeVertexBufferSize (0x00B80100).
#include <cstdio>

class CTVertexSeaBackground {
public:
    CTVertexSeaBackground() {}
    float X; float Y; float Z;
};

static unsigned int g_resizeTo = 0; static int g_resizeCalls = 0;
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long GetCount() const { return _M_finish - _M_start; }
    T* GetData() { return GetCount() ? _M_start : (T*)this; }
    void resize(unsigned int newSize, const T& x) { g_resizeTo = newSize; ++g_resizeCalls; _M_finish = _M_start + newSize; }
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

template <class T> class CWaterPatchTesselatorBase {
public:
    virtual ~CWaterPatchTesselatorBase() {}
    virtual long GetBridgeVertexBufferSize();
    virtual unsigned char* GetBridgeVertexBufferAsCArray();
    virtual void SetBridgeVertexBufferSize(long size);
    unsigned char _base[0x10];
    CArray<T> BridgeVertexBuffer;
};

template <class T> long CWaterPatchTesselatorBase<T>::GetBridgeVertexBufferSize()
{
    return BridgeVertexBuffer.GetCount();
}
template <class T> unsigned char* CWaterPatchTesselatorBase<T>::GetBridgeVertexBufferAsCArray()
{
    return (unsigned char*)BridgeVertexBuffer.GetData();
}
template <class T> void CWaterPatchTesselatorBase<T>::SetBridgeVertexBufferSize(long size)
{
    BridgeVertexBuffer.resize(size);
}

int main()
{
    static CTVertexSeaBackground storage[8];
    CWaterPatchTesselatorBase<CTVertexSeaBackground> t;
    t.BridgeVertexBuffer._M_start = storage; t.BridgeVertexBuffer._M_finish = storage + 5; t.BridgeVertexBuffer._M_end_of_storage = storage + 8;
    t.SetBridgeVertexBufferSize(3);
    if (g_resizeCalls != 1 || g_resizeTo != 3 || t.GetBridgeVertexBufferSize() != 3) { std::printf("00b80100_TEST FAIL\n"); return 1; }
    std::printf("00b80100_TEST PASS\n");
    return 0;
}