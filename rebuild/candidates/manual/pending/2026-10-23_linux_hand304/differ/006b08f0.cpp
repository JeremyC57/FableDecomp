// Grade: DIFFER(564v565) -- 27 difflines: marker LengthSquared loads X,Y,Z (retail Z,Y,X with X last through the +0x141 base), MapNumber load scheduled before `mov ebx,eax`, final Position=marker copy register (eax v ecx) and ParentThing loaded before the push. Tried by-value/ref/ctor getters, copy-ctor forms, operand orders, named region locals, named thing local.
// CTCPhysicsBase::OnSerialise(CPersistContext& context, bool) -- persist the position as PositionX/Y/Z relative to the origin of the map it is in; on load restore it (re-adding the map origin for serialise versions above 0), and for a thing flagged 0x20 whose save-game marker is set and lies in the same region, move it to the marker and update its map number
// Leaf: ?OnSerialise@CTCPhysicsBase@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B08F0, 565 bytes, `ret 8`.
// Callees: CTCBase::GetWorldMap 0x00686D20, CTCBase::GetWorld 0x00686A80, CTCBase::GetSerialiseVersion 0x00686A90,
//   CPersistContext::Transfer(const char*, float&, const float&) 0x00410620, CWorldMap::GetRegionNumberMapIsIn 0x004FC190,
//   CThing::SetCurrentMapNumber 0x004C7580; world-map virtual slots 16 (+0x40, the map number at a position, as the landed
//   CTCPhysicsLight::SetPosition 0x007233B0) and 23 (+0x5c, GetMapPos, C2DCoordI by hidden pointer). Strings "PositionX/Y/Z"
//   0x0125D460/54/48; g_FloatEpsilon 0x0129BA3C.
// Layout: local model as the landed sibling 0x006B0460 (engine/CTCPhysicsBase.h flattens Position into floats).
//   CPersistContext mode +0x18 (1/3 = saving); retail CThing ThingTypeFlags +0x6c [PDB +0x7c], map number +0x8e (short)
//   [PDB +0x9e]; retail CWorld SaveGameMarkerPos +0x141 (unaligned) [PDB +0x151].
class CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    float LengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
extern const float g_FloatEpsilon;                  // 0x0129BA3C

struct CPersistContext {
    unsigned char _pad[0x18];
    long          Mode;                             // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
};

struct CWorldMap {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual long GetMapNumberAt(const C3DVector& pos);   // slot 16 (+0x40)
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual C2DCoordI GetMapPos(long map) const;         // slot 23 (+0x5c) 0x004FC690
    long GetRegionNumberMapIsIn(long map) const;         // 0x004FC190
};

#pragma pack(push, 1)
struct CWorld {
    unsigned char _pad[0x141];
    C3DVector     SaveGameMarkerPos;                // +0x141
    C3DVector GetSaveGameMarkerPos() const { return SaveGameMarkerPos; }
};
#pragma pack(pop)

class CThing {
public:
    unsigned char  _pad0[0x6c];
    unsigned long  ThingTypeFlags;                  // +0x6c
    unsigned char  _pad70[0x8e - 0x70];
    short          MapNumber;                       // +0x8e
    void SetCurrentMapNumber(unsigned short mapNumber);   // 0x004C7580
};

class CTCPhysicsBase {
public:
    virtual void v000();
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[4];
    C3DVector     Position;                         // +0x0c
    const CWorld& GetWorld() const;                 // 0x00686A80 (CTCBase)
    long GetSerialiseVersion() const;               // 0x00686A90 (CTCBase)
    CWorldMap& GetWorldMap();                       // 0x00686D20 (CTCBase)
    virtual void OnSerialise(CPersistContext& context, bool);
};

void CTCPhysicsBase::OnSerialise(CPersistContext& context, bool)
{
    C3DVector pos;
    if (context.IsSaving()) {
        pos = Position;
        C2DCoordI origin = GetWorldMap().GetMapPos(GetWorldMap().GetMapNumberAt(pos));
        pos.X -= origin.X;
        pos.Y -= origin.Y;
    }
    context.Transfer("PositionX", pos.X, 0.0f);
    context.Transfer("PositionY", pos.Y, 0.0f);
    context.Transfer("PositionZ", pos.Z, 0.0f);
    if (!context.IsSaving()) {
        Position = pos;
        if (GetSerialiseVersion() > 0) {
            C2DCoordI origin = GetWorldMap().GetMapPos(ParentThing->MapNumber);
            Position.X += origin.X;
            Position.Y += origin.Y;
        }
        if (ParentThing->ThingTypeFlags & 0x20) {
            if (GetWorld().GetSaveGameMarkerPos().LengthSquared() > g_FloatEpsilon * g_FloatEpsilon) {
                C3DVector marker = GetWorld().SaveGameMarkerPos;
                if (GetWorldMap().GetRegionNumberMapIsIn(GetWorldMap().GetMapNumberAt(marker)) ==
                    GetWorldMap().GetRegionNumberMapIsIn(ParentThing->MapNumber)) {
                    Position = marker;
                    ParentThing->SetCurrentMapNumber(GetWorldMap().GetMapNumberAt(Position));
                }
            }
        }
    }
}
