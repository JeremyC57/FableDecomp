// CTCLight::SetLightDef(const CDefPointer<const CLightDef>& def) -- switch the light to overridden (inlined SetOverridden(true)) and copy the def's colour, radii, flicker and inverted flag into the private light info
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640E60, length 148 bytes (`ret 4`).
// Callees: operator delete @ 0x00BFE9BC, operator new @ 0x00BFEA1A (0x18-byte CLightInfo), CCountedPointer<CTCLight::CLightInfo>::Reset
// @ 0x00641EB0 -- all from the inlined SetOverridden(true) (out of line at 0x00640DF0, same body with the argument folded to 1).
// Layout: as SetOverridden 0x00640DF0; PDB CLightDef: Colour +0x28, InnerRadius +0x30, OuterRadius +0x34, Flicker +0x38, Inverted +0x58.
// Shape: each copy re-reads def.Object and PLightInfo.Data (stores may alias); floats copy through integer registers.
void __cdecl operator delete(void* block);          // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A
class CRGBColour { public: unsigned char B, G, R, A; };    // D3DCOLOR byte order
struct CLightInfo {                                 // CTCLight::CLightInfo (0x18, no ctor)
    CRGBColour    Colour;                           // +0x00
    float         InnerRadius;                      // +0x04
    float         OuterRadius;                      // +0x08
    float         Flicker;                          // +0x0c
    unsigned long LightingChannelsEffected;         // +0x10
    bool          Inverted;                         // +0x14
};
class CCPPointerInfo { public:                             // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    void Reset(T* data);                            // 0x00641EB0
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
class CLightDef { public:
    unsigned char _pad00[0x28];
    CRGBColour Colour;                              // +0x28
    long       HaloGraphic;                         // +0x2c
    float      InnerRadius;                         // +0x30
    float      OuterRadius;                         // +0x34
    float      Flicker;                             // +0x38
    unsigned char _pad3c[0x58 - 0x3c];
    bool       Inverted;                            // +0x58
};
template<class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CThing;
class CTCLight { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];             // CTCBase flags, CAGraphicAppearanceComponent vptr +0x0c
    CCountedPointer<CLightInfo> PLightInfo;         // +0x10
    CDefPointer<const CLightDef> PLightDef;         // +0x18
    bool  Active;                                   // +0x1c
    bool  Overridden;                               // +0x1d
    void SetOverridden(bool overridden)
    {
        if (Overridden != overridden) {
            if (Overridden)
                PLightInfo.Release();
            Overridden = overridden;
            if (overridden)
                PLightInfo.Reset(new CLightInfo);
        }
    }
    void SetLightDef(const CDefPointer<const CLightDef>& def);
};

void CTCLight::SetLightDef(const CDefPointer<const CLightDef>& def)
{
    SetOverridden(true);
    PLightInfo->Colour = def->Colour;
    PLightInfo->InnerRadius = def->InnerRadius;
    PLightInfo->OuterRadius = def->OuterRadius;
    PLightInfo->Flicker = def->Flicker;
    PLightInfo->Inverted = def->Inverted;
}