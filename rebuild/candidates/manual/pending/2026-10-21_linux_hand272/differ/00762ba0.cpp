// CTCShapeManager::OnSerialise(CPersistContext* context, long version) -- transfer the "coords relative to map" flag (and, when set, compute the owner region's map offset), then the shape count; when saving, serialise every persistent shape with the offset, when loading reset the closed-shape bounds and load each shape through StatOnSerialise; finally reset the selected shape to the end
// Grade: DIFFER(444v468) -- retail keeps three separate SelectedShape=end exits (loop end, empty vector, zero count) where ours merges two; the offset's zero stores use an integer zero register (edi) in retail; the two GetWorldMap results swap ebx/edi (register tie-break). Tried branch order (loading first fixed the layout), named map local, do/while explicit exits.
// Retail: Fable.exe @ 0x00762BA0, 480 bytes (`ret 8`, `and esp,-8` frame).
// Callees: CPersistContext::Transfer<bool> 0x004045C0 / <long> 0x00410BE0, CThing::GetPosition 0x004C73D0,
//   CTCBase::GetWorldMap 0x00686D20, CWorldMap virtual slots 16 (+0x40) and 23 (+0x5c), NShape::CShape virtual slot 2,
//   CTCShapeManager::StatOnSerialise 0x00762620 (__fastcall static), operator delete 0x00BFE9BC (counted-pointer block).
// Layout: Ego_r.pdb CTCShapeManager with the retail 12-byte STLport vector: PersistantShapes +0x0c, SelectedShape +0x18,
//   ClosedShapeBounds (C2DBoxF) +0x20 (as the landed InitialiseFrom 0x00762AB0 / EditUpdateDelete 0x00762D80).
//   CPersistContext::Mode +0x18 (1/3 = saving).
#include <stddef.h>
void operator delete(void* p);                      // 0x00BFE9BC
struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct C3DVector { float X, Y, Z; };
struct C2DBoxF { C2DVector TopLeft; C2DVector BottomRight; };
struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                             // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
};
struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetRegionAtPos(const C3DVector& pos);              // slot 16 (+0x40)
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual C2DVector GetRegionOffset(long region);                 // slot 23 (+0x5c)
};
namespace NShape {
struct CShape {
    virtual void v00();
    virtual void v01();
    virtual void Serialise(CPersistContext* context, long index, const C2DVector& offset);   // slot 2
};
}
struct CCPPointerInfo {
    long References;                                // +0
    void (__fastcall* PfnDestroy)(void*);           // +4
    void* Object;                                   // +8
};
struct CShapePointer {                              // CCountedPointer<NShape::CShape>
    NShape::CShape* Data;
    CCPPointerInfo* Info;
    CShapePointer(const CShapePointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
    ~CShapePointer()
    {
        if (Info && --Info->References == 0) {
            Info->PfnDestroy(Info->Object);
            delete Info;
        }
    }
    NShape::CShape* operator->() const { return Data; }
};
struct CTCShapeManager {
    virtual void v00();
    CThing*        ParentThing;                     // +0x04
    bool           ToBeRemoved;                     // +0x08
    bool           IsInGlobalUpdateTCs;             // +0x09
    unsigned char  _pad0a[2];
    CShapePointer* PersistantShapes_first;          // +0x0c
    CShapePointer* PersistantShapes_last;           // +0x10
    CShapePointer* PersistantShapes_end;            // +0x14
    CShapePointer* SelectedShape;                   // +0x18
    bool           Activated;                       // +0x1c
    bool           NewShapeActivated;               // +0x1d
    bool           LeavingShape;                    // +0x1e
    C2DBoxF        ClosedShapeBounds;               // +0x20
    CWorldMap* GetWorldMap();                       // 0x00686D20
    static void __fastcall StatOnSerialise(CTCShapeManager* manager, CPersistContext* context, long index,
                                           const C2DVector& offset, C2DBoxF& bounds);   // 0x00762620
    void OnSerialise(CPersistContext* context, long version);
};

void CTCShapeManager::OnSerialise(CPersistContext* context, long version)
{
    bool isCoordsRelativeToMap = true;
    if (context->IsSaving())
        context->Transfer("IsCoordsRelativeToMap", isCoordsRelativeToMap, false);
    else
        context->Transfer("IsCoordsRelativeToMap", isCoordsRelativeToMap, false);
    C2DVector offset(0.0f, 0.0f);
    if (isCoordsRelativeToMap)
    {
        C3DVector position = ParentThing->GetPosition();
        CWorldMap* worldMap = GetWorldMap();
        offset = GetWorldMap()->GetRegionOffset(worldMap->GetRegionAtPos(position));
    }
    long numShapes;
    if (context->IsSaving())
        numShapes = PersistantShapes_last - PersistantShapes_first;
    context->Transfer("NumShapes", numShapes, 0);
    if (!context->IsSaving())
    {
        ClosedShapeBounds.TopLeft.X = 1e10f;
        ClosedShapeBounds.TopLeft.Y = 1e10f;
        ClosedShapeBounds.BottomRight.X = -1e10f;
        ClosedShapeBounds.BottomRight.Y = -1e10f;
        for (long i = 0; i != numShapes; ++i)
            StatOnSerialise(this, context, i, offset, ClosedShapeBounds);
        SelectedShape = PersistantShapes_last;
        return;
    }
        long index = 0;
        for (CShapePointer* it = PersistantShapes_first; it != PersistantShapes_last; ++it)
        {
            CShapePointer shape = *it;
            shape->Serialise(context, index, offset);
            ++index;
        }
    SelectedShape = PersistantShapes_last;
}
