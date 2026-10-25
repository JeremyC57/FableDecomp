// Behaviour test for Coin::`scalar deleting destructor' (0x008EFFD0): deleting a Coin through a Mass pointer releases
// the Thing intelligent pointer once and frees the object; the layout puts Thing right after Owned.
#include <cstdio>
#include <stddef.h>

static int g_released = 0;
class CThing;

#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PItem(0) {}
    virtual ~CBaseIntelligentPointer() { ++g_released; }
    void* PItem;
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
};
#pragma pack(pop)

struct C2DVector { float X, Y; };

class Mass {
public:
    virtual ~Mass() {}
    C2DVector Pos;
    C2DVector Vel;
    float IMass;
    float APos;
    float AVel;
    float AIMass;
};

class Coin : public Mass {
public:
    bool Owned;
    CIntelligentPointer<CThing> Thing;
    float Radius;
    int Owner;
    unsigned long Sound;
};

int main()
{
    Mass* m = new Coin;
    delete m;
    if (g_released != 1) { std::printf("008effd0_TEST FAIL\n"); return 1; }
    if (offsetof(Coin, Thing) != offsetof(Coin, Owned) + 1) { std::printf("008effd0_TEST FAIL\n"); return 1; }
    {
        Coin local;
        local.Owned = true;
    }
    if (g_released != 2) { std::printf("008effd0_TEST FAIL\n"); return 1; }
    std::printf("008effd0_TEST PASS\n");
    return 0;
}