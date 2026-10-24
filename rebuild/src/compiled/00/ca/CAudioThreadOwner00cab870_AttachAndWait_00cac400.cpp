// CAudioThreadOwner_00cab870::AttachAndWait() -- attach a fresh auto-reset event as the owner's wake handle, block until it is signalled and fold the wait outcome into an HRESULT (S_OK, the last-error HRESULT on WAIT_FAILED, E_UNEXPECTED otherwise)
// Leaf: ?AttachAndWait@CAudioThreadOwner_00cab870@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAC400, 149 bytes (+11 int3 padding), plain `ret` (thiscall, no arguments).
// Label: the catalog's "NHitLocations::CPhysicalPrimitiveCylinder_TransferBinaryOutAsync" is an autoname (labels_forge_autoname_t5)
// built on the propagated label of its callee; the callee 0x00CAB870 is landed as CAudioThreadOwner_00cab870::Attach(thread)
// (a statically linked COM-style helper class named by address), and `this` is passed straight to it, so this is a member
// of the same class -- descriptive name, see NOTES label corrections. Bounds are right (149 bytes, three duplicated tails).
// Callees: CreateEventW / WaitForSingleObject / CloseHandle (KERNEL32 IAT 0x0143FDC8 / 0x0143FEAC / 0x0143FE30),
// CAudioThreadOwner_00cab870::Attach @ 0x00CAB870, LastErrorToHResult @ 0x00CB0BC0 (AmGetLastErrorToHResult; tail-jumped
// to when the event cannot be created).
// Shape: a `switch` on the wait result (case 0 laid out last, WAIT_FAILED second, default first); each arm duplicates
// the CloseHandle + `hr >= 0 ? 0 : hr` tail (`setge ; dec ; and`).
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void* attributes, int manualReset, int initialState, const unsigned short* name);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);
long LastErrorToHResult_00cab870();        // 0x00CB0BC0

struct CAudioThreadOwner_00cab870 {
    long Attach(void* thread);             // 0x00CAB870
    long AttachAndWait();
};

long CAudioThreadOwner_00cab870::AttachAndWait()
{
    void* event = CreateEventW(0, 0, 0, 0);
    if (!event)
        return LastErrorToHResult_00cab870();
    long hr = Attach(event);
    if (hr < 0) {
        CloseHandle(event);
        return hr;
    }
    switch (WaitForSingleObject(event, 0xFFFFFFFF)) {
    case 0:
        hr = 0;
        break;
    case 0xFFFFFFFF:
        hr = LastErrorToHResult_00cab870();
        break;
    default:
        hr = 0x8000FFFF;
        break;
    }
    CloseHandle(event);
    return hr >= 0 ? 0 : hr;
}