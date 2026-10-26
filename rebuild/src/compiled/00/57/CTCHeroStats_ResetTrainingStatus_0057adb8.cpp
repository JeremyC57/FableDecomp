// CTCHeroStats::ResetTrainingStatus() -- restores the hero's training status to the default from the parent thing's CHeroDef sub-definition
// Tried: (1) this form, pointee Release member + `if (Object) Object->Release()` 70v63; (2) `if (Object && --Object->RefCount == 0)` in the
// destructor: dec folds but Object is re-read after it (70v63); (3) local copy + direct dec: hoisted load lost (73v63); (4) local copy +
// pointee Release: 73v63.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057ADB8, 63 bytes.
// Callees: CDefClassBase::GetSubDef<CHeroDef>(CDefPointer<const CHeroDef>*) const @ 0x00580034 (looks the "CHeroDef"
//          sub-definition up by name, returns bool), CHeroDef virtual slot 1 (+0x04, destroy at count zero).
//
// Shape: a frame-pointer size TU (`push ebp ; mov ebp,esp ... leave`, `and dword ptr [ebp-4],0`: #pragma optimize("y",
// off) + optimize("s", on)). The one-word frame is a default-constructed (null) CDefPointer<const CHeroDef> whose
// address is handed to GetSubDef on the parent thing's base definition (ParentThing +0x70); a missing base definition
// jumps straight to the epilogue (VC knows the def pointer is still null there, so the release is skipped); on success
// TrainingStatus (+0x114) takes CHeroDef::DefaultHeroTrainingStatus (+0x2C); the destructor's null-tested release
// (`test ecx,ecx ; je ; dec [ecx+4] ; jne ; call [vt+4]`) closes the block. Release / destructor are `__forceinline`.
// Layout: retail CTCHeroStats sits 8 bytes below the PDB here: TrainingStatus +0x114 (PDB +0x11C); retail
// CThing::PBaseDef +0x70 (PDB +0x80: the component map, update list and serialisation string are smaller in retail).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CHeroDef { public:
    virtual void v00();
    virtual void Destroy();                    // slot 1 (+0x04)
    long          RefCount;                    // +0x04 CDefPointeeBase
    unsigned char _pad_0x08[0x24];
    long          DefaultHeroTrainingStatus;   // +0x2C EHeroTrainingStatus
    __forceinline void Release() { if (--RefCount == 0) Destroy(); }
};

struct CHeroDefPointer {                       // CDefPointer<const CHeroDef>
    CHeroDef* Object;
    __forceinline CHeroDefPointer() : Object(0) {}
    __forceinline ~CHeroDefPointer()
    {
        if (Object)
            Object->Release();
    }
    CHeroDef* operator->() const { return Object; }
};

class CDefClassBase { public:
    bool GetSubDef(CHeroDefPointer* subDef) const;   // GetSubDef<CHeroDef> @ 0x00580034
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CHeroDefPointer* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PBaseDef;                   // +0x70 CDefPointer<const CThingBaseDef>::Object (PDB +0x80)
};

struct CTCHeroStats_Retail {
    void*         __vftable;                   // +0x00
    CThing*       ParentThing;                 // +0x04 CTCBase
    unsigned char _pad_0x08[0x10C];
    long          TrainingStatus;              // +0x114 EHeroTrainingStatus (PDB +0x11C)

    void ResetTrainingStatus();
};

void CTCHeroStats_Retail::ResetTrainingStatus()
{
    CHeroDefPointer heroDef;
    CDefClassBase* baseDef = ParentThing->PBaseDef;
    if (baseDef) {
        if (baseDef->GetSubDef(&heroDef))
            TrainingStatus = heroDef->DefaultHeroTrainingStatus;
    }
}