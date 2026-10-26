#pragma optimize("s", on)
class CTCVolumeContainmentTrackerDef
{ public:
    void* vftable;
};
extern void* const g_CTCVolumeContainmentTrackerDef_vftable;
extern void __fastcall CTCVolumeContainmentTrackerDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCVolumeContainmentTrackerDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CTCVolumeContainmentTrackerDef_vftable;
    CTCVolumeContainmentTrackerDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}