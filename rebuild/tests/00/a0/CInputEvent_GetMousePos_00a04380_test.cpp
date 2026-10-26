// Test for 0x00A04380 CInputEvent::GetMousePos() const: returns the C2DVector at +0x18 by value.
#include <stdio.h>
#include <string.h>
class C2DVector { public: float X, Y; C2DVector() { } C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { } };
class CInputEvent {
public:
    unsigned char _data[0x18];
    C2DVector     MousePos;
    long          DeviceType, DeviceNumber, Type;
    float         CurrentTime, StartTime;
    C2DVector GetMousePos() const;
};
int main()
{
    CInputEvent e; memset(&e, 0, sizeof e);
    if (sizeof(CInputEvent) != 0x34 || (char*)&e.MousePos - (char*)&e != 0x18) { printf("00a04380_TEST FAIL layout\n"); return 1; }
    e.MousePos.X = 12.5f; e.MousePos.Y = -3.0f; e._data[0] = 7;
    const CInputEvent& r = e;
    C2DVector p = r.GetMousePos();
    if (p.X != 12.5f || p.Y != -3.0f) { printf("00a04380_TEST FAIL value\n"); return 1; }
    e.MousePos.Y = 640.0f;
    if (r.GetMousePos().Y != 640.0f) { printf("00a04380_TEST FAIL reread\n"); return 1; }
    printf("00a04380_TEST PASS\n");
    return 0;
}
