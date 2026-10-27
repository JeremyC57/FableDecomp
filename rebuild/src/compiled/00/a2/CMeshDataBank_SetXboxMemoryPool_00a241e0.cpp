// CMeshDataBank::SetXboxMemoryPool(CCountedPointer<CXboxBufferMemoryPool> pool, long unused) -- adopt the counted pointer into XboxMemoryPool (the parameter copy is released on return)
// Leaf: ?SetXboxMemoryPool@CMeshDataBank@@QAEXV?$CCountedPointer@VCXboxBufferMemoryPool@@@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a241e0, 106 bytes, thiscall, `ret 0Ch` (the 8-byte counted pointer by value).
// Identity (descriptive): the fourth of the four adjacent CMeshDataBank pool setters (0x00A24050 / 0x00A240E0 = SetClothMemoryPool (Ego_r.pdb) / 0x00A24150 / 0x00A241E0), storing Ego_r.pdb CMeshDataBank::XboxMemoryPool (CCountedPointer<CXboxBufferMemoryPool>, retail +0x378); no caller (the Xbox pool is unused on PC); it pops a second, unread 4-byte argument (`ret 0Ch`); the method name and that parameter are descriptive (only SetClothMemoryPool is in the PDB name sets).
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb XboxMemoryPool at +0x378.
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
class CXboxBufferMemoryPool;
class CMeshDataBank {
public:
    unsigned char _pad00[0x378];
    CCountedPointer<CXboxBufferMemoryPool> XboxMemoryPool;                          // +0x378
    void SetXboxMemoryPool(CCountedPointer<CXboxBufferMemoryPool> pool, long unused);
};
void CMeshDataBank::SetXboxMemoryPool(CCountedPointer<CXboxBufferMemoryPool> pool, long unused)
{
    XboxMemoryPool = pool;
}