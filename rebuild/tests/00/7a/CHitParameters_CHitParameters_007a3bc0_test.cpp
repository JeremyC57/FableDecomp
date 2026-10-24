#include <cstdio>
#include <cstdlib>
#include <new>

// Model: the default CHitParameters -- flag defaults, an empty STLport damage map (header node from malloc, linked to
// itself), an empty augmentation vector and null hit location / material / attacker / weapon pointers.
class CThing;
class CHitLocation;
class CMaterialDef;

#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
};
#pragma pack(pop)

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

struct CDamageMap {
    struct _Node : _Rb_tree_node_base {
        long  Attribute;
        float Damage;
    };
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    unsigned int        _M_key_compare;

    CDamageMap() : _M_header(0)
    {
        _M_header = (_Node*)std::malloc(sizeof(_Node));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct CAugmentationVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CAugmentationVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

static int g_pointersBuilt = 0;

struct CBaseIntelligentPointer {
    CBaseIntelligentPointer() : m_pItem(0) { ++g_pointersBuilt; }
    virtual ~CBaseIntelligentPointer() { }
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
    bool                        Localised;
    bool                        Directional;
    C3DVector                   Position;
    C3DVector                   Direction;
    bool                        KnockDown;
    bool                        Decapitate;
    bool                        Blockable;
    bool                        Flourish;
    bool                        EpicSpell;
    bool                        BlockCounter;
    bool                        PlayHitResponse;
    bool                        PlayHitResponseOverrideSet;
    bool                        MoveBack;
    bool                        CreateParticleEffectOnHit;
    bool                        CreateDustParticleEffectOnHit;
    bool                        GuaranteeHit;
    bool                        Blocked;
    bool                        HitNegated;
    bool                        CauseRecoil;
    float                       ShotBuildupTime;
    CDamageMap                  DamageMap;
    CAugmentationVector         Augmentations;
    CHitLocation*               PHitLocation;
    CDefPointer<CMaterialDef>   PHitMaterialDef;
    CIntelligentPointer<CThing> PThingHitting;
    CIntelligentPointer<CThing> PWeapon;

    CHitParameters();
};

CHitParameters::CHitParameters()
    : Localised(false), Directional(false), KnockDown(false), Decapitate(false), Blockable(true), Flourish(false),
      EpicSpell(false), BlockCounter(false), PlayHitResponse(true), PlayHitResponseOverrideSet(false), MoveBack(true),
      CreateParticleEffectOnHit(true), CreateDustParticleEffectOnHit(true), GuaranteeHit(false), Blocked(false),
      HitNegated(false), CauseRecoil(false), ShotBuildupTime(0.0f), PHitLocation(0)
{
}

int main()
{
    void* raw = std::malloc(sizeof(CHitParameters));
    unsigned char* bytes = (unsigned char*)raw;
    for (unsigned int i = 0; i < sizeof(CHitParameters); ++i)
        bytes[i] = 0xAB;                                 // flags must be written, not left from the heap
    CHitParameters* p = new (raw) CHitParameters;

    bool ok = !p->Localised && !p->Directional && !p->KnockDown && !p->Decapitate && p->Blockable && !p->Flourish
        && !p->EpicSpell && !p->BlockCounter && p->PlayHitResponse && !p->PlayHitResponseOverrideSet && p->MoveBack
        && p->CreateParticleEffectOnHit && p->CreateDustParticleEffectOnHit && !p->GuaranteeHit && !p->Blocked
        && !p->HitNegated && !p->CauseRecoil && p->ShotBuildupTime == 0.0f;
    _Rb_tree_node_base* header = p->DamageMap._M_header;
    ok = ok && header != 0 && p->DamageMap._M_node_count == 0 && header->_M_color == 0 && header->_M_parent == 0
        && header->_M_left == header && header->_M_right == header;
    ok = ok && p->Augmentations._M_start == 0 && p->Augmentations._M_finish == 0
        && p->Augmentations._M_end_of_storage == 0;
    ok = ok && p->PHitLocation == 0 && p->PHitMaterialDef.Object == 0 && p->PThingHitting.m_pItem == 0
        && p->PWeapon.m_pItem == 0 && g_pointersBuilt == 2;
    ok = ok && (unsigned char*)&p->Position - (unsigned char*)p == 2 && (unsigned char*)&p->KnockDown - (unsigned char*)p == 0x1a;

    std::free(header);
    p->~CHitParameters();
    std::free(raw);

    if (!ok) {
        std::printf("007a3bc0_TEST FAIL\n");
        return 1;
    }
    std::printf("007a3bc0_TEST PASS\n");
    return 0;
}