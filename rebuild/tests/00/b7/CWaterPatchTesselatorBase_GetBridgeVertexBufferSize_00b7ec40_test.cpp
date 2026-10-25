// Behaviour test for CWaterPatchTesselatorBase<CTVertexWaterBackground>::GetBridgeVertexBufferSize (0x00B7EC40).
#include <cstdio>

class CTVertexWaterBackground {
public:
    CTVertexWaterBackground() {}
    short X; short Y; float Z; float ShoreLookUpArray[12];
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
    static CTVertexWaterBackground storage[8];
    CWaterPatchTesselatorBase<CTVertexWaterBackground> t;
    t.BridgeVertexBuffer._M_start = storage; t.BridgeVertexBuffer._M_finish = storage + 5; t.BridgeVertexBuffer._M_end_of_storage = storage + 8;
    if (t.GetBridgeVertexBufferSize() != 5) { std::printf("00b7ec40_TEST FAIL\n"); return 1; }
    t.BridgeVertexBuffer._M_finish = storage;
    if (t.GetBridgeVertexBufferSize() != 0) { std::printf("00b7ec40_TEST FAIL\n"); return 1; }
    std::printf("00b7ec40_TEST PASS\n");
    return 0;
}