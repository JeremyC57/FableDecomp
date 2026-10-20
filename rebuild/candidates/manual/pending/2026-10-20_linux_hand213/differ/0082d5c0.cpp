// CThingBuilding::PeekPMaterial() -- resolves the building def's wall material index through the definition manager into a counted material def pointer; a null pointer when the building def names none
// Grade: DIFFER(ours v retail) -- 260v227: same pack(1) release shape as 0x0082D2D0
// Leaf: PeekPMaterial
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D5C0, 240 bytes. `ret 4` (the hidden result). Byte-twin of 0x0082D2D0 (PeekPSimDefinition)
//         reading WallMaterial instead (see docs/pipeline/GOTCHAS.md).
// Callees: CThing::GetDefinitionManager() const 0x004C7990, CDefinitionManager::GetDef(long, CDefPointer<const CThingBuildingDef>*)
//          0x004AB840 (x2) and GetDef(long, CDefPointer<const CWallMaterial>*) 0x0066C340 (manifest "GetDef<CThingPhysicalSwitchDef>"
//          propagated for both), CDefClassBase virtual slot 1 (Delete).
// Shape: PeekPDef() is inlined twice: a zeroed holder filled by GetDef and released at once, the raw def pointer read after
//        the release (`[edi+88h]`); the result holder is zeroed, filled, copied into the hidden result (add-ref) and released
//        (the increment and decrement through one register under a shared null test); the empty arm stores a null result.
// Layout: CThing DefGlobalIndex +0x8C; CThingBuildingDef::WallMaterial +0x88 (Ego_r.pdb +0x8C, def classes pack to 1).
struct CDefClassBase {
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
#pragma pack(push, 1)
struct CThingBuildingDef : CDefClassBase {
    unsigned char _pad08[0x88 - 0x08];
    unsigned long WallMaterial;                    // +0x88
};
#pragma pack(pop)
struct CWallMaterial : CDefClassBase { };
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
    bool GetDef(long index, CDefPointer<CWallMaterial>* out);     // 0x0066C340
};
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
    CDefPointer<CWallMaterial> PeekPMaterial() const;
};
CDefPointer<CWallMaterial> CThingBuilding::PeekPMaterial() const
{
    if (PeekPDef()->WallMaterial) {
        CDefPointer<CWallMaterial> sim;
        GetDefinitionManager()->GetDef(PeekPDef()->WallMaterial, &sim);
        return sim;
    }
    return CDefPointer<CWallMaterial>();
}
