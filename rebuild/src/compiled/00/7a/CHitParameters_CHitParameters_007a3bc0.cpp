// CHitParameters::CHitParameters() -- default hit parameters: undirected, blockable, plays the hit response, moves the target back and spawns both hit effects; empty damage map and augmentation list, no hit location, material, attacker or weapon
// Leaf: ??0CHitParameters@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A3BC0, 154 bytes (+ int3 padding to 160), __fastcall, plain `ret`.
// Identity: Ego_r.pdb `??0CHitParameters@@QAE@XZ` sits straight after CTCWillLightning's ??_G, as this code follows the
//   retail lightning block; the member stores are the PDB CHitParameters layout (the landed 3-argument ctor 0x0087FF60
//   has the same field order with Blockable false). 13 callers (hit builders for spells, melee and projectiles). The
//   catalog's "CWound_Constructor" is wrong (Ego_r CWound is 68 bytes: CEntry, Duration, decal ids).
// Callees: malloc 0x00BFEA0E (the 0x18-byte map header node), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10
//   (x2; then the CIntelligentPointer<CThing> vtable 0x01238C6C).
// Layout: Ego_r.pdb CHitParameters with the retail 12-byte STLport map: Localised +0, Directional +1, Position +2 / Direction
//   +0x0e (pack(1) C3DVectors, left uninitialised), KnockDown +0x1a .. CauseRecoil +0x28, ShotBuildupTime +0x2c, DamageMap
//   +0x30 (map<EDamageAttribute, float>: header, count, comparator), Augmentations +0x3c (STLport vector: start, finish,
//   end of storage), PHitLocation +0x48, PHitMaterialDef +0x4c (CDefPointer, null), PThingHitting +0x50, PWeapon +0x58.
// Shape: member-initialiser list in declaration order with one zero register (ebx) and al = 1 for the true flags;
//   ShotBuildupTime 0.0f stored from ebx; the inlined STLport map ctor (`_M_header(0)`, malloc, count 0, colour /
//   parent / left / right through re-reads of the header word) and vector ctor; the two intelligent pointers default.
// Label correction: catalogued as `CWound_Constructor`; the body is CHitParameters::CHitParameters().
extern "C" void* __cdecl malloc(unsigned int size);      // 0x00BFEA0E

class CThing;
class CHitLocation;
class CMaterialDef;

#pragma pack(push, 1)
struct C3DVector {                                       // pack(1) in the engine (engine/C3DVector); no default initialisation
    float X, Y, Z;
};
#pragma pack(pop)

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

struct CDamageMap {                                      // STLport map<EDamageAttribute, float>, 12 bytes in retail
    struct _Node : _Rb_tree_node_base {
        long  Attribute;                                 // +0x10
        float Damage;                                    // +0x14 (node 0x18 bytes)
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;                  // empty comparator, padded

    CDamageMap() : _M_header(0)
    {
        _M_header = (_Node*)malloc(sizeof(_Node));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct CAugmentationVector {                             // STLport vector<EObjectAugmentationType>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CAugmentationVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void* m_pItem;
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
};

template <class T> struct CDefPointer {
    const T* Object;
    CDefPointer() : Object(0) { }
};

class CHitParameters {
public:
    bool                        Localised;               // +0x00
    bool                        Directional;             // +0x01
    C3DVector                   Position;                // +0x02
    C3DVector                   Direction;               // +0x0e
    bool                        KnockDown;               // +0x1a
    bool                        Decapitate;              // +0x1b
    bool                        Blockable;               // +0x1c
    bool                        Flourish;                // +0x1d
    bool                        EpicSpell;               // +0x1e
    bool                        BlockCounter;            // +0x1f
    bool                        PlayHitResponse;         // +0x20
    bool                        PlayHitResponseOverrideSet;   // +0x21
    bool                        MoveBack;                // +0x22
    bool                        CreateParticleEffectOnHit;    // +0x23
    bool                        CreateDustParticleEffectOnHit;   // +0x24
    bool                        GuaranteeHit;            // +0x25
    bool                        Blocked;                 // +0x26
    bool                        HitNegated;              // +0x27
    bool                        CauseRecoil;             // +0x28
    float                       ShotBuildupTime;         // +0x2c
    CDamageMap                  DamageMap;               // +0x30
    CAugmentationVector         Augmentations;           // +0x3c
    CHitLocation*               PHitLocation;            // +0x48
    CDefPointer<CMaterialDef>   PHitMaterialDef;         // +0x4c
    CIntelligentPointer<CThing> PThingHitting;           // +0x50
    CIntelligentPointer<CThing> PWeapon;                 // +0x58

    CHitParameters();
};

CHitParameters::CHitParameters()
    : Localised(false), Directional(false), KnockDown(false), Decapitate(false), Blockable(true), Flourish(false),
      EpicSpell(false), BlockCounter(false), PlayHitResponse(true), PlayHitResponseOverrideSet(false), MoveBack(true),
      CreateParticleEffectOnHit(true), CreateDustParticleEffectOnHit(true), GuaranteeHit(false), Blocked(false),
      HitNegated(false), CauseRecoil(false), ShotBuildupTime(0.0f), PHitLocation(0)
{
}