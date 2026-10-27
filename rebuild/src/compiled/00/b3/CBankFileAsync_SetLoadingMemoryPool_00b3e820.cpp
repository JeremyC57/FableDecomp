// CBankFileAsync::SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool) -- adopt the counted pointer into LoadingMemoryPool (the parameter copy is released on return)
// Leaf: ?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b3e820, 106 bytes, thiscall, `ret 8` (the 8-byte counted pointer by value).
// Identity (proven): Ego_r.pdb ?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z; CEngineResourceManager's constructor 0x00B3CB30 passes its LoadingMemoryPool (Ego_r.pdb, the first pool) to the banks at +0x1C etc., and CEngineWorldMap::OpenStaticMaps 0x00B42750 to its static maps (CStaticMapBankFile : CBankFileAsync); bound in reloc_bindings as the stand-in ?SetMemoryPool@CStaticMapBankFile@@.
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb LoadingMemoryPool at +0x118.
// Header debt: engine/CBankFileAsync.h is a flat layout without methods; modelled locally.
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer& other);
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;                 // the source's fields are read first
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
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
class CMovableResourceMemoryPool;
class CBankFileAsync {
public:
    unsigned char _pad00[0x118];
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;                          // +0x118
    void SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool);
};
void CBankFileAsync::SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool)
{
    LoadingMemoryPool = pool;
}