#include <cstdio>
#include <cstring>
#include <new>
// Behaviour of CTCHeroPodium::CTCHeroPodium (0x00771DB0): CTCBase(parent), no occupants, an empty occupation vector.
struct CThing { int id; };
struct CTCBase {
    virtual long v00() { return 0; }
    CThing* ParentThing;
    CTCBase(CThing* parent) : ParentThing(parent) {}
};
struct C3DVector { float x, y, z; C3DVector() {} };
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    CLongVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};
struct CTCHeroPodium : CTCBase {
    long NumberOfCrowdOccupants;
    CLongVector CrowdOccupation;
    C3DVector E_X;
    C3DVector E_Y;
    virtual long v00() { return 3; }
    CTCHeroPodium(CThing* parent) : CTCBase(parent), NumberOfCrowdOccupants(0) {}
};
union UStorage { double d; void* p; unsigned char bytes[sizeof(CTCHeroPodium)]; };
int main()
{
    CThing thing;
    UStorage storage;
    std::memset(storage.bytes, 0xEE, sizeof(storage.bytes));
    CTCHeroPodium* podium = new (storage.bytes) CTCHeroPodium(&thing);
    if (podium->ParentThing != &thing || podium->NumberOfCrowdOccupants != 0 || podium->CrowdOccupation.m_pFirst != 0
        || podium->CrowdOccupation.m_pLast != 0 || podium->CrowdOccupation.m_pEnd != 0 || podium->v00() != 3) {
        std::printf("00771db0_TEST FAIL\n"); return 1;
    }
    std::printf("00771db0_TEST PASS\n");
    return 0;
}
