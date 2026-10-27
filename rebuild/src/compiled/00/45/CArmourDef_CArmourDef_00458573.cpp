// CArmourDef::CArmourDef() -- the CParentDefClassBase base, this def's vtable, then its three class-type members' constructors
// Leaf: ??0CArmourDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00458573, 42 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x01234AEC (RTTI .?AVCArmourDef@@ : CParentDefClassBase : CDefClassBase : ...); FableWin.pdb / Ego_r.pdb
//   ??0CArmourDef@@QAE@XZ; the constructed members are the Ego_r.pdb CArmourDef members at their retail offsets (retail = Ego_r.pdb - 4 .. - 0xC).
// Callees: CParentDefClassBase::CParentDefClassBase() 0x00430370, map<EObjectAugmentationType, float>() 0x00453536 (AugmentationResponse +0x3C), map<EDamageAttribute, float>() 0x00453554 (DamageTypeResponse +0x48, ArmourThresholds +0x54).
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
enum EObjectAugmentationType { };
enum EDamageAttribute { };
class CParentDefClassBase {                                            // retail 0x3C bytes (members not touched here)
public:
    CParentDefClassBase();                                             // 0x00430370
    virtual ~CParentDefClassBase();
    unsigned char _body[0x3C - 4];
};
class CArmourDef : public CParentDefClassBase {
public:
    CArmourDef();
    std::map<EObjectAugmentationType, float>  AugmentationResponse;                // +0x3C
    std::map<EDamageAttribute, float>         DamageTypeResponse;                  // +0x48
    std::map<EDamageAttribute, float>         ArmourThresholds;                    // +0x54
    bool                                      AllHitsKnockdown;                    // +0x60
    bool                                      AllHitsNegated;                      // +0x61
    bool                                      AllHitsCauseRecoil;                  // +0x62
    long                                      DamageMaterial;                      // +0x64
    long                                      BlockMaterial;                       // +0x68
};
CArmourDef::CArmourDef()
{
}