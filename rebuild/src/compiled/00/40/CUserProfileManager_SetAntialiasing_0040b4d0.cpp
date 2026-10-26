class CUserProfileManager { public:
    char pad0[0x0c];
    char UseNewProfileValues;        // +0x0c
    char pad1[0x38 - 0x0d];
    int Antialiasing;         // +0x38
    char pad2[0xf4 - 0x3c];
    int fldF4;         // +0xf4
};

extern "C" void __fastcall UPM_Apply(CUserProfileManager* self);
extern "C" void __fastcall UPM_Refresh(CUserProfileManager* self);

// __fastcall modeled as __fastcall: this in ecx, unused edx placeholder,
// then the real stack params (value, doRefresh) => ret 0x8.
void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, int /*edx*/, int value, char doRefresh)
{
    if (self->UseNewProfileValues)
        self->fldF4 = value;
    self->Antialiasing = value;
    UPM_Apply(self);
    if (doRefresh)
        UPM_Refresh(self);
}