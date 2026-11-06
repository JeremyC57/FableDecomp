// CJoystickDX::UpdateEvents() -- poll the DirectInput joystick, turn each buffered axis/button record into joystick events
// Leaf: ?UpdateEvents@CJoystickDX@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6E40, length 2568 bytes (2436 bytes of code, then the 18-entry dword jump
// table at 0x00AB77C4 and the 60-byte index table at 0x00AB780C).
// Shape: ebp frame aligned to 8, `ret`. Two-level switch on DIDEVICEOBJECTDATA::dwOfs (`cmp eax,3Bh; ja`): index table bytes
// 0x00 X -> 0, 0x04 Y -> 1, 0x08 Z -> 2, 0x0C RX and 0x10 RY -> 3 (an explicit empty case, its own entry at the refetch),
// 0x14 RZ -> 4, 0x30..0x3B buttons 0..11 -> 5..16, every other offset -> 17 (default, the same refetch address).
// The button arms all reach one shared AddEventToStore/ProcessMaintainedEvents block (0x00AB76A1), so the event is added once
// after the switch under a flag; the flag store in the RX/RY case is what keeps their table entry apart from the default.
// UpdateAcquired (0x00AB6D60) is expanded in place; Poll and GetDeviceData retry once after DIERR_INPUTLOST (0x8007001E)
// and accept DI_OK / 1 (DI_NOEFFECT, DI_BUFFEROVERFLOW).
#define FABLE_RETAIL_CJOYSTICKDX
#define FABLE_RETAIL_CJOYSTICKDX_UPDATEEVENTS
#define FABLE_RETAIL_CINPUTEVENT_JOYSTICK_SETTERS
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#include "engine/CInputEvent.h"
#include "engine/CJoystickDX.h"

static const long DIERR_INPUTLOST = 0x8007001E;
static const long DI_OK = 0;
static const long DI_NOEFFECT = 1;
static const long DI_BUFFEROVERFLOW = 1;

bool CJoystickDX::UpdateEvents()
{
    unsigned long count = 1;
    CInputEvent event;
    if (DJoystick != 0)
    {
        EmptyEventStore();
        UpdateAcquired();
        long result = DJoystick->Poll();
        if (result == DIERR_INPUTLOST)
        {
            DJoystick->Acquire();
            result = DJoystick->Poll();
        }
        if (result != DI_OK && result != DI_NOEFFECT)
            return false;
        DIDEVICEOBJECTDATA data;
        count = 1;
        result = DJoystick->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &count, 0);
        if (result == DIERR_INPUTLOST)
        {
            DJoystick->Acquire();
            result = DJoystick->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &count, 0);
        }
        if (result != DI_OK && result != DI_BUFFEROVERFLOW)
            return false;
        while (count > 0)
        {
            bool addEvent = false;
            switch (data.dwOfs)
            {
            case DIJOFS_X:  XAxis = ((float)(long)data.dwData * 2.0f) / 65536.0f - 1.0f; break;
            case DIJOFS_Y:  YAxis = ((float)(long)data.dwData * 2.0f) / 65536.0f - 1.0f; break;
            case DIJOFS_Z:  XAxis2 = ((float)(long)data.dwData * 2.0f) / 65536.0f - 1.0f; break;
            case DIJOFS_RX:
            case DIJOFS_RY:
                addEvent = false;
                break;
            case DIJOFS_RZ: YAxis2 = ((float)(long)data.dwData * 2.0f) / 65536.0f - 1.0f; break;
            case DIJOFS_BUTTON(0):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(1, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(1, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(1):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(2, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(2, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(2):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(3, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(3, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(3):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(4, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(4, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(4):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(5, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(5, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(5):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(6, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(6, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(6):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(7, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(7, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(7):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(8, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(8, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(8):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(9, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(9, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(9):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(10, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(10, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(10):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(11, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(11, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            case DIJOFS_BUTTON(11):
                if (data.dwData & 0x80)
                    event.SetAsJoystickButtonPressed(12, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                else
                    event.SetAsJoystickButtonReleased(12, 0.0f, C2DVector(XAxis, YAxis), data.dwTimeStamp);
                addEvent = true;
                break;
            }
            if (addEvent)
            {
                AddEventToStore(&event);
                ProcessMaintainedEvents(&event);
            }
            count = 1;
            result = DJoystick->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &count, 0);
            if (result == DIERR_INPUTLOST)
            {
                DJoystick->Acquire();
                result = DJoystick->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &data, &count, 0);
            }
            if (result != DI_OK && result != DI_BUFFEROVERFLOW)
                return false;
        }
        event.SetAsJoystickPosition(C2DVector(XAxis, YAxis), 0.0);
        AddEventToStore(&event);
        event.SetAsJoystickPosition2(C2DVector(XAxis2, YAxis2), 0.0);
        AddEventToStore(&event);
        UpdateMaintainedEventPositions();
        AddMaintainedEventsToEventStore();
    }
    return true;
}