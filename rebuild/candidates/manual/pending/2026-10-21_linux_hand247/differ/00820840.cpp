// Grade: DIFFER(372v372) -- frame and spill of `this` match; left: eax/edx naming of the vptr/argument temporaries around the virtual calls (the known vptr-register class) and retail's `add eax,0Ch` copy of the transform's second vector (ours folds the offset)
// CMap::ScriptedMapBrushMoveThings(const CScriptedMapBrush&) -- move every live thing standing on a cell the scripted brush covers to the brush's world position and orientation for that cell
// Leaf: ScriptedMapBrushMoveThings
// Callees: node malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list header and nodes), CScriptedMapBrush cell walk 0x0082E860 (labelled LinkedList_BuildFromTreeIterator) and per-cell transform 0x0082E4E0 (labelled CSurface::GetHeight), the map's cell source slots 3 / 13, the world-map index slot 17, thing holder 0x0051ED80 (labelled CStopWatch::GetTicks), CWorldMap::ConvertLocalCoordsToWorldMapCoords 0x004FC4A0, thing physics slots 31, 66, 67, 68.
// Layout: no CMap engine header; local model (cell source +0x20, map name +0x1D24).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C2DCoordI { long X, Y; };
void* __cdecl NodeAlloc(unsigned int size);     // 0x00BFEA0E
void __cdecl NodeFree(void* p);                 // 0x00BFEA14
struct SCoordNode {
    SCoordNode* Next;
    SCoordNode* Prev;
    C2DCoordI   Data;
};
struct CCoordList {                             // STLport list<C2DCoordI>: one pointer to the allocated header node
    SCoordNode* Node;
    CCoordList() : Node(0)
    {
        Node = (SCoordNode*)NodeAlloc(sizeof(SCoordNode));
        Node->Next = Node;
        Node->Prev = Node;
    }
    ~CCoordList()
    {
        clear();
        NodeFree(Node);
    }
    void clear()
    {
        SCoordNode* cur = Node->Next;
        while (cur != Node) {
            SCoordNode* tmp = cur;
            cur = cur->Next;
            NodeFree(tmp);
        }
        Node->Next = Node;
        Node->Prev = Node;
    }
};
struct STransform {
    C3DVector Position;
    C3DVector Angles;
    STransform(const STransform& o) : Position(o.Position), Angles(o.Angles) { }
};
struct CScriptedMapBrush {
    void GetCells(CCoordList& cells) const;                     // 0x0082E860
    const STransform& GetTransform(long x, long y) const;       // 0x0082E4E0
};
struct CPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void SetPosition(const C3DVector& position);       // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65();
    virtual void SetAngleX(float angle);       // slot 66 (+0x108)
    virtual void SetAngleY(float angle);       // slot 67 (+0x10c)
    virtual void SetAngleZ(float angle);       // slot 68 (+0x110)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysics*     Physics;                      // +0x60
    unsigned char _pad64[0x2d];
    unsigned char Flags91;                      // +0x91 bit 0: dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
struct CThingHolder {
    CThing* Get();                              // 0x0051ED80
};
struct CWorldMap {
    C3DVector ConvertLocalCoordsToWorldMapCoords(long map, const C3DVector& local);   // 0x004FC4A0
};
struct CMapCellSource {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingHolder* GetThingAt(long x, long y);   // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();           // slot 13 (+0x34)
};
struct CWorldMapIndex {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual long GetMapIndex(void* name);       // slot 17 (+0x44)
};
struct CMap {
    virtual void v00();
    unsigned char   _pad04[0x1c];
    CMapCellSource* Cells;                      // +0x20
    unsigned char   _pad24[0x1d00];
    unsigned char   Name[4];                    // +0x1d24
    void ScriptedMapBrushMoveThings(const CScriptedMapBrush& brush);
};

void CMap::ScriptedMapBrushMoveThings(const CScriptedMapBrush& brush)
{
    CCoordList cells;
    brush.GetCells(cells);
    long map = ((CWorldMapIndex*)Cells->GetWorldMap())->GetMapIndex(Name);
    for (SCoordNode* node = cells.Node->Next; node != cells.Node; node = node->Next) {
        CThing* thing = Cells->GetThingAt(node->Data.X, node->Data.Y)->Get();
        if (thing && !thing->IsDead()) {
            STransform transform = brush.GetTransform(node->Data.X, node->Data.Y);
            CPhysics* physics = thing->Physics;
            physics->SetPosition(Cells->GetWorldMap()->ConvertLocalCoordsToWorldMapCoords(map, transform.Position));
            thing->Physics->SetAngleX(transform.Angles.X);
            thing->Physics->SetAngleY(transform.Angles.Y);
            thing->Physics->SetAngleZ(transform.Angles.Z);
        }
    }
}
