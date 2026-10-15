// CEnginePrimitiveRenderer2D::Set2DLighting(const C3DVector& lightVector, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& back) -- store the 2D ambient / diffuse / back light colours and the normalised light direction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4A5F0, length 122 bytes (+ int3 padding).
// Callees: C3DVector::Normalise() @ 0x00A14440 (in place, on the stored LightVector; manifest label "GetScaled" is propagated).
// Label correction: the manifest's "CEngine::Set2DLighting" belongs to CEnginePrimitiveRenderer2D (PDB: AmbientLightColour +0x50,
// DiffuseLightColour +0x60, BackLightColour +0x70, LightVector +0x80; retail 4 bytes lower behind the 12-byte PrimitiveManagers CArray).
// Shape: `ret 10h`; each POD colour record is assigned whole (`lea eax,[ecx+N]` then four dword moves through esi); the direction is
// assigned through `add ecx,7Ch ; mov edx,ecx` so ecx is already the member when Normalise is called on it.
struct C3DVector {
    float X, Y, Z;
    void Normalise();                        // 0x00A14440
};

struct CRGBFloatColour {
    float R, G, B, A;
};

struct CEnginePrimitiveRenderer2D {
    unsigned char   _pad00[0x4c];            // vptr, PrimitiveManagers, the manager pointers, the 2D primitive lists, FOV2D
    CRGBFloatColour AmbientLightColour;      // +0x4c
    CRGBFloatColour DiffuseLightColour;      // +0x5c
    CRGBFloatColour BackLightColour;         // +0x6c
    C3DVector       LightVector;             // +0x7c
    void Set2DLighting(const C3DVector& lightVector, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& back);
};

void CEnginePrimitiveRenderer2D::Set2DLighting(const C3DVector& lightVector, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& back)
{
    AmbientLightColour = ambient;
    DiffuseLightColour = diffuse;
    BackLightColour = back;
    LightVector = lightVector;
    LightVector.Normalise();
}