// Behaviour test for 00a42a80 CLipSyncDataBankEntry::CData::UnloadResource (copy of the model; the owner's
// pointee is a counting stand-in).
#include <cstdio>

static int g_deletes = 0;
struct CIVCountedPointeeBase {
    virtual void Destroy(int) {}
    virtual void Delete() { ++g_deletes; }
    unsigned long IVRefCount;
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
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    bool IsValid() const { return Data != 0; }
    long GetRefCount() const { return Info ? Info->RefCount : 0; }
};
struct CLipSyncSampleData { int x; };
struct CBankFileAsyncData;
struct CLipSyncDataBank;
struct CLipSyncDataBankEntry { CIVCountedPointer Data; };
struct CData {
    unsigned char                       _resource[0x1c];
    CCountedPointer<CLipSyncSampleData> LipSyncData;
    CCountedPointer<CBankFileAsyncData> FileData;
    CLipSyncDataBankEntry*              Owner;
    CLipSyncDataBank*                   Bank;
    bool UnloadResource();
};
bool CData::UnloadResource()
{
    if (LipSyncData.IsValid() && LipSyncData.GetRefCount() == 1) {
        Owner->Data = 0;
        return true;
    }
    return false;
}

int main()
{
    CIVCountedPointeeBase pointee; pointee.IVRefCount = 1;
    CLipSyncDataBankEntry entry; entry.Data.Object = &pointee;
    CLipSyncSampleData sample; CCPPointerInfo info; info.RefCount = 2;
    CData d; d.Owner = &entry; d.LipSyncData.Data = 0; d.LipSyncData.Info = &info;
    int ok = 1;
    if (d.UnloadResource() || entry.Data.Object != &pointee) ok = 0;          // no data
    d.LipSyncData.Data = &sample; d.LipSyncData.Info = 0;
    if (d.UnloadResource()) ok = 0;                                         // no info block
    d.LipSyncData.Info = &info;
    if (d.UnloadResource() || g_deletes != 0) ok = 0;                       // shared (count 2)
    info.RefCount = 1;
    if (!d.UnloadResource() || entry.Data.Object != 0 || g_deletes != 1 || pointee.IVRefCount != 0) ok = 0;
    if (!d.UnloadResource() || g_deletes != 1) ok = 0;                      // owner already empty: still true
    if (!ok) { std::printf("00a42a80_TEST FAIL\n"); return 1; }
    std::printf("00a42a80_TEST PASS\n");
    return 0;
}