// CASuspendableProcess::CASuspendableProcess(bool, long, long) -- base CBase(), Running/StopRunning cleared, TerminateUsingExceptions and TimesliceLength from the arguments, own vtable (0x0129D428)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A445D0, length 48 bytes.
// Callees: CBase::CBase @ 0x0099A2F0
// Label: the manifest says "CCreatureAction_CastBattleCharge" (propagated); the stored vtable 0x0129D428 is
// CASuspendableProcess's (rtti-vtables.tsv). Layout and names from engine/CASuspendableProcess.h (sizeof 0x10, agrees
// with the bytes) but modelled locally because the header is a flat record and this body needs the polymorphic CBase
// base whose constructor is called. The second argument is never read (a `ret 0Ch` with only two stores); the
// timeslice (PDB: float) is copied with an integer move, so the parameter and member are modelled as dwords.
// TimeProcessSliceStarted (+0x08) is left uninitialised.
struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2F0
};

struct CASuspendableProcess : CBase {       // vtable 0x0129D428; sizeof 0x10
    unsigned char Running;                  // +0x04 (bool)
    unsigned char StopRunning;              // +0x05 (bool)
    unsigned char TerminateUsingExceptions; // +0x06 (bool)
    long          TimeProcessSliceStarted;  // +0x08 (float in the PDB; untouched here)
    long          TimesliceLength;          // +0x0c (float in the PDB; copied as a dword)
    virtual void v00();
    CASuspendableProcess(bool terminateUsingExceptions, long unused, long timesliceLength);
};

CASuspendableProcess::CASuspendableProcess(bool terminateUsingExceptions, long unused, long timesliceLength)
    : CBase()
    , Running(0)
    , StopRunning(0)
    , TerminateUsingExceptions(terminateUsingExceptions)
    , TimesliceLength(timesliceLength)
{
}