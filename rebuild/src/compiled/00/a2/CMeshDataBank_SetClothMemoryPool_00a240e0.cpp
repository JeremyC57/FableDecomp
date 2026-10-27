// CMeshDataBank::SetClothMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool) -- adopt the counted pointer into ClothMemoryPool (the parameter copy is released on return)
// Leaf: ?SetClothMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a240e0, 106 bytes, thiscall, `ret 8` (the 8-byte counted pointer by value).
// Identity (proven): Ego_r.pdb ?SetClothMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z; the second of four adjacent setters 0x00A24050 / 0x00A240E0 / 0x00A24150 / 0x00A241E0 over Ego_r.pdb CMeshDataBank AnimationMemoryPool / ClothMemoryPool / MeshDataMemoryPool / XboxMemoryPool (retail +0x360/+0x368/+0x370/+0x378); callers CEngineResourceManager's constructor 0x00B3CB30 passing its ClothMemoryPool.
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb ClothMemoryPool at +0x368.
// Header debt: engine/CMeshDataBank.h is a flat layout without methods; modelled locally.
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
class CMeshDataBank {
public:
    unsigned char _pad00[0x368];
    CCountedPointer<CMovableResourceMemoryPool> ClothMemoryPool;                          // +0x368
    void SetClothMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool);
};
void CMeshDataBank::SetClothMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool)
{
    ClothMemoryPool = pool;
}