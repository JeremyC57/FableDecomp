// CLipSyncDataBankEntry::CData::UnloadResource() -- when the lip-sync sample data is held only by this entry, drop the owner's counted pointer to this data (which deletes it) and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42A80, length 60 bytes (64 with int3 padding).
// Leaf: UnloadResource
// Callees: CIVCountedPointeeBase virtual slot 1 (Delete, `call [eax+4]`) when the owner's count reaches zero.
// Name: vtable 0x0129D3A4 slot 2 (slots 4/5 are IsResourceLoaded 0x00A42810 and OnResourceLoaded 0x00A42FB0);
// FableWin.pdb lists the CData overrides UnloadResource (UAE_NXZ, bool), IsResourceLoaded (const) and
// OnResourceLoaded, and this is the bool-returning, argument-less one.
// Layout: Ego_r.pdb CLipSyncDataBankEntry::CData (0x34): LipSyncData (CCountedPointer<CLipSyncSampleData>: data
// +0x1c, pointer info +0x20 whose first word is the reference count), FileData +0x24, Owner (CLipSyncDataBankEntry*)
// +0x2c; the entry's first member is its CIVCountedPointer<CData>.
// Shape: three tests to a shared `xor al,al` tail through inline accessors -- `IsValid()` and a value-returning
// `GetRefCount()` (`Info ? Info->RefCount : 0`) give retail's `mov eax,[eax] ; cmp eax,1` (a direct
// `Info->RefCount == 1` folds into `cmp dword ptr [eax],1`); the inlined
// CIVCountedPointer::operator=(0) as in the landed CFrameData::OnResourceLoaded (0x009FF860): the release through
// a local pointee copy with its zero store inside the null test, then the assignment's unconditional zero store.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(CIVCountedPointeeBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
        return *this;
    }
};

class CCPPointerInfo { public:
    long  RefCount;                                // +0
    void (__fastcall* DeleteFunc)(void*);          // +4
    void* Data;                                    // +8
};

template <class T> class CCountedPointer { public:
    T*              Data;                          // +0
    CCPPointerInfo* Info;                          // +4
    bool IsValid() const { return Data != 0; }
    long GetRefCount() const { return Info ? Info->RefCount : 0; }
};

class CLipSyncSampleData;
class CBankFileAsyncData;
class CLipSyncDataBank;

class CLipSyncDataBankEntry { public:
    CIVCountedPointer Data;                        // +0x00 CIVCountedPointer<CData>
};

struct CLipSyncDataBankEntry_CData {
    unsigned char                       _resource[0x1c];   // CResource base (vptr .. LastUsedFrame)
    CCountedPointer<CLipSyncSampleData> LipSyncData;       // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;          // +0x24
    CLipSyncDataBankEntry*              Owner;             // +0x2c
    CLipSyncDataBank*                   Bank;              // +0x30
    bool UnloadResource();
};

bool CLipSyncDataBankEntry_CData::UnloadResource()
{
    if (LipSyncData.IsValid() && LipSyncData.GetRefCount() == 1) {
        Owner->Data = 0;
        return true;
    }
    return false;
}