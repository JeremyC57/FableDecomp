// CEnginePrimitiveSpriteTrail::CEnginePrimitiveSpriteTrail() -- primitive type 0x14 with the default interpolation set and theme channel, an empty trail list, zero fade start and offset, identity world transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B09F90, length 103 bytes.
// Leaf: ??0CEnginePrimitiveSpriteTrail
// Label: the manifest's "C3DPrimitive2::C3DPrimitive2" is propagated; the only caller is the CPSCSingleSprite
// constructor (0x00B093C3), which builds its PDB member `CEnginePrimitiveSpriteTrail TrailUpdatePrimitive` at
// +0xac with this body (the next member, PrimIDList, follows at +0x12c: the retail object is 0x80 bytes).
// Callees: 0x00BFEA0E malloc (the list's 24-byte sentinel node).
// Layout: PDB CEnginePrimitiveSpriteTrail (0x88) with retail's 4-byte VC6 list (the sentinel pointer only, as
// the landed CMultiStatBar 0x006543AF found): CEnginePrimitive base {Type +0, InterpolationSet +4,
// ThemeChannelDesc +8}, Trail +0xc, then UpdateExisting..BlendOp +0x10..+0x4c, WorldTransform +0x50.
// Shape: no vtables; the base's inline constructor stores the type immediate, the interpolation set and the
// three channel bytes from the zero register; the inline VC6 list constructor nulls the head, mallocs the node
// and self-links it; FadeStart is an initialiser and OffsetVector a member object with its own zeroing constructor
// (body assignments would sink below the matrix); the matrix's inline default constructor
// is the landed SetIdentity (chained assignments: zeros in descending order, then the 1.0f diagonal).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

enum EEnginePrimitiveType { ENGINE_PRIMITIVE_SPRITE_TRAIL = 0x14 };
class CEnginePrimitiveThemeChannelDesc { public:                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                                // 12 bytes (PDB)
    EEnginePrimitiveType             Type;               // +0x00
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(EEnginePrimitiveType type) : Type(type), InterpolationSet(0) { }
};

class C3DVector  { public: float X, Y, Z; };
class CMatrix3x4 { public:
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()                                   // chained assignments: the stores run right to left
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

struct SZeroVector {                                     // C3DVector zeroed by its own constructor, as dwords
    long m_x, m_y, m_z;
    SZeroVector() : m_x(0), m_y(0), m_z(0) { }
};

struct CSpriteTrailEntry { float m_data[4]; };           // 16 bytes
template<class T> struct CRetailList {                   // VC6 list: the sentinel pointer only (retail)
    struct SNode { SNode* m_next; SNode* m_prev; T m_value; };   // 24 bytes
    SNode* m_head;
    CRetailList()
    {
        m_head = 0;
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->m_next = node;
        node->m_prev = node;
        m_head = node;
    }
};

struct CEnginePrimitiveSpriteTrail : CEnginePrimitive {
    CRetailList<CSpriteTrailEntry> Trail;                // +0x0c
    bool                           UpdateExisting;       // +0x10
    unsigned long                  MaxLength;            // +0x14
    unsigned long                  GraphicIndex;         // +0x18
    unsigned long                  GraphicFrame;         // +0x1c
    float                          SkipPoints;           // +0x20
    float                          Width;                // +0x24
    unsigned long                  HeadColour;           // +0x28 CRGBColour
    unsigned long                  MidColour;            // +0x2c
    unsigned long                  TailColour;           // +0x30
    long                           FadeStart;            // +0x34 (zeroed from the integer register)
    SZeroVector                    OffsetVector;         // +0x38 C3DVector
    unsigned short                 Flags;                // +0x44
    long                           BlendMode;            // +0x48
    long                           BlendOp;              // +0x4c
    CMatrix3x4                     WorldTransform;       // +0x50   sizeof == 0x80
    CEnginePrimitiveSpriteTrail();
};

CEnginePrimitiveSpriteTrail::CEnginePrimitiveSpriteTrail()
    : CEnginePrimitive(ENGINE_PRIMITIVE_SPRITE_TRAIL), FadeStart(0)
{
}