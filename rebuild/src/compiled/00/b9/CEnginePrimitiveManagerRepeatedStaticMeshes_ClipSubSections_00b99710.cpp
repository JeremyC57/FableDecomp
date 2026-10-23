// CEnginePrimitiveManagerRepeatedStaticMeshes::ClipSubSections(const CSubsectionElement* element, unsigned long clipFlags, float range, bool inside, CClipListElement*& out) -- walk the four sub-sections of a repeated-mesh quadtree node: reject those outside the camera range sphere, clip the rest against the frustum planes still open, and emit a clip-list entry for each visible leaf or recurse into its children
// Leaf: ?ClipSubSections@CEnginePrimitiveManagerRepeatedStaticMeshes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B99710, length 336 bytes (body 0x14f), `ret 14h`.
// PDB: ?ClipSubSections@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXPBUCSubsectionElement@NEnginePrimitiveManagerRepeatedStaticMeshes@@KM_NAAPAUCClipListElement@1@@Z.
// Callees: CEngineCamera::ClipSphere(const C3DVector&, float, unsigned long&) @ 0x00B30480 (camera global @ 0x01436EA0,
//   position +0x54), itself.
// Layout: CSubsectionElement (0x50 bytes, structure-of-arrays over the four children): X[4] +0, Y[4] +0x10, Z[4] +0x20,
//   Radius[4] +0x30, Count[4] (bytes) +0x40, Data[4] +0x44, Child[4] +0x48 (element offset to the child node); the 2-byte
//   CClipListElement {Data, Count}. Local model (names descriptive).
// Shape: the centre is copied into a local C3DVector; the camera distance is the Z,Y,X sum of squares on the x87 stack and
//   compared with (radius + range)^2 / (range - radius)^2; the plane flags live in the dead `out` slot.
struct C3DVector { float X, Y, Z; };

class CEngineCamera {
public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                    // +0x54
    void ClipSphere(const C3DVector& centre, float radius, unsigned long& flags);   // 0x00B30480
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

namespace NEnginePrimitiveManagerRepeatedStaticMeshes {
struct CSubsectionElement {
    float         X[4];                             // +0x00
    float         Y[4];                             // +0x10
    float         Z[4];                             // +0x20
    float         Radius[4];                        // +0x30
    unsigned char Count[4];                         // +0x40
    unsigned char Data[4];                          // +0x44
    unsigned char Child[4];                         // +0x48
    unsigned char _pad4c[4];
};
struct CClipListElement {
    unsigned char Data;
    unsigned char Count;
};
}
using namespace NEnginePrimitiveManagerRepeatedStaticMeshes;

class CEnginePrimitiveManagerRepeatedStaticMeshes {
public:
    void ClipSubSections(const CSubsectionElement* element, unsigned long clipFlags, float range, bool inside, CClipListElement*& out);
};

void CEnginePrimitiveManagerRepeatedStaticMeshes::ClipSubSections(const CSubsectionElement* element, unsigned long clipFlags, float range,
                                                                  bool inside, CClipListElement*& out)
{
    for (long i = 0; i < 4; ++i) {
        if (!element->Count[i])
            continue;
        unsigned long flags = clipFlags;
        bool childInside = inside;
        C3DVector centre;
        centre.X = element->X[i];
        centre.Y = element->Y[i];
        centre.Z = element->Z[i];
        float radius = element->Radius[i];
        if (!childInside) {
            float dx = centre.X - g_pEngineCamera->WorldPosition.X;
            float dy = centre.Y - g_pEngineCamera->WorldPosition.Y;
            float dz = centre.Z - g_pEngineCamera->WorldPosition.Z;
            float distanceSq = dx * dx + dy * dy + dz * dz;
            float outer = radius + range;
            if (distanceSq > outer * outer)
                continue;
            float inner = range - radius;
            if (distanceSq < inner * inner)
                childInside = true;
        }
        if ((flags & 0x3c) != 0x3c)
            g_pEngineCamera->ClipSphere(centre, radius, flags);
        if (flags & 0x40)
            continue;
        flags = 0;
        if (element->Child[i] == 0) {
            out->Data = element->Data[i];
            out[0].Count = element->Count[i];
            ++out;
        }
        else
            ClipSubSections(element + element->Child[i], flags, range, childInside, out);
    }
}