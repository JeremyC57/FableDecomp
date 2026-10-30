// CTCOwnedEntity::OnForceRecreateUID(C3DVector& out) -- (catalog name; the body picks a spawn point) gather the valid positions within 11 of the target thing, drop those within 10 of it, and return a random survivor; false when there is no live target or no position
// Grade: DIFFER(359v359) nd=10 (was 121) -- levers: C3DVector with the engine memberwise copy ctor + Pos(pos) member init in the predicate, a named C3DVector pos copy of the target position (gives retail's fld X/fld Y/dword Z build and Z frame park), and __forceinline GFRandom (the heavier TU otherwise calls it out of line). Tried: copy ctor alone (98), pos local alone (131), Random(range) split form (10), IsDead (10), named index (10). Residue: retail frame 0x1c vs 0x18 (one more dead 4-byte slot between the vector and pos). Next: find the dead slot (a by-ref temp in GetValidPositions' arguments?).
// Leaf: OnForceRecreateUID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00971ED0, 368 bytes (`ret 4`, bool in al).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x4); GetPWorld 0x008FCFC0 (x3; catalog "CPlayer::GetCurrentMode" is
//          propagated); the world's virtual slot 12 (+0x30, the world map) and search tools +0x20; GetValidPositions 0x0074A5D0
//          (__fastcall); CThing::GetPosition 0x004C73D0 (x2); std::remove_if<C3DVector*, CWithinDistance> 0x008AC060 (__fastcall,
//          predicate by value; names descriptive); CWorld random-seed accessor 0x0049D810; GFROR13 0x00497890 (__fastcall);
//          free 0x00BFEA14 (the STLport vector's storage).
// Layout: the target intelligent pointer at +0x24 (descriptive); the target's flag byte +0x91 (bit 0: dead/limbo).
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
long __fastcall GFROR13(unsigned long value);       // 0x00497890
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CWithinDistance {                            // remove_if predicate (descriptive)
    float     Distance;
    C3DVector Pos;
    CWithinDistance(float distance, const C3DVector& pos) : Distance(distance), Pos(pos) {}
};
C3DVector* __fastcall RemoveWithinDistance(C3DVector* first, C3DVector* last, CWithinDistance pred);   // 0x008AC060
struct CPositionVector {                            // STLport vector<C3DVector>
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CPositionVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CWorldMap;
struct CThingSearchTools;
struct CTCShapeManager;
void __fastcall GetValidPositions(CPositionVector& out, CWorldMap* map, CThingSearchTools* tools, const C3DVector& pos,
                                  float radius, bool flag, CTCShapeManager* shapes);   // 0x0074A5D0
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();               // slot 12 (+0x30)
    unsigned char      _pad04[0x20 - 0x04];
    CThingSearchTools* SearchTools;                 // +0x20
    unsigned long* GetRandomSeed();                 // 0x0049D810
};
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                        // +0x91
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CBaseIntelligentPointer {
    CThing* GetPItem() const;                       // 0x00A01B50
};
__forceinline unsigned long GFRandom(unsigned long* seed, unsigned long range)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
class CTCOwnedEntity {
public:
    virtual void v00();
    unsigned char           _pad04[0x24 - 0x04];
    CBaseIntelligentPointer Target;                 // +0x24
    CWorld* GetPWorld();                            // 0x008FCFC0
    bool OnForceRecreateUID(C3DVector& out);
};

bool CTCOwnedEntity::OnForceRecreateUID(C3DVector& out)
{
    if (!Target.GetPItem() || (Target.GetPItem()->m_flags91 & 1))
        return false;
    CPositionVector positions;
    CWorld* world = GetPWorld();
    CThingSearchTools* tools = GetPWorld()->SearchTools;
    GetValidPositions(positions, world->GetWorldMap(), tools, Target.GetPItem()->GetPosition(), 11.0f, false, 0);
    if (positions.size() == 0)
        return false;
    C3DVector pos = Target.GetPItem()->GetPosition();
    unsigned long count = RemoveWithinDistance(positions._M_start, positions._M_finish,
                                               CWithinDistance(10.0f, pos)) - positions._M_start;
    out = positions._M_start[GFRandom(GetPWorld()->GetRandomSeed(), count)];
    return true;
}
