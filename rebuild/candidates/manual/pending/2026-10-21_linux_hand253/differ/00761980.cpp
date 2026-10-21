// Grade: DIFFER(365v389) -- retail keeps the shape iterator in a frame slot and 0xff in bl for the colour bytes; ours keeps the iterator in ebx (frame 0x60 v 0x64), point copy scheduled differently
// CTCShapeManager::DrawShapes(CEnginePrimitivePrepareInfo& info, const C3DVector& viewPos) -- editor draw: every persistent shape in blue with a red line from its first point (lifted 0.15) to the owner, then the selected shape in sand with its points highlighted white
// Leaf: DrawShapes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00761980, 400 bytes (`ret 8`).
// Callees: CTCBase::GetWorld() const 0x00686A80 and its virtual slot 13 (+0x34, the world map); NShape::CShape virtual slot 3
//          (+0x0c, the editor draw; descriptive) and CShape::EditorDrawHighlightPoints 0x007617A0; CThing::GetPosition 0x004C73D0;
//          CEnginePrimitiveLine ctor 0x004C3090; the engine's virtual slot 20 (+0x50, add primitive; descriptive).
// Layout: retail CTCShapeManager PersistantShapes +0x0c (STLport vector of 8-byte CCountedPointer<CShape>), SelectedShape +0x18
//          (PDB +0x1c, less 4 for the 12-byte release vector); CShape points vector +0x08; CEnginePrimitivePrepareInfo::Engine +0x34.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRGBColour {                                 // B,G,R,A bytes
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EEnginePrimitiveBlendMode { EPBM_NONE = 0 };
enum EEngineInterpolationSet { EEIS_NONE = 0 };
enum EEnginePrimitiveBlendOp { EPBO_NONE = 0 };
class CEnginePrimitiveLine {
public:
    unsigned char _d[0x3c];
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width,
                         EEnginePrimitiveBlendMode blendMode, unsigned long flags, bool depthTest, EEngineInterpolationSet interp,
                         EEnginePrimitiveBlendOp blendOp);   // 0x004C3090
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19();
    virtual void AddPrimitive(CEnginePrimitiveLine* line);   // slot 20 (+0x50)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                           // +0x34
};
struct CWorldMap;
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap() const;         // slot 13 (+0x34)
};
struct C3DVectorVector {                            // STLport vector<C3DVector>
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};
namespace NShape {
class CShape {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void EditorDraw(CEnginePrimitivePrepareInfo& info, CWorldMap* map, const C3DVector& viewPos, const CRGBColour& colour);   // slot 3
    long            m_refs;                         // +0x04
    C3DVectorVector Points;                         // +0x08
    void EditorDrawHighlightPoints(CEnginePrimitivePrepareInfo& info, CWorldMap* map, const C3DVector& viewPos, const CRGBColour& colour);   // 0x007617A0
};
}
struct CShapePointer {                              // CCountedPointer<NShape::CShape>
    NShape::CShape* Object;
    void*           Count;
};
struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CShapeIterator {                             // _Vector_iterator<CCountedPointer<NShape::CShape> >
    CShapePointer* _Myptr;
    CShapeIterator(CShapePointer* ptr) : _Myptr(ptr) {}
    CShapePointer& operator*() const { return *_Myptr; }
    CShapePointer* operator->() const { return _Myptr; }
    CShapeIterator& operator++() { ++_Myptr; return *this; }
    bool operator==(const CShapeIterator& right) const { return _Myptr == right._Myptr; }
    bool operator!=(const CShapeIterator& right) const { return !(*this == right); }
};
struct CShapeVector {
    CShapePointer* _Myfirst;
    CShapePointer* _Mylast;
    CShapePointer* _Myend;
    CShapeIterator begin() { return CShapeIterator(_Myfirst); }
    CShapeIterator end() { return CShapeIterator(_Mylast); }
};
class CTCShapeManager {
public:
    virtual void v00();
    CThing*        ParentThing;                     // +0x04
    long           _base08;                         // +0x08
    CShapeVector   PersistantShapes;                // +0x0c
    CShapeIterator SelectedShape;                   // +0x18
    const CWorld* GetWorld() const;                 // 0x00686A80 (CTCBase)
    void DrawShapes(CEnginePrimitivePrepareInfo& info, const C3DVector& viewPos);
};

void CTCShapeManager::DrawShapes(CEnginePrimitivePrepareInfo& info, const C3DVector& viewPos)
{
    CRGBColour shapeColour(0x50, 0x50, 0xff, 0xff);
    CRGBColour selectedColour(0xb4, 0xa0, 0x50, 0xff);
    CWorldMap* map = GetWorld()->GetWorldMap();
    for (CShapeIterator it = PersistantShapes.begin(); it != PersistantShapes.end(); ++it) {
        NShape::CShape* shape = it->Object;
        shape->EditorDraw(info, map, viewPos, shapeColour);
        if (shape->Points._M_start != shape->Points._M_finish) {
            C3DVector first = *shape->Points._M_start;
            C3DVector top(first.X, first.Y, first.Z + 0.15f);
            info.Engine->AddPrimitive(&CEnginePrimitiveLine(0, top, ParentThing->GetPosition(), CRGBColour(0xff, 0, 0, 0xff), 0.05f,
                                                            EPBM_NONE, 0, true, EEIS_NONE, EPBO_NONE));
        }
    }
    if (SelectedShape != PersistantShapes.end()) {
        NShape::CShape* shape = SelectedShape->Object;
        shape->EditorDraw(info, map, viewPos, selectedColour);
        shape->EditorDrawHighlightPoints(info, map, viewPos, CRGBColour(0xff, 0xff, 0xff, 0xff));
    }
}
