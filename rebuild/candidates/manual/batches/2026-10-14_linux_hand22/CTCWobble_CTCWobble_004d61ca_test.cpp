#include <cstdio>
struct CThing { int id; };
struct CTCBase {
    virtual void v00() { }
    CThing* m_pOwner;
    CTCBase(CThing* owner) : m_pOwner(owner) { }
};
struct CTCWobble : CTCBase {
    unsigned char _pad[0x20];
    long  FramesOfWobbleRemaining;
    float DirectionLastRotated;
    float AngleOfWobble;
    virtual void v00() { }
    CTCWobble(CThing* owner);
};
CTCWobble::CTCWobble(CThing* owner)
    : CTCBase(owner), FramesOfWobbleRemaining(0), DirectionLastRotated(1.0f), AngleOfWobble(0.0f)
{
}
int main()
{
    CThing thing = { 9 };
    CTCWobble w(&thing);
    if (w.m_pOwner != &thing || w.FramesOfWobbleRemaining != 0 || w.DirectionLastRotated != 1.0f || w.AngleOfWobble != 0.0f) { std::printf("004d61ca_TEST FAIL\n"); return 1; }
    std::printf("004d61ca_TEST PASS\n");
    return 0;
}
