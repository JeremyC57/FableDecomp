// CLipSyncDataBank::CLipSyncDataBank() -- CBankFile base, empty Entries, null resource bank / phoneme bank, SkipUpdate false, clear the static-map flag, then ResourceBank takes a new CResourceBank
// Leaf: ??0CLipSyncDataBank
// Retail: Fable.exe 0x00A42B80, 118 bytes (plain `ret`). Vtable 0x0129D3D4 = CLipSyncDataBank (rtti-vtables); only
// caller 0x00991B17.
// Label correction: the catalog "CTCSkeletalMorph::CTCSkeletalMorph" is propagated; the vtable store names the class
// (audio/lip-sync layer: record for the later replacement work).
// Callees: CBankFile::CBankFile() @ 0x009D5F80, CBankFile::SetIsStaticMap(bool) @ 0x009D5230 (empty `ret 4`, called
// with false; as the landed CStaticMapBankFile ctor 0x00B3ECC0), operator new @ 0x00BFEA1A (0x7C bytes),
// CResourceBank::CResourceBank() @ 0x009FC5F0, CCountedPointer<CResourceBank>::Reset(CResourceBank*) @ 0x00992170
// (manifest "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset", byte twin).
// Layout: retail CBankFile is 0x164 bytes (PDB 0x174, as the landed CParticleDataBank ctor 0x00AF1670); retail
// CLipSyncDataBank is 0x180: Entries (CArray<CLipSyncDataBankEntry>, 12 bytes) +0x164, ResourceBank
// (CCountedPointer<CResourceBank>) +0x170, SkipUpdate +0x178, PhonemeBank (CPhonemeDataBank*) +0x17C. The PDB's
// CCountedPointer<CSpeechAnalyser> SpeechAnalyser (PDB +0x1F0) is absent from the retail object.
// Shape: one shared zero register (ebx); the setter's false is that register; the new-expression's null test feeds Reset.
#include <stddef.h>
void* __cdecl operator new(size_t size);            // 0x00BFEA1A
struct CBankFile {
    virtual void v00();
    unsigned char _bankFile[0x160];                 // +0x004
    CBankFile();                                    // 0x009D5F80
    void SetIsStaticMap(bool isStaticMap);          // 0x009D5230 (empty body: `ret 4`)
};
struct CResourceBank {
    unsigned char _bank[0x7c];
    CResourceBank();                                // 0x009FC5F0
};
template <class T> struct _STLP_alloc_proxy {
    T* _M_data;
    _STLP_alloc_proxy(T* p) : _M_data(p) { }
};
template <class T> struct CArray {                  // retail 12 bytes: STLport _Vector_base layout
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T* Data;                                        // +0x00
    CCPPointerInfo* Info;                           // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    void Reset(T* item);                            // 0x00992170
};
struct CLipSyncDataBankEntry;
struct CPhonemeDataBank;
struct CLipSyncDataBank : CBankFile {               // vtable 0x0129D3D4
    CArray<CLipSyncDataBankEntry>  Entries;         // +0x164
    CCountedPointer<CResourceBank> ResourceBank;    // +0x170
    bool                           SkipUpdate;      // +0x178
    CPhonemeDataBank*              PhonemeBank;     // +0x17c
    virtual void v00();
    CLipSyncDataBank();
};

CLipSyncDataBank::CLipSyncDataBank()
    : SkipUpdate(false)
    , PhonemeBank(0)
{
    SetIsStaticMap(false);
    ResourceBank.Reset(new CResourceBank);
}
