// CInputManager::UpdateChecksum(long timeStamp, const CChecksum& checksum) -- copy the four-word checksum record into Checksum (+0x3c) and store the time stamp (+0x5c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4A90, length 41 bytes (+ int3 padding). No callees.
// Layout from Ego_r.pdb CInputManager (144 bytes there, 0x80 in retail: the four std::vectors are 12 bytes each, not 16):
// ProcessedEvents +4, EventStore +0x10, NoStoredEvents +0x1c, NoEventScansRunning +0x20, PMouse +0x24, PKeyboard +0x28,
// PJoysticks +0x2c, Loading +0x38, Saving +0x39, Checksum +0x3c (CChecksum = four longs), LoadedEventPackages +0x4c,
// CurrentFilePackageIndex +0x58, TimeStamp +0x5c, PackageFile +0x60 (CDiskFileWin32, 0x1c bytes), NoUpdates +0x7c.
// The record is the SECOND argument ([esp+8] before the push, the stamp [esp+8] after it).
// Shape: the 16-byte POD record is copied word by word through esi from `lea eax,[ecx+3Ch]`, then the word store, `ret 8`.
class CChecksum { public:
    long Checksum1;                                 // +0x00
    long Checksum2;                                 // +0x04
    long Checksum3;                                 // +0x08
    long Checksum4;                                 // +0x0c
};

class CInputManager { public:
    virtual void v00();
    unsigned char _pad04[0x38];                     // +0x04 the event vectors, counters, devices, joysticks, flags
    CChecksum     Checksum;                         // +0x3c
    unsigned char _pad4c[0x10];                     // +0x4c LoadedEventPackages, CurrentFilePackageIndex
    long          TimeStamp;                        // +0x5c
    void UpdateChecksum(long timeStamp, const CChecksum& checksum);
};

void CInputManager::UpdateChecksum(long timeStamp, const CChecksum& checksum)
{
    Checksum = checksum;
    TimeStamp = timeStamp;
}