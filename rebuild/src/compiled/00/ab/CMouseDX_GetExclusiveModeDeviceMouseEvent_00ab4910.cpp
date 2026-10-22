// CMouseDX::GetExclusiveModeDeviceMouseEvent(CMouseEvent* event) -- pop the next buffered DirectInput mouse record (refilling the buffer from the device when it is drained) and translate it into a CMouseEvent
// Leaf: GetExclusiveModeDeviceMouseEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB4910, length 672 bytes (incl. a 20-entry byte index table + 11-entry jump table), `ret 4`.
// Ego_r.pdb: CMouseDX::GetExclusiveModeDeviceMouseEvent(CMouseEvent*) -> bool.
// Callees: GFGetSystemManager @ 0x009A4EC0, CSystemManager::IsApplicationActive @ 0x009A57B0, CSystemManager::HasInputFocus @ 0x009A4FB0
//          (labelled "CTCInventoryBase::GetCanItemBeQuickAssigned", propagated), __ftol2 @ 0x00BFEA70,
//          IDirectInputDevice8::Acquire (COM slot 7) / GetDeviceData (COM slot 10), CMouseDX::SetHasInputFocus (slot 14, +0x38).
// Globals: 4294967296.0f @ 0x0122DCB4 (unsigned fild fix-up), 0.001f @ 0x01266300.
// Layout: CMouseDX retail sits 8 bytes below Ego_r.pdb (as in the landed UpdateCurrentStatusFromEvent 0x00AB5190): DMouse +0x341c,
//   Acquired +0x3424, MouseStep +0x3428, ExclusiveModeData (DIDEVICEOBJECTDATA[256], 20 bytes each) +0x3440,
//   ExclusiveModeEventsCount +0x4840, NoExclusiveModeEvents +0x4844. CMouseEvent (PDB): Time +0, Type +8, Movement +0xc.
// Shape: the head is the inlined CMouseDX::UpdateAcquired (landed @ 0x00AB4830); `cmp byte ptr [Acquired],1` is `Acquired == true`;
//   DIERR_INPUTLOST (0x8007001E) re-acquires and re-reads once. The DIMOFS_* switch is a byte index table; buttons map
//   (dwData & 0x80) to the down/up event types; the timestamp is an unsigned DWORD scaled by 0.001f into the double Time.
struct CSystemManager {
    bool IsApplicationActive();                    // 0x009A57B0
    bool HasInputFocus();                          // 0x009A4FB0
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

struct DIDEVICEOBJECTDATA {
    unsigned long dwOfs;
    unsigned long dwData;
    unsigned long dwTimeStamp;
    unsigned long dwSequence;
    unsigned long uAppData;
};

struct IDirectInputDevice8W {
    virtual long __stdcall QueryInterface(const void* riid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetCapabilities(void* caps);
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);
    virtual long __stdcall GetProperty(const void* guid, void* header);
    virtual long __stdcall SetProperty(const void* guid, const void* header);
    virtual long __stdcall Acquire();
    virtual long __stdcall Unacquire();
    virtual long __stdcall GetDeviceState(unsigned long size, void* data);
    virtual long __stdcall GetDeviceData(unsigned long objectSize, DIDEVICEOBJECTDATA* data, unsigned long* inOut, unsigned long flags);
};

enum EMouseEventType {
    MOUSE_EVENT_LMB_DOWN = 1,
    MOUSE_EVENT_RMB_DOWN = 2,
    MOUSE_EVENT_MMB_DOWN = 3,
    MOUSE_EVENT_LMB_UP = 4,
    MOUSE_EVENT_RMB_UP = 5,
    MOUSE_EVENT_MMB_UP = 6,
    MOUSE_EVENT_MOVE_X = 7,
    MOUSE_EVENT_MOVE_Y = 8,
    MOUSE_EVENT_WHEEL_UP = 9,
    MOUSE_EVENT_WHEEL_DOWN = 10,
    MOUSE_EVENT_SET_POSITION = 11,
    MOUSE_EVENT_BUTTON3_DOWN = 15,
    MOUSE_EVENT_BUTTON4_DOWN = 16,
    MOUSE_EVENT_BUTTON5_DOWN = 17,
    MOUSE_EVENT_BUTTON6_DOWN = 18,
    MOUSE_EVENT_BUTTON7_DOWN = 19,
    MOUSE_EVENT_BUTTON3_UP = 20,
    MOUSE_EVENT_BUTTON4_UP = 21,
    MOUSE_EVENT_BUTTON5_UP = 22,
    MOUSE_EVENT_BUTTON6_UP = 23,
    MOUSE_EVENT_BUTTON7_UP = 24
};

struct CMouseEvent {
    double          Time;                           // +0x00
    EMouseEventType Type;                           // +0x08
    float           Movement;                       // +0x0c
    float           PosX;                           // +0x10
    float           PosY;                           // +0x14
};

enum {
    DIMOFS_X = 0, DIMOFS_Y = 4, DIMOFS_Z = 8,
    DIMOFS_BUTTON0 = 12, DIMOFS_BUTTON1, DIMOFS_BUTTON2, DIMOFS_BUTTON3,
    DIMOFS_BUTTON4, DIMOFS_BUTTON5, DIMOFS_BUTTON6, DIMOFS_BUTTON7
};

class CMouseDX {
public:
    virtual ~CMouseDX();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void SetHasInputFocus(bool focus);      // slot 14 (+0x38)

    unsigned char         _pad04[0x3418];
    IDirectInputDevice8W* DMouse;                   // +0x341c
    unsigned char         _pad3420[4];
    bool                  Acquired;                 // +0x3424
    float                 MouseStep;                // +0x3428
    unsigned char         _pad342c[0x14];
    DIDEVICEOBJECTDATA    ExclusiveModeData[256];   // +0x3440
    long                  ExclusiveModeEventsCount; // +0x4840
    long                  NoExclusiveModeEvents;    // +0x4844

    void UpdateAcquired()
    {
        if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->HasInputFocus())
            SetHasInputFocus(true);
        else
            SetHasInputFocus(false);
    }
    bool GetExclusiveModeDeviceMouseEvent(CMouseEvent* event);
};

bool CMouseDX::GetExclusiveModeDeviceMouseEvent(CMouseEvent* event)
{
    if (ExclusiveModeEventsCount >= NoExclusiveModeEvents) {
        UpdateAcquired();
        if (Acquired == true && DMouse) {
            unsigned long count = 256;
            long hr = DMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), ExclusiveModeData, &count, 0);
            if (hr == (long)0x8007001E) {
                DMouse->Acquire();
                hr = DMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), ExclusiveModeData, &count, 0);
            }
            if (hr >= 0) {
                ExclusiveModeEventsCount = 0;
                NoExclusiveModeEvents = count;
            }
        }
    }
    if (ExclusiveModeEventsCount < NoExclusiveModeEvents) {
        DIDEVICEOBJECTDATA* data = &ExclusiveModeData[ExclusiveModeEventsCount++];
        switch (data->dwOfs) {
        case DIMOFS_X:
            event->Type = MOUSE_EVENT_MOVE_X;
            event->Movement = (float)(long)((long)data->dwData * MouseStep);
            break;
        case DIMOFS_Y:
            event->Type = MOUSE_EVENT_MOVE_Y;
            event->Movement = (float)(long)((long)data->dwData * MouseStep);
            break;
        case DIMOFS_Z:
            event->Type = MOUSE_EVENT_WHEEL_DOWN;
            event->Movement = (float)(long)data->dwData;
            break;
        case DIMOFS_BUTTON0:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_LMB_DOWN : MOUSE_EVENT_LMB_UP;
            break;
        case DIMOFS_BUTTON1:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_RMB_DOWN : MOUSE_EVENT_RMB_UP;
            break;
        case DIMOFS_BUTTON2:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_MMB_DOWN : MOUSE_EVENT_MMB_UP;
            break;
        case DIMOFS_BUTTON3:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_BUTTON3_DOWN : MOUSE_EVENT_BUTTON3_UP;
            break;
        case DIMOFS_BUTTON4:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_BUTTON4_DOWN : MOUSE_EVENT_BUTTON4_UP;
            break;
        case DIMOFS_BUTTON5:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_BUTTON5_DOWN : MOUSE_EVENT_BUTTON5_UP;
            break;
        case DIMOFS_BUTTON6:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_BUTTON6_DOWN : MOUSE_EVENT_BUTTON6_UP;
            break;
        case DIMOFS_BUTTON7:
            event->Type = ((unsigned char)data->dwData & 0x80) ? MOUSE_EVENT_BUTTON7_DOWN : MOUSE_EVENT_BUTTON7_UP;
            break;
        default:
            return false;
        }
        event->Time = data->dwTimeStamp * 0.001f;
        return true;
    }
    return false;
}