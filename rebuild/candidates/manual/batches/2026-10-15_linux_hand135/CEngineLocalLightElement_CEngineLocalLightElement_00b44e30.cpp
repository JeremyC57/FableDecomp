// CEngineLocalLightElement::CEngineLocalLightElement(const C3DVector& worldPos, float radius, float spotRadius, const CRGBFloatColour& colour, unsigned long channelMask) -- records a local light and precomputes its squared radius and attenuation terms
// Leaf: ??0CEngineLocalLightElement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44E30, length 128 bytes. No calls.
// Globals: the 1.0f / -1.0f pool constants @ 0x0122DED8 / 0x0122DEE0.
// Shape: the members are the PDB's (0x34 bytes): WorldPos, Radius, RadiusSquared, SpotRadius, Colour (a nested POD copied
// through `lea edx,[eax+18h]` and four moves), AttenuationDistSquared, AttenuationConstant, ChannelMask. RadiusSquared is
// `radius * radius` from the parameter (fst), its reciprocal stays on the x87 stack as a named local, then
// AttenuationDistSquared = 1/spotRadius^2 - 1/radius^2 and AttenuationConstant = (1/radius^2) / AttenuationDistSquared * -1.
// WorldPos is copied word by word through edx/ecx straight into [eax]: C3DVector has a memberwise user copy constructor
// (the implicit POD copy goes through `mov edx,eax` and esi). The class has no generated engine header in the tree, so it is
// modelled locally with the PDB names.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CRGBFloatColour {
    float R, G, B, A;
};

struct CEngineLocalLightElement {
    C3DVector       WorldPos;                      // +0x00
    float           Radius;                        // +0x0C
    float           RadiusSquared;                 // +0x10
    float           SpotRadius;                    // +0x14
    CRGBFloatColour Colour;                        // +0x18
    float           AttenuationDistSquared;        // +0x28
    float           AttenuationConstant;           // +0x2C
    unsigned long   ChannelMask;                   // +0x30
    CEngineLocalLightElement(const C3DVector& worldPos, float radius, float spotRadius, const CRGBFloatColour& colour,
                             unsigned long channelMask);
};

CEngineLocalLightElement::CEngineLocalLightElement(const C3DVector& worldPos, float radius, float spotRadius,
                                                   const CRGBFloatColour& colour, unsigned long channelMask)
    : WorldPos(worldPos), Radius(radius), RadiusSquared(radius * radius), SpotRadius(spotRadius), Colour(colour),
      ChannelMask(channelMask)
{
    float invRadiusSquared = 1.0f / RadiusSquared;
    AttenuationDistSquared = 1.0f / (spotRadius * spotRadius) - invRadiusSquared;
    AttenuationConstant = invRadiusSquared / AttenuationDistSquared * -1.0f;
}
