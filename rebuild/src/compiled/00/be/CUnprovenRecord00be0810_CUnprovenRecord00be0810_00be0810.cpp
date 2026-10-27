// CUnprovenRecord_00be0810::~CUnprovenRecord_00be0810() -- implicit destructor: release the counted pointer member (+0xC) -- drop the shared count, on zero run the deleter and free the count block, then clear it
// Leaf: ??1CUnprovenRecord_00be0810@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00be0810, 49 bytes, thiscall, plain `ret`.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT, next to CEngineStaticMapFileBlockManager's tree helpers): the implicit destructor of a record whose only non-trivial member is a CCountedPointer at +0x0C; types unproven.
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L028): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
//   delete), then Info and Data cleared in that order (ReleaseData) or Data then Info (the destructor).
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
class CCPPointerInfo {
public:
    long RefCount;                                        // +0x00
    void (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                           // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes: Data, Info
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    void ReleaseData()                                    // Ego_r.pdb ?ReleaseData@?$CCountedPointer@..@@QAEXXZ (inline here)
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    ~CCountedPointer()                                    // the destructor clears Data before Info
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CUnprovenPointee_00be0810;                                               // descriptive: the pointee is unproven
class CUnprovenRecord_00be0810 {                                              // descriptive: the record type is unproven
public:
    unsigned char Head[0xC];                              // +0x00 (trivially destroyed)
    CCountedPointer<CUnprovenPointee_00be0810> Pointer;                        // +0x0C
    ~CUnprovenRecord_00be0810();
};
CUnprovenRecord_00be0810::~CUnprovenRecord_00be0810()
{
}