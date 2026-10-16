// CTCLight::SetOverridden(bool overridden) -- on a change: drop the shared light info when leaving the overridden state, record the flag, and give an overridden light a fresh private CLightInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640DF0, length 99 bytes (`ret 4`).
// Callees: operator delete @ 0x00BFE9BC (the counted pointer's info block, after its __fastcall DeleteFunc on the data),
// operator new @ 0x00BFEA1A (0x18-byte CLightInfo), CCountedPointer<CTCLight::CLightInfo>::Reset(T*) @ 0x00641EB0
// (manifest label "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset" is a byte-twin).
// Layout (retail = PDB CTCLight, landed ~CTCLight 0x00640F80): PLightInfo +0x10 (Data, Info), PLightDef +0x18, Active +0x1C,
// Overridden +0x1D.
// Shape: the release is an inline CCountedPointer member (re-reads Info after the decrement, then zeroes Info and Data in that
// order); `new CLightInfo` has no ctor so the raw pointer goes straight to Reset.
void __cdecl operator delete(void* block);          // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);      // 0x00BFEA1A
struct CRGBColour { unsigned char B, G, R, A; };    // D3DCOLOR byte order
struct CLightInfo {                                 // CTCLight::CLightInfo (0x18, no ctor)
    CRGBColour    Colour;                           // +0x00
    float         InnerRadius;                      // +0x04
    float         OuterRadius;                      // +0x08
    float         Flicker;                          // +0x0c
    unsigned long LightingChannelsEffected;         // +0x10
    bool          Inverted;                         // +0x14
};
struct CCPPointerInfo {                             // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> struct CCountedPointer {
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
struct CLightDef {
    unsigned char _pad00[0x28];
    CRGBColour Colour;                              // +0x28
    long       HaloGraphic;                         // +0x2c
    float      InnerRadius;                         // +0x30
    float      OuterRadius;                         // +0x34
    float      Flicker;                             // +0x38
    unsigned char _pad3c[0x58 - 0x3c];
    bool       Inverted;                            // +0x58
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CThing;
struct CTCLight {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];             // CTCBase flags, CAGraphicAppearanceComponent vptr +0x0c
    CCountedPointer<CLightInfo> PLightInfo;         // +0x10
    CDefPointer<const CLightDef> PLightDef;         // +0x18
    bool  Active;                                   // +0x1c
    bool  Overridden;                               // +0x1d
    void SetOverridden(bool overridden);
};

void CTCLight::SetOverridden(bool overridden)
{
    if (Overridden != overridden) {
        if (Overridden)
            PLightInfo.Release();
        Overridden = overridden;
        if (overridden)
            PLightInfo.Reset(new CLightInfo);
    }
}