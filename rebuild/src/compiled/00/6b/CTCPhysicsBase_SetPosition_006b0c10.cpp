// CTCPhysicsBase::SetPosition(const C3DVector& pos) -- move the body: when it is in the map-who, refuse a move into another map that is not loaded (reporting a collision of kind 4), else move its map-who entry; then store the position and the thing's map number
// Leaf: SetPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0C10, 484 bytes (+int3), `ret 4`, esp frame (speed TU).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inline GetInterface 0x3A), CTCBase::GetWorld @ 0x00686A70 + slot 12 (inline
//   GetWorldMap; out-of-line copy 0x006AF620), the physics slots 28 (+0x70, GetOffset2D) / 96 (+0x180, UpdateOldPosition),
//   CWorldMap slots 16 (+0x40, GetMapNumber) / 10 (+0x28, IsMapLoaded), CThing slot 41 (+0xA4, OnCollision),
//   CTCMapwho::MoveInMapwho @ 0x0071A750, CThing::UpdateGraphicAppearance @ 0x004C7590, CThing::SetCurrentMapNumber @ 0x004C7580.
// Shape: a same-TU auto_inline(off) stand-in body for LowerBound (the 008330d0 idiom) lets VC7.1 share the lookup key word with
//   the zero-initialised mapwho out pointer; the final SetCurrentMapNumber goes through an inline GetParentThing() (retail
//   mov esi,[esi+4] / mov ecx,esi). Local CTCPhysicsBase model (the engine header has no virtuals).
class C2DVector { public:
    float X, Y;
    C2DVector() { }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector OffsetBy2D(const C2DVector& o) const { C3DVector result; result.X = X + o.X; result.Y = Y + o.Y; result.Z = Z; return result; }
};
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte entries): VC7.1 then knows
// the key's address does not escape and can share its frame word.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3)
    virtual void v40();
    virtual void OnCollision(long collideType, const C3DVector* position, const C3DVector* normal, CThing* other);   // slot 41 (+0xa4)
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    void SetCurrentMapNumber(long map);                 // 0x004C7580
    void UpdateGraphicAppearance(long reason);          // 0x004C7590
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCMapwho { public:                                      // interface 0x3a
    unsigned char _pad00[0x50];
    bool          InMapwho : 1;                         // +0x50 bit 0 (name descriptive)
    void MoveInMapwho(const C3DVector& pos);            // 0x0071A750
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;           // slot 10 (+0x28)
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumber(const C3DVector& pos);    // slot 16 (+0x40)
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                   // slot 12 (+0x30)
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1)
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual C2DVector GetOffset2D();                    // slot 28 (+0x70)
    virtual void v29();
    FILL10(3) FILL10(4) FILL10(5) FILL10(6) FILL10(7) FILL10(8)
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95();
    virtual void UpdateOldPosition();                   // slot 96 (+0x180) (name descriptive)
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x0c - 0x08];
    C3DVector     Position;                             // +0x0c
    CThing* GetParentThing() { return ParentThing; }
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    CWorldMap* GetWorldMap() { return GetWorld()->GetWorldMap(); }   // 0x006AF620 out of line
    C3DVector GetMapwhoPosition() { return Position.OffsetBy2D(GetOffset2D()); }   // 0x006AFCB0 out of line
    void SetPosition(const C3DVector& pos);
};

void CTCPhysicsBase::SetPosition(const C3DVector& pos)
{
    CTCMapwho* mapwho = 0;
    ParentThing->GetInterface(&mapwho, 0x3a);
    if (mapwho->InMapwho) {
        long oldMap = GetWorldMap()->GetMapNumber(GetMapwhoPosition());
        long newMap = GetWorldMap()->GetMapNumber(pos.OffsetBy2D(GetOffset2D()));
        if (oldMap != newMap) {
            if (newMap < 1 || !GetWorldMap()->IsMapLoaded(newMap)) {
                ParentThing->OnCollision(4, &Position, &C3DVector(0.0f, 0.0f, 1.0f), 0);
                return;
            }
        }
        mapwho->MoveInMapwho(pos.OffsetBy2D(GetOffset2D()));
        ParentThing->UpdateGraphicAppearance(1);
    }
    UpdateOldPosition();
    Position = pos;
    GetParentThing()->SetCurrentMapNumber(GetWorldMap()->GetMapNumber(Position));
}