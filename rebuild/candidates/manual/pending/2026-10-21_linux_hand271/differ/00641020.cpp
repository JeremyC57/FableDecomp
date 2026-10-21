// CTCLight::Persist(CPersistContext* context, bool flag) -- persist Active (a load with the flag set re-requests the appearance unless the hero state is being serialised) and, for an overridden light, its own light info (a load builds a fresh CLightInfo)
// Grade: DIFFER(461v488) -- this/context register swap and the Flicker transfer tail is not merged
// Leaf: Persist
struct CRGBColour {                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18 (1 / 3: saving)
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);               // 0x004045C0
    void Transfer(const char* name, float& value, const float& defaultValue);             // 0x00410620
    void Transfer(const char* name, CRGBColour& value, const CRGBColour& defaultValue);   // 0x006420F0
};

struct CLightInfo {                                     // CTCLight::CLightInfo (PDB, 24 bytes)
    CRGBColour    Colour;                               // +0x00
    float         InnerRadius;                          // +0x04
    float         OuterRadius;                          // +0x08
    float         Flicker;                              // +0x0c
    unsigned long LightingChannelsEffected;             // +0x10
    bool          Inverted;                             // +0x14
};

struct CLightInfoPointer {                              // CCountedPointer<CTCLight::CLightInfo>
    CLightInfo* m_pObject;
    void*       m_pBlock;
    void Reset(CLightInfo* object);                     // 0x00641EB0
    CLightInfo* operator->() const { return m_pObject; }
};

struct CWorld {
    unsigned char _pad00[0x102];
    bool          SerialisingHeroState;                 // +0x102 (PDB +0x10E)
};
class CThing {
public:
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                       // +0x6c
    void UpdateGraphicAppearance(long reason);          // 0x004C7590
    void SetAsFrameUpdated();                           // 0x004C7C40
};

struct CTCLight {
    CThing*           ParentThing;                      // +0x04
    unsigned char     _pad08[0x10 - 0x08];
    CLightInfoPointer PLightInfo;                       // +0x10
    const void*       PLightDef;                        // +0x18
    bool              Active;                           // +0x1c
    bool              Overridden;                       // +0x1d
    CWorld* GetWorld() const;                           // 0x00686A80 (CTCBase)
    virtual void v00();
    virtual void Persist(CPersistContext* context, bool flag);
};

void CTCLight::Persist(CPersistContext* context, bool flag)
{
    context->Transfer("Active", Active, false);
    if (flag && !context->IsSaving()) {
        if (!(ParentThing->ThingTypeFlags & 2) || !GetWorld()->SerialisingHeroState) {
            ParentThing->UpdateGraphicAppearance(4);
            ParentThing->SetAsFrameUpdated();
        }
    }
    context->Transfer("Overridden", Overridden, false);
    if (Overridden) {
        if (context->IsSaving()) {
            context->Transfer("Colour", PLightInfo->Colour, CRGBColour(0, 0, 0, 0xff));
            context->Transfer("InnerRadius", PLightInfo->InnerRadius, 0.0f);
            context->Transfer("OuterRadius", PLightInfo->OuterRadius, 0.0f);
            context->Transfer("Flicker", PLightInfo->Flicker, 0.0f);
        } else {
            PLightInfo.Reset(new CLightInfo);
            CRGBColour colour;
            context->Transfer("Colour", colour, CRGBColour(0, 0, 0, 0xff));
            PLightInfo->Colour = colour;
            context->Transfer("InnerRadius", PLightInfo->InnerRadius, 0.0f);
            context->Transfer("OuterRadius", PLightInfo->OuterRadius, 0.0f);
            context->Transfer("Flicker", PLightInfo->Flicker, 0.0f);
        }
        context->Transfer("Inverted", PLightInfo->Inverted, false);
    }
}
