#include <cstdio>
// Behaviour test for CMemoryDataOutputStream::GetLength() const (0x00994B40): Data.Size - Data.Start.
class CDataOutputStream { public:
    virtual ~CDataOutputStream() { }
    unsigned char _pad04[4];
};
template <class T> class CArray { public:
    T*            Start;
    unsigned long Size;
    unsigned char _pad08[4];
};
class CMemoryDataOutputStream : public CDataOutputStream { public:
    CArray<char>  Data;
    unsigned long CurrentPosition;
    unsigned long GetLength() const { return Data.Size - (unsigned long)Data.Start; }
};
int main()
{
    CMemoryDataOutputStream s; s.Data.Start = (char*)2; s.Data.Size = 7;
    if (s.GetLength() != 5) { std::printf("00994b40_TEST FAIL\n"); return 1; }

    std::printf("00994b40_TEST PASS\n");
    return 0;
}
