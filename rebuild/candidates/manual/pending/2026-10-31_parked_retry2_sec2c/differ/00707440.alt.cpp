// CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& info) -- draw the hero's attachable appearance modifiers: when the primitives must be recreated drop the old ones; then per modifier either refresh the existing primitive's animation/position (animation-only redraws) or build a morphed animating mesh (the modifier's texture morphs plus the draw info's), creating its primitive node when recreating, and hand it to the appearance; clear the recreate flag
// Grade: DIFFER(1239v1253) -- first draft: control flow, loops, callees and the mesh-ctor argument list as retail; residue is register assignment (retail this ebx / appearance edi / zero esi, ours edi / ebp), the texture-morph param temporary build order, and the primitive index kept as a byte offset in a frame slot
// Leaf: ?OnAppearanceDraw@CTCHeroAttachableAppearanceModifiers
//
// Retail 0x00707440, 1264 bytes (FPO `sub esp,110h`, `ret 8`; `this` is the appearance-modifier interface at TC+0x0C).
// Callees: CTCGraphicAppearance::GetFadeDistance 0x004BC180 / RemovePrimitive 0x004BF920 / RefreshPrimitiveAnimationAndPos
//   0x004BCFE0 / the primitive-node insert 0x004C0050 / GetColour 0x004BD040 / the theme-channel getter 0x004BCF50 /
//   SetPrimitiveGraphics 0x004BD620, vector<CPrimitive*>::reserve 0x00708F50 / InsertGrow 0x006422B0, the texture-morph list's
//   Add 0x006CA950, CBaseClass ctor 0x0099A2D0 / dtor 0x0099A2E0, malloc 0x00BFEA0E, free 0x00BFEA14,
//   CEnginePrimitiveMorphedAnimatingMesh ctor 0x00709A50 / dtor 0x00709B10, the set's node erase 0x004ABA70.
// Layout: retail offsets (the generated engine header carries Ego_r.pdb debug-build offsets, 16-byte vectors): interface
//   `this` = TC+0x0C; ParentThing at this-8; flag byte this+0x08 (bit 1 RecreatePrimitives); PrimHandles this+0x0C;
//   AppearanceModifiers this+0x30 (12-byte entries {def, primitive data, scale}); modifier def texture morphs +0x28 (8-byte
//   {texture, morph}); CGraphicAppearanceDrawInfo Pos +4, Orientation +8, Anim +0x0C, PTextureMorphs +0x10 (0x14-byte
//   entries, 0x10-byte morph params at +8), ReasonsForDraw +0x1C; appearance +0x34 LOD scale, +0x40 theme channel desc,
//   +0x5A interpolation byte; CThing UID +0x18.
#include <stddef.h>
#include <new>
extern "C" void* __cdecl malloc(size_t size);
extern "C" void __cdecl free(void*);

struct CRGBColour {
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct C3DAnimationInterpolator;
struct C3DPositionInterpolator;
struct C3DOrientationInterpolator;
struct CEngineTextureMorphParams {                      // 0x10 bytes
    long       Morph;
    long       Type;
    CRGBColour Colour1;
    CRGBColour Colour2;
    CEngineTextureMorphParams(long morph, long type, CRGBColour c1, CRGBColour c2) : Morph(morph), Type(type), Colour1(c1), Colour2(c2) {}
};
struct CEngineTextureMorph {                            // 0x14 bytes
    long Texture;
    long Unused;
    CEngineTextureMorphParams* ParamsBegin;             // +0x08
    CEngineTextureMorphParams* ParamsEnd;
    CEngineTextureMorphParams* ParamsCapacity;
    unsigned long ParamCount() const { return ParamsEnd - ParamsBegin; }
};
struct CTextureMorphList {
    CEngineTextureMorph* first;
    CEngineTextureMorph* last;
    CEngineTextureMorph* end;
    CTextureMorphList() : first(0), last(0), end(0) {}
    ~CTextureMorphList()
    {
        for (CEngineTextureMorph* p = first; p != last; ++p)
            if (p->ParamsBegin)
                free(p->ParamsBegin);
        if (first)
            free(first);
    }
    void Add(long texture, const CEngineTextureMorphParams& params);   // 0x006CA950
};
struct CTextureMorphVector {
    CEngineTextureMorph* first;
    CEngineTextureMorph* last;
    CEngineTextureMorph* end;
    unsigned long size() const { return last - first; }
};
struct SSetNode { unsigned char Colour; SSetNode* Parent; SSetNode* Left; SSetNode* Right; void* Value; };
struct CCharStringSet {
    long      Proxy;
    SSetNode* Header;
    unsigned long Count;
    void EraseSubtree(SSetNode* node);                  // 0x004ABA70
    CCharStringSet() : Proxy(0)
    {
        Header = (SSetNode*)malloc(sizeof(SSetNode));
        Count = 0;
        Header->Colour = 0;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
    ~CCharStringSet()
    {
        if (Count) {
            EraseSubtree(Header->Parent);
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            Count = 0;
        }
        if (Header)
            free(Header);
    }
};
struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    virtual void v02();
    virtual void OnLastReference(void* owner);          // slot 3 (+0x0C)
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template<class T> struct CCountedPtr {
    T* Object;
    CCountedPtr() : Object(0) {}
    ~CCountedPtr() { if (Object) { Object->Release(); Object = 0; } }
    CCountedPtr& operator=(const CCountedPtr& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CBaseClass {
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CPrimitiveRef : CBaseClass {
    CRefCounted* P;
    long         Data;
    CPrimitiveRef(long data) : P(0), Data(data) {}
    ~CPrimitiveRef()
    {
        if (P) {
            if (P->RefCount == 1)
                P->OnLastReference(this);
            if (P)
                P->Release();
        }
        P = 0;
    }
};
struct SPrimitiveNodeDesc {
    long          Type;
    CPrimitiveRef Ref;
    SPrimitiveNodeDesc(long type, long data) : Type(type), Ref(data) {}
};
struct CEnginePrimitiveThemeChannelDesc { long _body[6]; };
struct CEnginePrimitiveMorphedAnimatingMesh {
    unsigned char _pad00[0x80];
    CCountedPtr<CRefCounted> Theme;                     // +0x80
    unsigned char _pad84[0xa4 - 0x84];
    CEnginePrimitiveMorphedAnimatingMesh(unsigned __int64 id, unsigned long data, const CTextureMorphList& morphs,
        const CCharStringSet& hidden, const C3DAnimationInterpolator& anim, const C3DPositionInterpolator& pos,
        const C3DOrientationInterpolator& orientation, float scale, bool flag, unsigned char interpolation,
        CRGBColour colour, float fade, long interpolationSet, const CEnginePrimitiveThemeChannelDesc& theme);   // 0x00709A50
    ~CEnginePrimitiveMorphedAnimatingMesh();            // 0x00709B10
};
struct CPrimitive;
namespace NGraphicAppearance {
struct CGraphicAppearanceDrawInfo {
    float                       Interpolate;            // +0x00
    C3DPositionInterpolator*    Pos;                    // +0x04
    C3DOrientationInterpolator* Orientation;            // +0x08
    C3DAnimationInterpolator*   Anim;                   // +0x0C
    CTextureMorphVector*        PTextureMorphs;         // +0x10
    void*                       PTextureDecals;         // +0x14
    void*                       HiddenMaterials;        // +0x18
    unsigned long               ReasonsForDraw;         // +0x1C
};
struct CTCGraphicAppearance {
    unsigned char _pad00[0x34];
    float         LODScale;                             // +0x34
    unsigned char _pad38[0x40 - 0x38];
    CEnginePrimitiveThemeChannelDesc ThemeDesc;         // +0x40
    unsigned char _pad58[0x5a - 0x58];
    unsigned char Interpolation;                        // +0x5A
    float GetFadeDistance();                            // 0x004BC180
    void RemovePrimitive(CPrimitive* primitive);        // 0x004BF920
    void RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& anim,
                                         const C3DPositionInterpolator& pos, const C3DOrientationInterpolator& orientation);   // 0x004BCFE0
    CPrimitive* InsertPrimitiveNode(const SPrimitiveNodeDesc& desc);   // 0x004C0050
    CRGBColour GetColour();                             // 0x004BD040
    CCountedPtr<CRefCounted> GetThemeChannel();         // 0x004BCF50
    void SetPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitiveMorphedAnimatingMesh& mesh, unsigned short flags);   // 0x004BD620
};
}
using namespace NGraphicAppearance;
struct SScalarTag { SScalarTag() { } };
struct CPrimitiveVector {
    CPrimitive** first;
    CPrimitive** last;
    CPrimitive** end;
    CPrimitive*& operator[](unsigned long i) { return *(first + i); }
    bool empty() const { return first == last; }
    CPrimitive*& back() { return *(last - 1); }
    void pop_back() { --last; }
    void reserve(unsigned long n);                      // 0x00708F50
    void InsertGrow(CPrimitive** where, CPrimitive* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x006422B0
    void push_back(CPrimitive* const& value)
    {
        if (last != end) {
            new (last) CPrimitive*(value);
            ++last;
        } else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct STextureMorphEntry { long Texture; long Morph; };
struct CModifierDef {
    unsigned char       _pad00[0x28];
    STextureMorphEntry* MorphsBegin;                    // +0x28
    STextureMorphEntry* MorphsEnd;                      // +0x2C
};
struct CModifierEntry {                                 // 12 bytes
    CModifierDef* Def;
    long          PrimitiveData;
    float         Scale;
};
struct CThing {
    unsigned char    _pad00[0x18];
    unsigned __int64 UID;                               // +0x18
};
struct CModifierEntryVector {
    CModifierEntry* first;
    CModifierEntry* last;
    CModifierEntry* end;
    unsigned long size() const { return last - first; }
};

class CTCHeroAttachableAppearanceModifiers {           // the appearance-modifier interface sub-object (TC+0x0C)
public:
    virtual void v00();
    unsigned char        _pad04[0x08 - 0x04];
    unsigned char        Flags;                         // +0x08 (bit 1 RecreatePrimitives)
    unsigned char        _pad09[0x0c - 0x09];
    CPrimitiveVector     PrimHandles;                   // +0x0C
    unsigned char        _pad18[0x30 - 0x18];
    CModifierEntryVector AppearanceModifiers;           // +0x30
    CThing* ParentThing() const { return *(CThing* const*)((const char*)this - 8); }
    bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
};

bool CTCHeroAttachableAppearanceModifiers::OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info)
{
    float lodScale = appearance.LODScale;
    float fade = appearance.GetFadeDistance();
    if (Flags & 2) {
        while (!PrimHandles.empty()) {
            appearance.RemovePrimitive(PrimHandles.back());
            PrimHandles.pop_back();
        }
        PrimHandles.reserve(AppearanceModifiers.size());
    }
    unsigned long primitive = 0;
    for (CModifierEntry* entry = AppearanceModifiers.first; entry != AppearanceModifiers.last; ++entry) {
        if (!(Flags & 2) && (info.ReasonsForDraw == 2 || info.ReasonsForDraw == 1 || info.ReasonsForDraw == 3)) {
            appearance.RefreshPrimitiveAnimationAndPos(PrimHandles[primitive], *info.Anim, *info.Pos, *info.Orientation);
            ++primitive;
            continue;
        }
        CTextureMorphList morphs;
        if (entry->Def) {
            for (long i = 0; i < entry->Def->MorphsEnd - entry->Def->MorphsBegin; ++i)
                morphs.Add(entry->Def->MorphsBegin[i].Texture,
                           CEngineTextureMorphParams(entry->Def->MorphsBegin[i].Morph, 3, CRGBColour(0, 0, 0, 0xff), CRGBColour(0, 0, 0, 0)));
        }
        if (CTextureMorphVector* extra = info.PTextureMorphs) {
            for (unsigned long i = 0; i < extra->size(); ++i)
                for (unsigned long j = 0; j < extra->first[i].ParamCount(); ++j)
                    morphs.Add(extra->first[i].Texture, extra->first[i].ParamsBegin[j]);
        }
        if (Flags & 2) {
            CPrimitive* node = appearance.InsertPrimitiveNode(SPrimitiveNodeDesc(2, entry->PrimitiveData));
            PrimHandles.push_back(node);
        }
        CCharStringSet hidden;
        unsigned char interpolation = appearance.Interpolation;
        CThing* parent = ParentThing();
        CEnginePrimitiveMorphedAnimatingMesh mesh(parent->UID, entry->PrimitiveData, morphs, hidden, *info.Anim, *info.Pos,
            *info.Orientation, lodScale * entry->Scale, true, interpolation, appearance.GetColour(), fade, 0, appearance.ThemeDesc);
        mesh.Theme = appearance.GetThemeChannel();
        appearance.SetPrimitiveGraphics(PrimHandles[primitive], mesh, 0);
        ++primitive;
    }
    Flags &= ~2;
    return true;
}
