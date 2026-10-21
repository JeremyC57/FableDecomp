// CWorldMap::IsPosChangeable(const C2DCoordI&) const -- whether the map containing a coordinate may be changed
// Leaf: IsPosChangeable@CWorldMap
// Retail: Fable.exe @ 0x004FB2F0, 25 bytes, `ret 4`; Ego_r.pdb ?IsPosChangeable@CWorldMap@@UBE_NABVC2DCoordI@@@Z, CWorldMap vtable
// 0x01244AEC slot 25. Calls own virtual slot 17 (+0x44, ?GetMapNumberAt@CWorldMap@@UBEJABVC2DCoordI@@@Z) and slot 24 (+0x60,
// ?IsMapChangeable@CWorldMap@@UBE_NJ@Z) through one vptr load. Slot numbers read from the retail vtable; the placeholder
// slots stand in for the other virtuals.
class C2DCoordI { public: long X, Y; };
class CWorldMap {
public:
    virtual void _s00();
    virtual void _s01();
    virtual void _s02();
    virtual void _s03();
    virtual void _s04();
    virtual void _s05();
    virtual void _s06();
    virtual void _s07();
    virtual void _s08();
    virtual void _s09();
    virtual void _s10();
    virtual void _s11();
    virtual void _s12();
    virtual void _s13();
    virtual void _s14();
    virtual void _s15();
    virtual void _s16();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;      // slot 17 (+0x44)
    virtual void _s18();
    virtual void _s19();
    virtual void _s20();
    virtual void _s21();
    virtual void _s22();
    virtual void _s23();
    virtual bool IsMapChangeable(long mapNumber) const;           // slot 24 (+0x60)
    virtual bool IsPosChangeable(const C2DCoordI& pos) const;         // slot 25 (+0x64)
};
bool CWorldMap::IsPosChangeable(const C2DCoordI& pos) const
{
    return IsMapChangeable(GetMapNumberAt(pos));
}