// Behaviour test for 0x00A55A80 C3DBoxF::ToC3DBoxI() const
#include <cstdio>

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
class C3DBoxI : public C2DBoxI {
public:
    long TLZ;
    long BRZ;
    C3DBoxI(long tlx, long tly, long tlz, long brx, long bry, long brz) : C2DBoxI(tlx, tly, brx, bry), TLZ(tlz), BRZ(brz) { }
    C3DBoxI(const C3DBoxI& other) : C2DBoxI(other), TLZ(other.TLZ), BRZ(other.BRZ) { }
};
class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
};
class C3DBoxF : public C2DBoxF {
public:
    float TLZ;
    float BRZ;
    C3DBoxI ToC3DBoxI() const
    {
        return C3DBoxI((long)TLX, (long)TLY, (long)TLZ, (long)BRX, (long)BRY, (long)BRZ);
    }
};

int main()
{
    C3DBoxF f;
    f.TLX = 1.7f; f.TLY = -2.3f; f.TLZ = 3.9f;          // truncation toward zero
    f.BRX = 10.2f; f.BRY = 20.8f; f.BRZ = -30.5f;
    C3DBoxI b = f.ToC3DBoxI();
    if (b.TLX != 1 || b.TLY != -2 || b.TLZ != 3 || b.BRX != 10 || b.BRY != 20 || b.BRZ != -30) {
        std::printf("00a55a80_TEST FAIL\n");
        return 1;
    }
    std::printf("00a55a80_TEST PASS\n");
    return 0;
}