// Behaviour test for 0x009E93C0 CMemoryFile::Open(void*, long).
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CWideString { public:
    const unsigned short* Text;
    CWideString& operator=(const unsigned short* text) { Text = text; return *this; }
};
static const unsigned short kEmpty[1] = { 0 };

struct vector {
    unsigned char* _M_start; unsigned char* _M_finish; unsigned char* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void resize(unsigned int n, const unsigned char& value)
    {
        unsigned char* p = (unsigned char*)std::malloc(n ? n : 1);
        unsigned int old = size();
        for (unsigned int i = 0; i < n; ++i) p[i] = i < old ? _M_start[i] : value;
        std::free(_M_start);
        _M_start = p; _M_finish = p + n; _M_end_of_storage = p + n;
    }
    unsigned char* GetData() { return size() ? _M_start : (unsigned char*)this; }
};

class CMemoryFile { public:
    long AccessType; long Position; long Length; vector DataBuffer; bool Opened; CWideString Pathname;
    void Open(void* data, long length)
    {
        Pathname = kEmpty;
        Position = 0;
        Length = length;
        Opened = true;
        DataBuffer.resize(length, 0);
        std::memcpy(DataBuffer.GetData(), data, length);
    }
};

int main()
{
    CMemoryFile f;
    f.AccessType = 4; f.Position = 9; f.Length = 0; f.Opened = false; f.Pathname.Text = 0;
    f.DataBuffer._M_start = f.DataBuffer._M_finish = f.DataBuffer._M_end_of_storage = 0;
    unsigned char data[5] = { 'h', 'e', 'l', 'l', 'o' };
    f.Open(data, 5);
    bool ok = f.Position == 0 && f.Length == 5 && f.Opened && f.Pathname.Text == kEmpty && f.AccessType == 4
           && f.DataBuffer.size() == 5 && std::memcmp(f.DataBuffer._M_start, "hello", 5) == 0;
    f.Open(data, 0);
    ok = ok && f.Length == 0 && f.DataBuffer.size() == 0;
    std::free(f.DataBuffer._M_start);
    if (!ok) { std::printf("009e93c0_TEST FAIL\n"); return 1; }
    std::printf("009e93c0_TEST PASS\n");
    return 0;
}