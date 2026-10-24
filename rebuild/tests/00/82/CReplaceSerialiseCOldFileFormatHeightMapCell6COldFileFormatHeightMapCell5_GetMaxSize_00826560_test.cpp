// Behaviour test: CReplaceSerialise<Cell6,Cell5>::GetMaxSize -- the largest record size along the version chain from the given version up to 6 (Cell5 is the biggest)
#include <cstdio>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned int  ByteLength;
    unsigned char SerialiseVersionNumber;
    static unsigned short GetClassVersionNumber() { return T::VERSION; }
    static long GetMaxSize(long version, long size)
    {
        if (version == GetClassVersionNumber())
            return MAX(size, (long)sizeof(T));
        return MAX(TOld::GetMaxSize(version, size), (long)sizeof(T));
    }
};
template <class T, long V> class CBaseSerialise {
public:
    unsigned int  ByteLength;
    unsigned char SerialiseVersionNumber;
    static long GetMaxSize(long, long size) { return MAX(size, (long)sizeof(T)); }
};
class Cell1 : public CBaseSerialise<Cell1, 1> { public: enum { VERSION = 1 }; float Height; };
class Cell2 : public CReplaceSerialise<Cell2, Cell1> { public: enum { VERSION = 2 }; float Height; unsigned char Alpha; };
class Cell3 : public CReplaceSerialise<Cell3, Cell2> { public: enum { VERSION = 3 }; float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; };
class Cell4 : public CReplaceSerialise<Cell4, Cell3> { public: enum { VERSION = 4 }; float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; unsigned char Passable; unsigned char CameraPassable; };
class Cell5 : public CReplaceSerialise<Cell5, Cell4> { public: enum { VERSION = 5 }; float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; unsigned char Passable; unsigned char CameraPassable; int SoundLayer; int MinimapZone; };
class Cell6 : public CReplaceSerialise<Cell6, Cell5> { public: enum { VERSION = 6 }; float Height; unsigned char Alpha; unsigned char Theme[3]; unsigned char Blend[2]; unsigned char Passable; unsigned char CameraPassable; signed char SoundLayer; signed char MinimapZone; bool ShorePoint; bool NavPreferred; };
#pragma pack(pop)

int main()
{
    if (sizeof(Cell1) != 9 || sizeof(Cell2) != 10 || sizeof(Cell3) != 15 || sizeof(Cell4) != 17 || sizeof(Cell5) != 25 || sizeof(Cell6) != 21)
        { std::printf("00826560_TEST FAIL\n"); return 1; }
    if (Cell6::GetMaxSize(6, 0) != 21)
        { std::printf("00826560_TEST FAIL\n"); return 1; }
    if (Cell6::GetMaxSize(6, 30) != 30)
        { std::printf("00826560_TEST FAIL\n"); return 1; }
    if (Cell6::GetMaxSize(5, 0) != 25)
        { std::printf("00826560_TEST FAIL\n"); return 1; }
    if (Cell6::GetMaxSize(4, 0) != 25)
        { std::printf("00826560_TEST FAIL\n"); return 1; }
    if (Cell6::GetMaxSize(1, 0) != 25)
        { std::printf("00826560_TEST FAIL\n"); return 1; }
    if (Cell6::GetMaxSize(2, 26) != 26)
        { std::printf("00826560_TEST FAIL\n"); return 1; }
    std::printf("00826560_TEST PASS\n");
    return 0;
}