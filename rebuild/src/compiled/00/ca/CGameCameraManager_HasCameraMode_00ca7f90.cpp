// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?AreWeOutOfSync@CEnumMediaTypes@@AAEHXZ -- 25 bytes verified against Fable.exe 0x00CA7F90
enum EMode { EM_0 = 0, EM_1 = 1 };

struct ICamProvider {
    virtual int vf0() = 0;
    virtual int vf1() = 0;
    virtual int vf2() = 0;
    virtual int vf3() = 0;
    virtual int GetMode() = 0;   // slot 4 -> [vtbl+0x10]
};

class CGameCameraManager { public:
    int pad0;              // +0
    int pad4;              // +4
    ICamProvider* prov;    // +8
    int curMode;           // +0xc
};

int __fastcall CGameCameraManager_HasCameraMode(CGameCameraManager* self, EMode mode)
{
    return self->prov->GetMode() != self->curMode;
}