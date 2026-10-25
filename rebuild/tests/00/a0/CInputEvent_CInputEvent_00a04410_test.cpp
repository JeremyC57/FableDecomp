// Link test for 0x00A04410: the source object's CInputEvent::CInputEvent zeroes Data, DeviceNumber and Type only.
#include <cstdio>
#include <string.h>
#include <new>
enum EInputDeviceType { };
enum EInputEventType { };
union UInputEventData { unsigned char Raw[0x20]; };
class CInputEvent {
public:
    UInputEventData  Data;
    EInputDeviceType DeviceType;
    long             DeviceNumber;
    EInputEventType  Type;
    float            CurrentTime;
    float            StartTime;
    CInputEvent();                                 // from the source object
};
int main()
{
    unsigned char raw[sizeof(CInputEvent)];
    memset(raw, 0xee, sizeof(raw));
    CInputEvent* e = new (raw) CInputEvent;
    int ok = sizeof(CInputEvent) == 0x34 && e->DeviceNumber == 0 && (int)e->Type == 0;
    for (int i = 0; i < 0x20; ++i) ok = ok && e->Data.Raw[i] == 0;
    for (int j = 0x20; j < 0x24; ++j) ok = ok && raw[j] == 0xee;
    for (int k = 0x2c; k < 0x34; ++k) ok = ok && raw[k] == 0xee;
    if (!ok) { std::printf("00a04410_TEST FAIL\n"); return 1; }
    std::printf("00a04410_TEST PASS\n");
    return 0;
}
