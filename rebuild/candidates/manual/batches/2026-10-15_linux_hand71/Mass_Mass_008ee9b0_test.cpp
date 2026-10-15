#include <cstdio>

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

struct Mass {
    C2DVector Pos; C2DVector Vel; float IMass; float APos; float AVel; float AIMass;
    Mass()
    {
        Pos = C2DVector(0.0f, 0.0f);
        Vel = C2DVector(0.0f, 0.0f);
        APos = 0.0f;
        AVel = 0.0f;
    }
    virtual ~Mass() { }
};

int main()
{
    Mass* m = new Mass;
    if (m->Pos.X != 0.0f || m->Pos.Y != 0.0f || m->Vel.X != 0.0f || m->Vel.Y != 0.0f || m->APos != 0.0f || m->AVel != 0.0f)
    { std::printf("008ee9b0_TEST FAIL\n"); return 1; }
    delete m;
    std::printf("008ee9b0_TEST PASS\n");
    return 0;
}
