// CGameEventPackageSet::InitFromCompressedBuffer(const unsigned char* buffer) -- rebuild the package set from its
// Grade: RELOCATION_MATCH with cl 13.10.3077 (RTM); the DDK 13.10.4035 build emits a different /GS prologue for this body.
// compressed form: a package count byte, then per package an event count byte, a 32-bit time stamp and the
// compressed events
// Leaf: ?InitFromCompressedBuffer@CGameEventPackageSet
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F1AC0, length 221 bytes, `ret 4`, /GS cookie frame (0x678 bytes).
// Callees: CGameEvent::InitFromCompressedBuffer(const unsigned char*) @ 0x009F1870 (returns the bytes consumed),
// CGameEventPackage::AddEvent(const CGameEvent*) @ 0x009F1650, __security_check_cookie @ 0x00BFE9F9 (cookie
// 0x0139C8A8).
// Layout (PDB, retail agrees; engine/CGameEventPackageSet.h keeps the packages as opaque bytes, so the types are
// modelled locally): CGameEventPackageSet NoPackages +0, Packages[50] +4 (0x648 each); CGameEventPackage TimeStamp +0,
// NoEvents +4, Events[40] +8; CGameEvent (0x28) Type, Player +4 (-1 when unset), Data[32], EndPos, Valid +0x26,
// Replacement.
// Shape: the counts are unsigned char locals (`mov al ; movzx`) compared as long; the event and the package are locals with inline ctors (the package's event array is an inline counted
// loop); Clear()/SetTimeStamp() are inline; the package is copied whole (`rep movsd` x 0x192) into the next slot and
// NoPackages re-read for the increment.

class CGameEvent {
public:
    long Type;
    signed char Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool Valid;
    bool Replacement;
    CGameEvent() : Player(-1), Valid(false) {}
    long InitFromCompressedBuffer(const unsigned char* buffer);     // 0x009F1870
};

class CGameEventPackage {
public:
    long TimeStamp;
    unsigned long NoEvents;
    CGameEvent Events[40];
    CGameEventPackage() : NoEvents(0) {}
    void Clear() { NoEvents = 0; }
    void SetTimeStamp(long timeStamp) { TimeStamp = timeStamp; }
    void AddEvent(const CGameEvent* event);                         // 0x009F1650
};

class CGameEventPackageSet {
public:
    unsigned long NoPackages;
    CGameEventPackage Packages[50];
    void InitFromCompressedBuffer(const unsigned char* buffer);
};

void CGameEventPackageSet::InitFromCompressedBuffer(const unsigned char* buffer)
{
    CGameEvent event;
    CGameEventPackage package;
    NoPackages = 0;
    unsigned char noPackages = *buffer++;
    for (long i = 0; i < noPackages; ++i) {
        unsigned char noEvents = *buffer++;
        long timeStamp = *(const long*)buffer;
        buffer += 4;
        package.Clear();
        package.SetTimeStamp(timeStamp);
        for (long j = 0; j < noEvents; ++j) {
            buffer += event.InitFromCompressedBuffer(buffer);
            package.AddEvent(&event);
        }
        Packages[NoPackages] = package;
        ++NoPackages;
    }
}