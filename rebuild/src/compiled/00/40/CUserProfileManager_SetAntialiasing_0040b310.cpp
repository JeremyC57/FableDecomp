class CUserProfileManager { public:
    char pad0[0x0c];
    char UseNewProfileValues;
    char pad1[0x13];
    float val20;            // +0x20 HUDAlpha (PDB)
    char pad2[0xbc];
    float vale0;            // +0xe0
};

extern void __fastcall CUserProfileManager_Apply(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_Refresh(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, int edx, float val, char flag)
{
    if (self->UseNewProfileValues)
        self->vale0 = val;
    self->val20 = val;
    CUserProfileManager_Apply(self);
    if (flag)
        CUserProfileManager_Refresh(self);
}