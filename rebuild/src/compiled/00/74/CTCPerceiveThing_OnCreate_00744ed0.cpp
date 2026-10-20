// CTCPerceiveThing::OnCreate() -- fetch the parent's thing def, take its CPerceivedThingDef sub-def into PDef, reset the perception state, load the four radii from the def (keeping ExtendedSightRadius >= SightRadius), the give-up-chase radius, and clear DirectAwareness
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744ED0, 157 bytes. Vtable 0x012688CC slot 4 (catalog "vfunc_4"; slot 4 is the CTC OnCreate).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0 (`mov ecx,[ecx+4] ; jmp CThing::GetDefinitionManager 0x004C7990`; catalog
//   "_Cons_val<...>"), CDefinitionManager::GetDef<CThingDef>(long, CDefPointer&) @ 0x0042AF3C (catalog
//   "GetDef<CSpecialAbilitiesDrunkennessDef>", one body of the shared template), CDefClassBase::GetSubDef<CPerceivedThingDef>
//   @ 0x00746100 (catalog "GetSubDef<CAICreatureWillPowerIndicatorDef>"), CTCPerceiveThing::ResetPerceptionVariables @ 0x00744DB0.
// Shape: the GetDef arguments (the parent's 16-bit def index at +0x8c, &def) are pushed before the manager call; the local def
//   pointer's zero store lands after the pushes; the four radius setters (landed 0x00744540 SetSightRadius, 0x00744470
//   SetSoundRadius, 0x007444A0 SetSmellRadius, 0x00744500 SetExtendedSightRadius) are inlined with the -1 "use the def" argument,
//   so only their def arms remain; the def is read once into ecx; the local def pointer (a CDefPointer<CThingDef> in the original) is taken into a
//   register once and released by hand at the end, as the GetSubDef OnCreate rows of earlier chunks.
// Layout: CTCPerceiveThing (engine/CTCPerceiveThing.h): DirectAwareness +0x30, PDef +0x34, SoundRadius +0x3c, SightRadius +0x40,
//   ExtendedSightRadius +0x44, SmellRadius +0x48, GiveUpChaseRadius +0x4c. CPerceivedThingDef: GiveUpChaseRadius +0x2c,
//   SightRadius +0x30, ExtendedSightRadius +0x34, SoundRadius +0x38, SmellRadius +0x3c (offsets from the landed setters).
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CThingDef;
struct CThingDefOut {                           // the GetDef out CDefPointer<CThingDef>, released by hand by the caller
    CThingDef* Object;
    CThingDefOut() : Object(0) {}
};
struct CPerceivedThingDef : CDefClassBase {
    unsigned char _pad08[0x24];
    float GiveUpChaseRadius;                    // +0x2c
    float SightRadius;                          // +0x30
    float ExtendedSightRadius;                  // +0x34
    float SoundRadius;                          // +0x38
    float SmellRadius;                          // +0x3c
};
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CPerceivedThingDef>* out);   // 0x00746100 (GetSubDef<CPerceivedThingDef>)
};
void* FindDef_CThingDef_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<CPerceivedThingDef>* out)
{
    void* def = FindDef_CThingDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    bool GetDef(long globalIndex, CThingDefOut* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CThingDefOut* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;              // +0x8c
};
struct CTCPerceiveThing {
    void*     __vftable;                        // +0x00 (CTCBase)
    CThing*   ParentThing;                      // +0x04
    bool      ToBeRemoved;                      // +0x08
    bool      IsInGlobalUpdateTCs;              // +0x09
    unsigned char _pad0a[0x26];                 // +0x0a PerceivedThing .. SearchingForThingToPerceive
    bool      DirectAwareness;                  // +0x30
    unsigned char _pad31[3];
    CDefPointer<CPerceivedThingDef> PDef;       // +0x34
    long      UpdateOffset;                     // +0x38
    float     SoundRadius;                      // +0x3c
    float     SightRadius;                      // +0x40
    float     ExtendedSightRadius;              // +0x44
    float     SmellRadius;                      // +0x48
    float     GiveUpChaseRadius;                // +0x4c
    CDefinitionManager* GetDefinitionManager(); // 0x006869D0 (CTCBase)
    const CPerceivedThingDef* GetDef() const { return PDef.Object; }
    void ResetPerceptionVariables();            // 0x00744DB0
    void SetSightRadius(float radius)
    {
        if (radius != -1.0f) {
            SightRadius = radius;
            return;
        }
        SightRadius = GetDef()->SightRadius;
        if (SightRadius > ExtendedSightRadius)
            ExtendedSightRadius = SightRadius;
    }
    void SetSoundRadius(float radius)
    {
        if (radius != -1.0f) {
            SoundRadius = radius;
            return;
        }
        SoundRadius = GetDef()->SoundRadius;
    }
    void SetSmellRadius(float radius)
    {
        if (radius != -1.0f) {
            SmellRadius = radius;
            return;
        }
        SmellRadius = GetDef()->SmellRadius;
    }
    void SetExtendedSightRadius(float radius)
    {
        if (radius != -1.0f) {
            ExtendedSightRadius = radius;
            return;
        }
        ExtendedSightRadius = GetDef()->ExtendedSightRadius;
        if (ExtendedSightRadius < SightRadius)
            ExtendedSightRadius = SightRadius;
    }
    void OnCreate();
};

void CTCPerceiveThing::OnCreate()
{
    CThingDefOut thingDef;
    CThing* thing = ParentThing;
    GetDefinitionManager()->GetDef(thing->DefGlobalIndex, &thingDef);
    CThingDef* def = thingDef.Object;
    def->GetSubDef(&PDef);
    ResetPerceptionVariables();
    SetSightRadius(-1.0f);
    SetSoundRadius(-1.0f);
    SetSmellRadius(-1.0f);
    SetExtendedSightRadius(-1.0f);
    GiveUpChaseRadius = GetDef()->GiveUpChaseRadius;
    DirectAwareness = false;
    if (def && --def->RefCount == 0)
        def->Destroy();
}