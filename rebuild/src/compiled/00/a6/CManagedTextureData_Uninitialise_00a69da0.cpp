// CManagedTextureData::Uninitialise() -- release the counted CTexture, clear the D3D header, data block and size, then tail-call CTexture::Uninitialise through the (now null) texture pointer
// Leaf: ?Uninitialise@CManagedTextureData@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69DA0, 65 bytes. Name: Ego_r.pdb ?Uninitialise@CManagedTextureData@@QAEXXZ; the
//   catalog label agrees.
// Layout: Ego_r.pdb CManagedTextureData, local model (engine/CManagedTextureData.h flattens the counted pointers) (Texture = CCountedPointer<CTexture> {Data +0x20, Info +0x24}, D3DHeader +0x34,
//   Data +0x3C, AllocSize +0x40). Callees: CCPPointerInfo::DeleteFunc (__fastcall through +4), operator delete 0x00BFE9BC,
//   CTexture::Uninitialise 0x009F9F70 (landed; tail jump).
// Shape: CCountedPointer::ReleaseData inline (Info re-read after the decrement, Info cleared before Data); the texture
//   pointer re-read from the member AFTER it was cleared: retail calls Uninitialise on a null CTexture (a retail bug; the
//   callee's null test makes it harmless).
void operator delete(void* p);                          // 0x00BFE9BC
class CCPPointerInfo {
public:
    long RefCount;                                      // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};
class CTexture {
public:
    void Uninitialise();                                // 0x009F9F70
};
class CVolumeTexture;
class CManagedTextureOwner;
struct IDirect3DTexture9;
struct IDirect3DVolumeTexture9;
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    T* operator->() const { return Data; }
    void ReleaseData()
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
};
template <class T> class scoped_ptr { public: T* ptr; };

class CManagedTextureData {                             // Ego_r.pdb, 0x4C bytes
public:
    virtual ~CManagedTextureData();
    unsigned char                   _pad04[0x18];       // +0x04
    unsigned short                  Width;              // +0x1C
    unsigned short                  Height;             // +0x1E
    CCountedPointer<CTexture>       Texture;            // +0x20
    CCountedPointer<CVolumeTexture> VolumeTexture;      // +0x28
    CManagedTextureOwner*           Owner;              // +0x30
    IDirect3DTexture9*              D3DHeader;          // +0x34
    scoped_ptr<IDirect3DVolumeTexture9> D3DVolumeHeader; // +0x38
    void*                           Data;               // +0x3C
    unsigned long                   AllocSize;          // +0x40
    unsigned long                   PoolIndex;          // +0x44
    bool                            Allocated;          // +0x48
    bool                            Locked;             // +0x49
    bool                            BusyFlag;           // +0x4A
    void Uninitialise();
};

void CManagedTextureData::Uninitialise()
{
    Texture.ReleaseData();
    D3DHeader = 0;
    Data = 0;
    AllocSize = 0;
    Texture->Uninitialise();
}