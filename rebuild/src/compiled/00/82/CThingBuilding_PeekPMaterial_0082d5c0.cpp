// CThingBuilding::PeekPMaterial() -- resolves the building def's wall material index through the definition manager into a counted material def pointer; a null pointer when the building def names none
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
class CDefClassBase { public:
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
#pragma pack(push, 1)
class CThingBuildingDef : public CDefClassBase { public:
    unsigned char _pad08[0x88 - 0x08];
    unsigned long WallMaterial;                    // +0x88
};
#pragma pack(pop)
struct CWallMaterial : CDefClassBase { };
template <class T> class CDefPointer { public:
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
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingBuildingDef>* out);   // 0x004AB840
    bool GetDef(long index, CDefPointer<CWallMaterial>* out);     // 0x0066C340
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
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CWallMaterial>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThingBuilding { public:
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