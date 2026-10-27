// Test for 0x00A03B80 CInputEvent::GetChar: returns the 16-bit character stored at the start of the event data.
#include <stdio.h>
union UInputEventData { unsigned short Char; unsigned char Raw[0x20]; };
class CInputEvent {
public:
    unsigned short GetChar() const { return Data.Char; }
    UInputEventData Data; int DeviceType; int DeviceNumber; int Type; float CurrentTime; float StartTime;
};
int main()
{
    CInputEvent e; e.Data.Char = 0x263A;
    bool ok = e.GetChar() == 0x263A;
    e.Data.Char = 'q'; if (e.GetChar() != 'q') ok = false;
    puts(ok ? "00a03b80_TEST PASS" : "00a03b80_TEST FAIL");
    return ok ? 0 : 1;
}