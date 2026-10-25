// CTCDExplosion::SetSpecificThingToHit(CThing&) -- appends the thing to the explosion's specified-things-to-hit list
// Leaf: ?SetSpecificThingToHit@CTCDExplosion@@QAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080CCF0, 66 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: CTCDExplosion::SetSpecificThingToHit(CThing&) -- proven; evidence: it sits in the CTCDExplosion module (after SetSpecificThingsToHit 0x0080CC40, before UpdateHitThings
//   0x0080CD40); Ego_r.pdb CTCDExplosion::SpecifiedThingsToHit is the last vector<CIntelligentPointer<CThing> > (retail +0x6C);
//   caller CThingGameObject::OnHit 0x00839940;
//   FableWin.pdb name set holds ?SetSpecificThingToHit@CTCDExplosion@@QAEXAAVCThing@@@Z.
//   Family G039 (8 bodies: one temporary intelligent pointer handed to a list/vector member; the propagated label
//   "CCreatureActionBase::AddDependentThing" is wrong for all eight -- that member sits at +0x9C in retail).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90, std::vector<CIntelligentPointer<CThing> >::push_back 0x004AD520, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10.
//   Vtable: CIntelligentPointer<CThing> 0x01238C6C.
// Shape: the temporary's inline (T*) constructor (base ctor, own vtable, SetPItem) and its implicit destructor (a direct
//   call of the base destructor, no vtable reset); no EH frame (the module is built without /GX).
// Model: local -- retail STLport lists are 4 bytes and vectors 12 (the generated engine headers carry Ego_r's 12/16-byte
//   Dinkumware containers and no member functions), so the class is laid out here at its retail offsets.
class CBaseObject {
public:
    virtual ~CBaseObject();
};
class CThing : public CBaseObject { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void remove(const _Tp& __val);                           // out of line
    void* _M_node;                                           // _STLP_alloc_proxy<_List_node<_Tp>*, ...>
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    void push_back(const _Tp& __x);                          // out of line
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;                                  // _STLP_alloc_proxy<_Tp*, ...>
};
}
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                     // +0x04
    bool    ToBeRemoved;                                     // +0x08
    bool    IsInGlobalUpdateTCs;                             // +0x09
};
struct C3DVector { float X, Y, Z; };
class CExplosionDef;
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
class CTCDExplosion : public CTCBase {
public:
    void SetThingAsImmune(CThing& thing);
    void SetSpecificThingToHit(CThing& thing);
    float     DamageRadius;                                  // +0x0c
    float     PhysicsRadius;                                 // +0x10
    float     MaxDamage;                                     // +0x14
    long      FireDamage;                                    // +0x18
    long      ExplosionLifeTimer;                            // +0x1c
    C3DVector StartPos;                                      // +0x20
    C3DVector EndPos;                                        // +0x2c
    const CExplosionDef* PExplosionDef;                      // +0x38 CDefPointer<const CExplosionDef>
    bool      CreatedEmitter;                                // +0x3c
    bool      NeedToTriggerExplosion;                        // +0x3d
    std::vector<std::pair<CIntelligentPointer<CThing>, long> > Emitters;   // +0x40
    long      ExplosionEvolutionTimer;                       // +0x4c
    long      CurrentExplosionRing;                          // +0x50
    std::vector<CIntelligentPointer<CThing> > HitThings;     // +0x54
    std::vector<CIntelligentPointer<CThing> > ImmuneThings;  // +0x60
    std::vector<CIntelligentPointer<CThing> > SpecifiedThingsToHit;   // +0x6c
};

void CTCDExplosion::SetSpecificThingToHit(CThing& thing)
{
    SpecifiedThingsToHit.push_back(CIntelligentPointer<CThing>(&thing));
}