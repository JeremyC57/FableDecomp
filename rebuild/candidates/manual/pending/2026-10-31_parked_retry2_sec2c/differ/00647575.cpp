// CDrawProjectileTargetingCentre::Draw() -- HEAD DRAFT (first block of 1343 bytes): reset the primitive handle at +0x1C
// Grade: DIFFER(48v1343) 507 diff lines (skeleton 3v1343/515): head draft of the handle reset/assign through a CBaseClass IV handle (vtable 0x0122F598); the x87 reticle layout not drafted (retry wave 2 sec2c)
// (unique-owner callback slot 3, release) and assign it an empty handle (a CBaseClass-derived IV handle, vtable 0x0122F598)
// whose destructor runs the same unique/release sequence. The x87 targeting-reticle layout (0x00647640..) is not drafted.
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) @ 0x00647575, 1343 bytes; /Os frame `lea ebp,[esp-74h]`.
#pragma optimize("s", on)
#include "engine/CBaseClass.h"
class CIVObject { public: virtual void v00(); virtual void Destroy(); virtual void v02(); virtual void OnUniqueRelease(void* owner);
    long RefCount; };
class CIVHandle : public CBaseClass {
public:
    CIVObject* Data;
    CIVHandle() : Data(0) { }
    long GetRefCount() const { return Data ? Data->RefCount : 0; }
    void Release() { if (Data) { if (--Data->RefCount == 0) Data->Destroy(); Data = 0; } }
    void Reset() { if (GetRefCount() == 1) Data->OnUniqueRelease(this); Release(); Data = 0; }
    CIVHandle& operator=(const CIVHandle& other)
    {
        if (Data != other.Data) {
            Release();
            Data = other.Data;
            if (Data)
                ++Data->RefCount;
        }
        return *this;
    }
    ~CIVHandle() { Reset(); }
};
class CDrawProjectileTargetingCentre {
public:
    unsigned char _pad00[0x1c];
    CIVHandle* PHandle;                                                  // +0x1c
    void Draw();
};
void CDrawProjectileTargetingCentre::Draw()
{
    CIVHandle empty;
    CIVHandle* handle = PHandle;
    handle->Reset();
    *handle = empty;
}
