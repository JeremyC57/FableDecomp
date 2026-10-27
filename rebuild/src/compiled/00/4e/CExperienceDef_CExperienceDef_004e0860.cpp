// CExperienceDef::CExperienceDef() -- the CSubDefClassBase base, this def's vtable, then its three class-type members' constructors
// Leaf: ??0CExperienceDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0860, 42 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x01242104 (RTTI .?AVCExperienceDef@@ : CSubDefClassBase : CDefClassBase : ...); FableWin.pdb / Ego_r.pdb
//   ??0CExperienceDef@@QAE@XZ; the constructed members are the Ego_r.pdb CExperienceDef members at their retail offsets (retail = Ego_r.pdb .. - 8).
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0, map<long, float>() 0x004DF956 (ParticleScalesPerExperiencePoints +0x2C, LifeTimeForDistanceThresholds +0x44), map<long, long>() 0x004DF86C (PickupParticleEffectsPerExperiencePoints +0x38).
// Shape (family K090, representative 0x0044C3AA): the implicit-style default constructor: base call, vtable store, the
//   members' out-of-line default constructors in declaration order; the POD members stay uninitialised.
namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                              // STLport 4.6, 12 bytes (header, count, comparator)
public:
    map();                                               // out of line (this map's own COMDAT)
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
class CSubDefClassBase {                                            // retail 0x28 bytes (members not touched here)
public:
    CSubDefClassBase();                                             // 0x0044C0C0
    virtual ~CSubDefClassBase();
    unsigned char _body[0x28 - 4];
};
class CExperienceDef : public CSubDefClassBase {
public:
    CExperienceDef();
    long                                      OrbCombiningParticleEffect;          // +0x28
    std::map<long, float>                     ParticleScalesPerExperiencePoints;   // +0x2C
    std::map<long, long>                      PickupParticleEffectsPerExperiencePoints; // +0x38
    std::map<long, float>                     LifeTimeForDistanceThresholds;       // +0x44
};
CExperienceDef::CExperienceDef()
{
}