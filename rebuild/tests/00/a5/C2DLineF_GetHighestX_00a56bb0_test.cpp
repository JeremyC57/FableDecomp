#include <cstdio>
template <class T> inline const T& Max(const T& left, const T& right) { return left < right ? right : left; }
template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }
class C2DVector { public: float X; float Y; };
class C2DLineF { public:
    C2DVector Point1; C2DVector Point2;
    float GetHighestX() const { return (float)Max<double>(Point1.X, Point2.X); }
};
int main()
{
    C2DLineF a = { { 1.0f, -2.0f }, { 4.0f, 3.5f } };
    C2DLineF b = { { 9.0f, 8.0f }, { -1.0f, 0.25f } };
    if (a.GetHighestX() != 4.0f || b.GetHighestX() != 9.0f) { std::printf("00a56bb0_TEST FAIL\n"); return 1; }
    std::printf("00a56bb0_TEST PASS\n");
    return 0;
}