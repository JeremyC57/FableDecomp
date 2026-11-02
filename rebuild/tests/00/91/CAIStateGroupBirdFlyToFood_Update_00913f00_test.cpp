// Test: CAIStateGroup_BirdFlyToFood::Update() stamps the current frame, flies to the food position, then Terminate().
#include <cstdio>
static int g_terminate = 0, g_fly = 0, g_peek = 0;
static const void* g_flyTarget = 0;
class C3DVector { public: float X, Y, Z; };
class CWorld { public:
    static long GetFrame() { return 1234; }
};
static CWorld g_world;
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() { }
    void Terminate() { ++g_terminate; }
protected:
    const CWorld& PeekWorld() const { ++g_peek; return g_world; }
};
class CAIStateGroup_FlyingThingBase : public CAIStateGroupBase { public:
protected:
    int AIAction_RandomFlyToPos(const C3DVector& pos) { ++g_fly; g_flyTarget = &pos; return 0; }
};
class CAIStateGroup_BirdFlyToFood : public CAIStateGroup_FlyingThingBase { public:
    virtual void Update();
    C3DVector     FoodPos;
    unsigned long FrameLastFoundFood;
};
void CAIStateGroup_BirdFlyToFood::Update()
{
    FrameLastFoundFood = PeekWorld().GetFrame();
    AIAction_RandomFlyToPos(FoodPos);
    Terminate();
}
int main()
{
    CAIStateGroup_BirdFlyToFood bird;
    bird.FrameLastFoundFood = 0;
    bird.Update();
    if (bird.FrameLastFoundFood != 1234 || g_fly != 1 || g_flyTarget != &bird.FoodPos || g_terminate != 1 || g_peek != 1) {
        std::printf("00913f00_TEST FAIL\n"); return 1;
    }
    std::printf("00913f00_TEST PASS\n");
    return 0;
}