// Grade: DIFFER(548v760) 250 diff lines (parked 438/261): the local set<CCharString> modelled (header node malloc'd by the inline constructor, Erase 0x004ABA70 + free in the destructor) -- the set's use, the counted primitive-mesh exchange and the 0x00709A50 CEnginePrimitiveMorphedAnimatingMesh argument list are still missing (structural draft) (retry wave 2 sec2c)
// Was: DIFFER(438v760) -- structural draft only: the local set<CCharString> (header new'd and erased), the counted primitive-mesh exchange, the by-value argument built with push esp and the 0x709A50 argument list are not modelled yet
// Tried: one draft.
// CTCRandomAppearanceMorph::OnAppearanceDraw(CTCGraphicAppearance& app, const CGraphicAppearanceDrawInfo& info) -- on first draw insert one appearance node per morphed primitive; afterwards refresh animation only, or rebuild each primitive's morphed mesh for the current fade
// Leaf: OnAppearanceDraw
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727D00, 760 bytes, `ret 8`; `this` is the draw-callback base at +0x0c (ParentThing read as [this-8]).
// Callees: CTCGraphicAppearance alpha @ 0x004BBFF0 / GetFadeDistance @ 0x004BC180, operator new @ 0x00BFEA0E (set header),
//   the PrimHandles insert @ 0x00642440, memmove @ 0x00BFEAE6, the node params ctor @ 0x0099A2D0 / dtor @ 0x004C4160,
//   InsertAppearanceNode @ 0x004C0050, RefreshPrimitiveAnimationAndPos @ 0x004BCFE0, the LOD/coord helper @ 0x004BD040,
//   CEnginePrimitiveMorphedAnimatingMesh ctor @ 0x00709A50 / dtor @ 0x00709B10, the primitive getter @ 0x004BCF50,
//   SetPrimitiveGraphics @ 0x004BD620, the set erase @ 0x004ABA70, free @ 0x00BFEA14.
// Status: structural draft only (the by-value argument built with `push esp`, the counted-pointer exchange and the set
//   temporary are approximated).
extern "C" void __cdecl free(void* block);
void* __cdecl operator new(unsigned int size);
struct C3DVector { float X, Y, Z; };
struct CPrimitive;
struct CEngineMesh {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
};
struct CMeshRef {
    CEngineMesh* Data;
};
struct CNodeParams {
    long  Type;
    unsigned char _base[4];
    void* Vtable;
    long  Zero;
    long  Primitive;
    CNodeParams(long type, long primitive);
    ~CNodeParams();                                     // 0x004C4160
};
struct CMorphedMesh {
    unsigned char _body[0x50];
    CMorphedMesh(unsigned long lo, unsigned long hi, long a, const void* morphs, long b, long c, long d, bool e, void* names);   // 0x00709A50
    ~CMorphedMesh();                                    // 0x00709B10
};
struct CGraphicAppearanceDrawInfo {
    unsigned char _pad00[4];
    void* Anim;                                         // +0x04
    void* Pos;                                          // +0x08
    void* Orient;                                       // +0x0c
    unsigned char _pad10[0x1c - 0x10];
    long  Mode;                                         // +0x1c
};
struct CTCGraphicAppearance {
    unsigned char _pad00[0x40];
    C3DVector     Scale;                                // +0x40
    unsigned char _pad4c[0x5a - 0x4c];
    unsigned char LOD;                                  // +0x5a
    float GetAlpha();                                   // 0x004BBFF0
    float GetFadeDistance();                            // 0x004BC180
    CPrimitive* InsertAppearanceNode(const CNodeParams& params);   // 0x004C0050
    void RefreshPrimitiveAnimationAndPos(CPrimitive* p, void* orient, void* anim, void* pos);   // 0x004BCFE0
    long GetLevel(void* thing, float fade, long zero, const C3DVector* scale);   // 0x004BD040
    CMeshRef* GetPrimitive(CMeshRef* out);              // 0x004BCF50
    void SetPrimitiveGraphics(CPrimitive* p, CMorphedMesh* mesh, unsigned short flags);   // 0x004BD620
};
struct CThing {
    unsigned char _pad00[0x18];
    unsigned long IDLo, IDHi;                           // +0x18
};
struct SMeshPrimitiveMorphs {
    long          Primitive;
    void*         Morphs[3];
};
struct CMeshMorphSet {
    SMeshPrimitiveMorphs* First;
    SMeshPrimitiveMorphs* Last;
};
struct CPrimVector {
    CPrimitive** First;
    CPrimitive** Last;
    CPrimitive** End;
    void resize(unsigned int n)
    {
        if (n < (unsigned int)(Last - First))
            Last = First + n;
        else
            Grow(Last, n - (Last - First), 0);
    }
    void Grow(CPrimitive** where, unsigned int count, CPrimitive* const& value);   // 0x00642440
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
};
struct CAppearanceDrawCallback {
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& app, const CGraphicAppearanceDrawInfo& info) = 0;
};
struct CTCRandomAppearanceMorph : CTCBase, CAppearanceDrawCallback {
    CMeshMorphSet* PMeshMorphSet;                       // +0x10
    unsigned char  _pad14[0x1c - 0x14];
    CPrimVector    PrimHandles;                         // +0x1c
    bool OnAppearanceDraw(CTCGraphicAppearance& app, const CGraphicAppearanceDrawInfo& info);
};

extern "C" void* __cdecl malloc(unsigned int);   // 0x00BFEA0E
extern "C" void __cdecl free(void*);             // 0x00BFEA14
struct CStringSet {                                // STLport set<CCharString>: header node new'd by the constructor
    struct Node { char Color; Node* Parent; Node* Left; Node* Right; void* Value; };
    Node* Header;
    unsigned long NodeCount;
    __forceinline CStringSet() : Header(0)
    {
        Header = (Node*)malloc(sizeof(Node));
        NodeCount = 0;
        Header->Color = 0;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
    __forceinline ~CStringSet()
    {
        if (NodeCount) {
            Erase(Header->Parent);
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            NodeCount = 0;
        }
        free(Header);
    }
    void Erase(Node* node);                        // 0x004ABA70
};
bool CTCRandomAppearanceMorph::OnAppearanceDraw(CTCGraphicAppearance& app, const CGraphicAppearanceDrawInfo& info)
{
    float alpha = app.GetAlpha();
    float fade = app.GetFadeDistance();
    bool firstTime = PrimHandles.First == PrimHandles.Last;
    CStringSet names;
    unsigned int i = 0;
    for (SMeshPrimitiveMorphs* prim = PMeshMorphSet->First; prim != PMeshMorphSet->Last; ++prim, ++i) {
        if (firstTime) {
            PrimHandles.resize(i + 1);
            CNodeParams params(2, prim->Primitive);
            PrimHandles.First[i] = app.InsertAppearanceNode(params);
        }
        else if (info.Mode == 2 || info.Mode == 1 || info.Mode == 3) {
            app.RefreshPrimitiveAnimationAndPos(PrimHandles.First[i], info.Orient, info.Anim, info.Pos);
            continue;
        }
        long level = app.GetLevel(ParentThing, fade, 0, &app.Scale);
        CMorphedMesh mesh(ParentThing->IDLo, ParentThing->IDHi, level, prim->Morphs, (long)info.Orient, (long)info.Anim,
                          (long)info.Pos, true, 0);
        CMeshRef ref;
        app.GetPrimitive(&ref);
        app.SetPrimitiveGraphics(PrimHandles.First[i], &mesh, 0);
    }
    return true;
}
