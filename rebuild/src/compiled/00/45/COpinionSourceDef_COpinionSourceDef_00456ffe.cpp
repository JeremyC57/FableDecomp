// COpinionSourceDef::COpinionSourceDef() -- the CParentDefClassBase base, this def's vtable, then its three class-type members' constructors
// Leaf: ??0COpinionSourceDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456FFE, 42 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x01233FF4 (RTTI .?AVCOpinionSourceDef@@ : CParentDefClassBase : CDefClassBase : ...); FableWin.pdb / Ego_r.pdb
//   ??0COpinionSourceDef@@QAE@XZ; the constructed members are the Ego_r.pdb COpinionSourceDef members at their retail offsets (retail = Ego_r.pdb - 4 .. - 0x10 (12-byte STLport maps)).
// Callees: CParentDefClassBase::CParentDefClassBase() 0x00430370, map<EOpinion, float>() 0x004534C4 (ProducedOpinion +0x3C), map<EOpinionReactionType, bool>() 0x00453447 (ReactionFlag +0x50), COpinionSet::COpinionSet() 0x004538EB (OpinionProduced +0x6C).
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
enum EOpinion { };
enum EOpinionReactionType { };
class COpinionSet {
public:
    COpinionSet();                                       // 0x004538EB
    float* OpinionValues[3];                             // CArray<float> (12 bytes in retail)
};
class CParentDefClassBase {                                            // retail 0x3C bytes (members not touched here)
public:
    CParentDefClassBase();                                             // 0x00430370
    virtual ~CParentDefClassBase();
    unsigned char _body[0x3C - 4];
};
class COpinionSourceDef : public CParentDefClassBase {
public:
    COpinionSourceDef();
    std::map<EOpinion, float>                 ProducedOpinion;                     // +0x3C
    bool                                      AlwaysAwareOf;                       // +0x48
    bool                                      UnknownThreatSource;                 // +0x49
    bool                                      ReactNonstop;                        // +0x4A
    bool                                      ReactToSelfPermitted;                // +0x4B
    bool                                      ReactionFlagDefault;                 // +0x4C
    std::map<EOpinionReactionType, bool>      ReactionFlag;                        // +0x50
    bool                                      UseNavLineOfSight;                   // +0x5C
    bool                                      WitnessesWillNotAttack;              // +0x5D
    bool                                      WitnessesWillOverrideFactionEnemy;   // +0x5E
    bool                                      BypassesPersonality;                 // +0x5F
    bool                                      RelativeEffect;                      // +0x60
    long                                      RestrictToFactionAndAllies;          // +0x64
    bool                                      RestrictToFactionIsReversed;         // +0x68
    COpinionSet                               OpinionProduced;                     // +0x6C
};
COpinionSourceDef::COpinionSourceDef()
{
}