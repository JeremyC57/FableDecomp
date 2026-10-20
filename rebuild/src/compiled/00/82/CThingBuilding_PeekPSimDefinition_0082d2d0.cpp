// CThingBuilding::PeekPSimDefinition() -- resolves the building def's sim-building def index through the definition manager into a counted def pointer; a null pointer when the building def names none
// Leaf: PeekPSimDefinition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D2D0, 224 bytes. `ret 4` (the hidden result). Manifest "PeekPMaterial" is the propagated
//         label of the byte-twin 0x0082D5C0 (see docs/pipeline/GOTCHAS.md); this one reads SimBuildingDef.
// Callees: CThing::GetDefinitionManager() const 0x004C7990, CDefinitionManager::GetDef(long, CDefPointer<const CThingBuildingDef>*)
//          0x004AB840 (x2) and GetDef(long, CDefPointer<const CSimBuildingDef>*) 0x0082E1E0 (manifest "GetDef<CThingPhysicalSwitchDef>"
//          propagated for both), CDefClassBase virtual slot 1 (Delete).
// Shape: PeekPDef() is inlined twice: a zeroed holder filled by GetDef and released at once, the raw def pointer read after
//        the release (`[edi+74h]`); the result holder is zeroed, filled, copied into the hidden result (add-ref) and released
//        (the increment and decrement through one register under a shared null test); the empty arm stores a null result.
// Layout: CThing DefGlobalIndex +0x8C; CThingBuildingDef::SimBuildingDef +0x74 (Ego_r.pdb +0x78, def classes pack to 1).
struct CDefClassBase {
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
#pragma pack(push, 1)
struct CThingBuildingDef : CDefClassBase {
    unsigned char _pad08[0x74 - 0x08];
    unsigned long SimBuildingDef;                    // +0x74
};
#pragma pack(pop)
struct CSimBuildingDef : CDefClassBase { };
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        CDefClassBase* old = Object;                  // released through the unpacked base (the def classes pack to 1)
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingBuildingDef>* out);   // 0x004AB840
    bool GetDef(long index, CDefPointer<CSimBuildingDef>* out);     // 0x0082E1E0
};
void* FindDef_CDefinitionManager_GetDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingBuildingDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CSimBuildingDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingBuilding {
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;                   // +0x8c
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline const CThingBuildingDef* PeekPDef() const
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Object;
    }
    CDefPointer<CSimBuildingDef> PeekPSimDefinition() const;
};
CDefPointer<CSimBuildingDef> CThingBuilding::PeekPSimDefinition() const
{
    if (PeekPDef()->SimBuildingDef) {
        CDefPointer<CSimBuildingDef> sim;
        GetDefinitionManager()->GetDef(PeekPDef()->SimBuildingDef, &sim);
        return sim;
    }
    return CDefPointer<CSimBuildingDef>();
}