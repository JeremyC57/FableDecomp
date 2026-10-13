#include <cstdio>

struct CMazeAngleSource {
    virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void v0c(){}
    virtual void v10(){} virtual void v14(){} virtual void v18(){} virtual void v1c(){}
    virtual void v20(){} virtual void v24(){}
    virtual float GetAngle() { return 1.5f; }
    unsigned char pad[8];
};
struct CGTDI_Maze { void* vt; unsigned char p[4]; CMazeAngleSource AngleSource; void* a; void* b; float InitialAngle;
    void Init() { InitialAngle = AngleSource.GetAngle(); } };
int main()
{
    CGTDI_Maze m; m.InitialAngle = 0.0f; m.Init();
    if (m.InitialAngle != 1.5f) { std::printf("00e27c60_TEST FAIL\n"); return 1; }
    std::printf("00e27c60_TEST PASS\n");
    return 0;
}
