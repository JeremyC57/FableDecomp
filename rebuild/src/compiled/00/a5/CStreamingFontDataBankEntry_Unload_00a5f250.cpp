// CStreamingFontDataBankEntry::Unload() -- release the Font counted pointer (+0x4): drop the shared count, on zero run the deleter on the data and free the count block, then clear the pointer
// Leaf: ?Unload@CStreamingFontDataBankEntry@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a5f250, 49 bytes, thiscall, plain `ret`.
// Identity (proven): vtable 0x0129D5B4 (RTTI CStreamingFontDataBankEntry) slot 3; Ego_r.pdb ?Unload@CStreamingFontDataBankEntry@@UAEXXZ and CStreamingFontDataBankEntry::Font (CCountedPointer<CFontBank>, +4).
// Callees: the CCPPointerInfo deleter (fastcall, Data in ecx), operator delete 0x00BFE9BC.
// Shape (family L108): CCountedPointer's release with `--Info->RefCount` (Info reloaded after the decrement and for the
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
class CFontBank;
class CBaseClassNonCopyable {                              // Fable.exe RTTI base
public:
    virtual ~CBaseClassNonCopyable();
};
class CStreamingFontDataBankEntry : public CBaseClassNonCopyable {
public:
    CCountedPointer<CFontBank> Font;                        // +0x4
    virtual void Unload();
};
void CStreamingFontDataBankEntry::Unload()
{
    Font.ReleaseData();
}