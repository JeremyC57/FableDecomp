// Reconstruction of CTCCameraPointFixedPoint::GetPointType-style query
// retail 0x0072a430: virtual call to slot 0x70 returns a point-type enum;
// if it equals 7 (fixed-point), writes this into *out and returns true.

class CTCCameraPointDefinitionBase;

struct CTCCameraPointVtbl {
    // slot 0x70 / 4 = index 28
    int (__fastcall *pad[28])(CTCCameraPointDefinitionBase*);
    int (__fastcall *GetPointType)(CTCCameraPointDefinitionBase*);
};

class CTCCameraPointDefinitionBase { public:
    CTCCameraPointVtbl* vtbl;
};

bool __fastcall GetPointType_CTCCameraPointFixedPoint_(
    CTCCameraPointDefinitionBase* self,
    void* /*edx*/,
    CTCCameraPointDefinitionBase** out)
{
    if (self->vtbl->GetPointType(self) == 7) {
        *out = self;
        return true;
    }
    return false;
}