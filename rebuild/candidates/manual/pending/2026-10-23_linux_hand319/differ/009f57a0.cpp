// Grade: DIFFER(88v1072) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CInputManager::Update() -- per-frame input: while replaying a recorded package, apply the next package's events and
//   checksum; otherwise update the devices, clear both event containers, scan the mouse / joystick / per-device scanners
//   into the event vector (STLport push_back of 0x34-byte CInputEvent), then turn the inkey buffer into keyboard
//   held events (GFGetTime timestamps) and finally save the event package
// Leaf: ?Update@CInputManager@@
// Stub: first-pass skeleton only (the replay branch); the scanner loops, the inline CInputEvent copy and the inkey loop are
//   not reconstructed yet.
// Callees: CInputEventPackage ctor 0x00A04410 / dtor 0x00A03B30, SetEventsFromEventPackage 0x009F54D0,
//   CalculateInternalChecksum 0x009F5170.
struct CInputEventPackage {
    unsigned char _body[0x30];
    CInputEventPackage();                               // 0x00A04410
    ~CInputEventPackage();                              // 0x00A03B30
};
struct CRecordedPackage { unsigned char _pad[0xc]; long FrameNumber; unsigned char _pad10[0x20]; };   // 0x30 bytes
class CInputManager {
public:
    virtual void Update();
    unsigned char      _pad04[0x34];
    bool               Replaying;                       // +0x38
    unsigned char      _pad39[0x0f];
    long               Checksum;                        // +0x48
    CRecordedPackage*  Packages;                        // +0x4c
    unsigned char      _pad50[0x08];
    long               PackageIndex;                    // +0x58
    long               CurrentFrame;                    // +0x5c
    unsigned char      _pad60[0x1c];
    long               FrameCount;                      // +0x7c
    void SetEventsFromEventPackage(CRecordedPackage* package);   // 0x009F54D0
    long CalculateInternalChecksum();                   // 0x009F5170
};
void CInputManager::Update()
{
    CInputEventPackage package;
    FrameCount++;
    if (Replaying) {
        CRecordedPackage* p = &Packages[PackageIndex];
        if (p->FrameNumber == CurrentFrame) {
            SetEventsFromEventPackage(p);
            Checksum = CalculateInternalChecksum();
            PackageIndex++;
        }
        return;
    }
}
