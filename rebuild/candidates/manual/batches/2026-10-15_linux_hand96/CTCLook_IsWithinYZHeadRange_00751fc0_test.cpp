#include <cstdio>
struct CLookDef { float MaxHeadAngleYZUp; float MaxHeadAngleYZDown; };
struct CTCLook {
    const CLookDef* PDef;
    long IsWithinYZHeadRange(float a)
    {
        if (a <= PDef->MaxHeadAngleYZUp * 0.0027777778f && a >= -(PDef->MaxHeadAngleYZDown * 0.0027777778f)) return 1;
        return 0;
    }
};
int main()
{
    CLookDef d; d.MaxHeadAngleYZUp = 90.0f; d.MaxHeadAngleYZDown = 36.0f;   // [-0.1, 0.25] turns
    CTCLook l; l.PDef = &d;
    if (l.IsWithinYZHeadRange(0.0f) != 1 || l.IsWithinYZHeadRange(0.2f) != 1 || l.IsWithinYZHeadRange(-0.09f) != 1) { std::printf("00751fc0_TEST FAIL\n"); return 1; }
    if (l.IsWithinYZHeadRange(0.3f) != 0 || l.IsWithinYZHeadRange(-0.2f) != 0) { std::printf("00751fc0_TEST FAIL\n"); return 1; }
    std::printf("00751fc0_TEST PASS\n");
    return 0;
}
