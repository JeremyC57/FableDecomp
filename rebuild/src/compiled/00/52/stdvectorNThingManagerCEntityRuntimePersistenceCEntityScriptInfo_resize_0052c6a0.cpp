// std::vector<NThingManager::CEntityRuntimePersistence::CEntityScriptInfo>::resize(size_type n) -- the one-argument STLport resize: grow or shrink the entity-script table to n records, new ones default-constructed (a temporary passed to resize(n, value))
// Leaf: ?resize@?$vector@VCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@V?$allocator@VCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052C6A0, length 88 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0 and ~CCharString @ 0x0099EAE0 (the temporary's Script),
// vector::resize(size_type, const T&) @ 0x0052C630 (24-byte elements: the 0x2AAAAAAB / `sar edx,2` divide; manifest
// "vector<CTCInventoryMap::CUIMiniMapMarker>::resize" is propagated), free @ 0x00BFEA14 (the temporary's SaveData storage).
// Identity: Ego_r.pdb ?resize@?$vector@VCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@...@std@@QAEXI@Z; the
// five callers are the landed NThingManager::CEntityRuntimePersistence::SetLevelPersistentScript 0x00523790,
// SetGamePersistentScript 0x005237F0, SetGamePersistentSaveData 0x00523860, SetGamePersistentSaveDataCompressed
// 0x00523910 and CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps (0x00523F70), which resize the entity-script
// tables before indexing them. The catalog's "CThingDeadCreature::WorldUpdateBulletTime" and the earlier descriptive
// CMarkerRecordVector name were propagated / descriptive labels.
// Layout: Ego_r.pdb CEntityScriptInfo {CCharString Script +0; CArray<unsigned char> SaveData +4; long UncompressedSize;
// bool Valid} is 28 bytes with the Dinkumware 16-byte vector; retail has STLport's 12-byte one, so UncompressedSize +0x10,
// Valid +0x14, 0x18 bytes (local model, built in the frame).
// Shape: Script is constructed first (out of line); the zero stores of SaveData, UncompressedSize and Valid are scheduled
// after the argument pushes (ebx = 0); after the call the temporary dies in reverse member order: SaveData frees its
// storage when non-null (`push eax ; call free ; add esp,4`), then Script's destructor.
#include <stdlib.h>

struct CCharString {
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
};

template <class T> struct CArray {              // std::vector<T> view with the malloc-based allocator
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CArray()
    {
        if (_M_start)
            free(_M_start);
    }
};

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {                   // 0x18 bytes in retail
    public:
        CCharString           Script;           // +0x00
        CArray<unsigned char> SaveData;         // +0x04
        long                  UncompressedSize; // +0x10
        bool                  Valid;            // +0x14
        CEntityScriptInfo() : UncompressedSize(0), Valid(false) { }
    };
};
}

namespace std {
template <class _Tp> class allocator;

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x0052C630 for CEntityScriptInfo
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<NThingManager::CEntityRuntimePersistence::CEntityScriptInfo>::resize(unsigned int);
}
