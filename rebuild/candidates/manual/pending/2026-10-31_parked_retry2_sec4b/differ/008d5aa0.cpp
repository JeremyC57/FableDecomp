// CCreatureAction_FireMissileWeapon::PerformAction() -- release the loaded missile: record the frame, unload the creature's
//   primary-slot projectile weapon, make SIMPLE_NOISE, take the missile / weapon / shot def from ReleaseMissile, then fire
//   one shot (the released missile first, then new shots from the def) per multi-arrow volley shot: multi-arrow shots get
//   a CTCShotDefinitionMultiArrow re-initialised from the shot's def (the first flagged), Decapitate is passed on, and the
//   volley's shots are made not solid to each other; finally reset the creature's projectile-weapon state
// Grade: DIFFER(1115v1126) 66 difflines (wave 2 sec4b: first reconstruction from the disassembly; was the empty skeleton)
// Tried (graded): g1 straight draft 1094v1126/119; g2 the bitset::test GetInterface form (`shr; and cl,1; mov [esp+13h],cl`
//   for interfaces 0x46/0x48/0x88 and the first 0x77) + nested-if IS_THING_ALIVE 1115/68; g3 the LowerBound key as a
//   temporary (shares the 0x30 slot as retail) 66; no gain: three declaration orders of missile / weapon / def index (66).
// Residue: frame 0x44 vs 0x40 (the multi-arrow lookup's key temporary keeps its own slot; retail shares 0x30 with every
//   other key and the AddTC result); retail orders the ReleaseMissile outs weapon 0x14 < def index 0x1c < missile 0x20
//   (ours missile lowest); ebx/edi swapped from the top (&PCreature in edi, 0 in ebx in retail), so the CreateShot zero
//   pushes are registers in ours; the not-solid loop's begin/end copy (`mov ecx,esi; cmp ecx,eax`).
// Next: find the shape that scopes the multi-arrow lookup apart from the loop (inline helper / block), the vector loop
//   with an explicit end iterator.
// Leaf: ?PerformAction@CCreatureAction_FireMissileWeapon@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D5AA0, 1126 bytes (catalog length confirmed: `ret` at 0x008D5F05), plain `ret`.
// Layout: retail CCreatureActionBase 0xA8 (PCreature +0x08); FireMissileWeapon (Ego_r 0xD4 less 0x0C): PTarget +0xA8,
//   ShotAccuracyPercentage +0xB0, ShotDamageMultiplier +0xB4, ShotBuildupTime +0xB8, Decapitate +0xBC, DelayTimer +0xC0,
//   PSpecialShotDef +0xC4; CTCMultiArrow NumberOfShots +0x10 / NumberInVolley +0x14 (Ego_r); CTCShotDefinitionBase
//   PShotDef +0x38 (Ego_r +0x3C less the vector shrink), Decapitate +0x67, CanBeDeflected +0x68.
// Callees (Ego_r / FableWin spellings): CWorld::GetFrame 0x0049D870, CTCCarrying::GetThingInPrimarySlot 0x006A1440 (Ego_r
//   0x006C5CE0), CTCWeapon::SetProjectileWeaponLoaded 0x005DAC50, CThingCreatureBase::MakeNoise 0x00662110,
//   ReleaseMissile 0x008D5790 (PDB `AAEXPAPAVCThing@@0AAJ@Z`: private, long&), NThingShot::CreateShot 0x007C0AD0 (def index
//   form) / 0x007C0E80 (CThing& form), CThing::AddTC 0x004C9D60, CTCShotDefinitionBase::Init 0x007C0690 /
//   SetDecapitate 0x007BED40 / SetCanBeDeflected 0x007BED50 (Ego_r 0x007BDC10 / 0x007BDC20, consecutive as retail; the
//   0x67 / 0x68 members), CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, vector<pair<CThing*, CTCSolidToThing*> >::push_back
//   0x008D62B0, CTCMultiArrow::ReleaseShot 0x007CC710 (Ego_r 0x007CA9C0), ResetCreatureUseProjectileWeaponState 0x008D58D0.
// Globals: g_FireMissileWeaponFrame_013bafd0 0x013BAFD0 (written only, here; descriptive).
class CBaseObject { public: virtual void v00(); };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
class C3DVector;
class CRightHandedSet;
extern "C" void __cdecl free(void* block);               // 0x00BFEA14
namespace std {
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc&, _Value __p) : _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void push_back(const _Tp& __x);                       // <pair<CThing*, CTCSolidToThing*> > 0x008D62B0
};
}
class CTCBase;
enum ETCInterfaceType { TC_CARRYING = 0x46, TC_WEAPON = 0x48, TC_SOLID_TO_THING = 0x4d, TC_SHOT_DEFINITION = 0x77,
                        TC_MULTI_ARROW = 0x88 };          // enumerator names unproven
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1
    mutable long IVRefCount;                             // +0x04
    void AddRef() const { ++IVRefCount; }
    long Release() const { return --IVRefCount; }
};
enum ENullDef { NULL_DEF };
template <class T> class CDefPointer {
public:
    __forceinline CDefPointer(ENullDef = NULL_DEF) : Object(0) { }
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer() { Release(); }
    __forceinline void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;
};
class CThingShotDef;
class CMeleeCombatAbilityDef;
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0x1c];
    unsigned long   UsingComponents[9];                  // +0x20 bitset<274>
    CInterfaceMap   Components;                          // +0x44
    unsigned char   _pad4c[0x45];
    unsigned char   Flags91;                             // +0x91 bit 0 dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound((ETCInterfaceType)type);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    bool TestTC(long type) const { return ((UsingComponents[type >> 5] >> (type & 31)) & 1) != 0; }   // bitset::test
    template <class T> __forceinline bool GetInterfaceT(T** out, long type)
    {
        bool has = TestTC(type);
        if (has) {
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound((ETCInterfaceType)type);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
        }
        return has;
    }
    CTCBase* AddTC(const CCharString& name, bool activate, const class CTCParamsBase* params);   // 0x004C9D60
};
class CThingCreatureBase : public CThing { public:
    void MakeNoise(const CCharString& noise);             // 0x00662110
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot() const;                // 0x006A1440
};
class CTCWeapon { public:
    void SetProjectileWeaponLoaded(bool loaded, CThing* missile);   // 0x005DAC50
};
class CTCMultiArrow { public:
    unsigned char _pad00[0x10];
    long NumberOfShots;                                   // +0x10
    long NumberInVolley;                                  // +0x14
    void ReleaseShot();                                   // 0x007CC710
};
class CTCSolidToThing { public:
    void AddThingNotSolidTo(const CThing* thing);         // 0x00770FA0
};
namespace NThingShot {
class CTCShotDefinitionBase { public:
    unsigned char _pad00[0x38];
    CDefPointer<const CThingShotDef> PShotDef;            // +0x38
    void Init(CDefPointer<const CThingShotDef> shotDef, CThing* shooter, CThing* weapon, const CThing* target,
              float accuracy, float damage, float buildup, const CDefPointer<const CMeleeCombatAbilityDef>& ability);   // 0x007C0690
    void SetDecapitate(bool decapitate);                  // 0x007BED40
    void SetCanBeDeflected(bool deflect);                 // 0x007BED50
};
class CTCShotDefinitionMultiArrow : public CTCShotDefinitionBase { public:
    unsigned char _pad3c[0xb0 - 0x3c];
    bool FirstArrow;                                      // +0xB0 (name unproven)
};
CThing* __fastcall CreateShot(long defIndex, CThing* shooter, CThing* weapon, const CThing* target, float accuracy,
                              float damage, float buildup, const CDefPointer<const CMeleeCombatAbilityDef>& ability,
                              const C3DVector* pos, const CRightHandedSet* rhs);   // 0x007C0AD0
CThing* __fastcall CreateShot(CThing& missile, CThing* shooter, CThing* weapon, const CThing* target, float accuracy,
                              float damage, float buildup, const CDefPointer<const CMeleeCombatAbilityDef>& ability,
                              long index);                // 0x007C0E80
}
class CWorld { public:
    static long __fastcall GetFrame();                    // 0x0049D870
};
extern long g_FireMissileWeaponFrame_013bafd0;            // 0x013BAFD0 (written only; no name)
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline T* operator->() const { return (T*)GetPItem(); }
    __forceinline operator T*() const { return (T*)GetPItem(); }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_FireMissileWeapon : public CCreatureActionBase { public:
    CIntelligentPointer<const CThing> PTarget;           // +0xA8
    float ShotAccuracyPercentage;                        // +0xB0
    float ShotDamageMultiplier;                          // +0xB4
    float ShotBuildupTime;                               // +0xB8
    bool Decapitate;                                     // +0xBC
    long DelayTimer;                                     // +0xC0
    CDefPointer<const CMeleeCombatAbilityDef> PSpecialShotDef;   // +0xC4
    virtual void PerformAction();
private:
    void ReleaseMissile(CThing** missile, CThing** weapon, long& shotDefIndex);   // 0x008D5790
    void ResetCreatureUseProjectileWeaponState();         // 0x008D58D0
};

void CCreatureAction_FireMissileWeapon::PerformAction()
{
    g_FireMissileWeaponFrame_013bafd0 = CWorld::GetFrame();
    if (IS_THING_ALIVE(PCreature)) {
        CTCCarrying* carrying;
        if (!PCreature->GetInterfaceT(&carrying, TC_CARRYING))
            return;
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (!IS_THING_ALIVE(weapon))
            return;
        CTCWeapon* weaponTC;
        if (!weapon->GetInterfaceT(&weaponTC, TC_WEAPON))
            return;
        weaponTC->SetProjectileWeaponLoaded(false, 0);
        PCreature->MakeNoise("SIMPLE_NOISE");
        CThing* missile = 0;
        CThing* missileWeapon = 0;
        long shotDefIndex = -1;
        ReleaseMissile(&missile, &missileWeapon, shotDefIndex);
        if (shotDefIndex) {
            long shotCount = 1;
            CTCMultiArrow* multiArrow = 0;
            if (PCreature->GetInterfaceT(&multiArrow, TC_MULTI_ARROW) && multiArrow->NumberOfShots > 0)
                shotCount = multiArrow->NumberInVolley;
            std::vector<std::pair<CThing*, CTCSolidToThing*> > shots;
            for (long i = 0; i < shotCount; ++i) {
                CThing* shot;
                if (!missile) {
                    shot = NThingShot::CreateShot(shotDefIndex, PCreature, missileWeapon, PTarget, ShotAccuracyPercentage,
                                                  ShotDamageMultiplier, ShotBuildupTime, PSpecialShotDef, 0, 0);
                } else {
                    shot = NThingShot::CreateShot(*missile, PCreature, missileWeapon, PTarget, ShotAccuracyPercentage,
                                                  ShotDamageMultiplier, ShotBuildupTime, PSpecialShotDef, -1);
                    missile = 0;
                }
                if (IS_THING_ALIVE(shot)) {
                    if (multiArrow && multiArrow->NumberOfShots > 0) {
                        NThingShot::CTCShotDefinitionBase* shotDef;
                        shot->GetInterfaceT(&shotDef, TC_SHOT_DEFINITION);
                        CDefPointer<const CThingShotDef> def = shotDef->PShotDef;
                        NThingShot::CTCShotDefinitionMultiArrow* arrow =
                            (NThingShot::CTCShotDefinitionMultiArrow*)shot->AddTC("CTCShotDefinitionMultiArrow", true, 0);
                        if (i == 0)
                            arrow->FirstArrow = true;
                        arrow->Init(def, PCreature, missileWeapon, PTarget, ShotAccuracyPercentage, ShotDamageMultiplier,
                                    ShotBuildupTime, PSpecialShotDef);
                        arrow->SetCanBeDeflected(false);
                    }
                    NThingShot::CTCShotDefinitionBase* shotDef;
                    shot->GetInterface(&shotDef, TC_SHOT_DEFINITION);
                    if (Decapitate)
                        shotDef->SetDecapitate(true);
                    CTCSolidToThing* solid;
                    if (shot->GetInterface(&solid, TC_SOLID_TO_THING)) {
                        for (std::pair<CThing*, CTCSolidToThing*>* it = shots.begin(); it != shots.end(); ++it) {
                            solid->AddThingNotSolidTo(it->first);
                            it->second->AddThingNotSolidTo(shot);
                        }
                        shots.push_back(std::pair<CThing*, CTCSolidToThing*>(shot, solid));
                    }
                }
                if (multiArrow)
                    multiArrow->ReleaseShot();
            }
        }
    }
    ResetCreatureUseProjectileWeaponState();
}
