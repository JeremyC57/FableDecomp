#include <cstdio>
struct IDirect3DTexture9 {
    int refs;
    virtual long QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long AddRef() { return ++refs; }
    virtual unsigned long Release() { return --refs; }
};
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;
    unsigned int ByteLength : 28;
    unsigned int AllocationSource : 4;
    CTexture() : PD3DTexture(0) { ByteLength = 0; AllocationSource = 0; }
    CTexture(const CTexture& other)
    {
        PD3DTexture = other.PD3DTexture;
        ByteLength = other.ByteLength;
        AllocationSource = other.AllocationSource;
        if (PD3DTexture)
            PD3DTexture->AddRef();
    }
};
int main()
{
    IDirect3DTexture9 tex; tex.refs = 1;
    CTexture a; a.PD3DTexture = &tex; a.ByteLength = 0x0abcdef; a.AllocationSource = 9;
    CTexture b(a);
    CTexture empty; empty.ByteLength = 5; empty.AllocationSource = 2;
    CTexture c(empty);
    if (b.PD3DTexture != &tex || b.ByteLength != 0x0abcdef || b.AllocationSource != 9 || tex.refs != 2 ||
        c.PD3DTexture != 0 || c.ByteLength != 5 || c.AllocationSource != 2) { std::printf("009f9d00_TEST FAIL\n"); return 1; }
    std::printf("009f9d00_TEST PASS\n");
    return 0;
}