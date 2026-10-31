// CCreatureAction_CastWillRaiseDeadSpell::PerformAction() -- raise an undead soldier: collect the valid positions within
//   3 m of the caster (NCreatureHiding::GetValidPositions), keep those outside the caster's radius + 0.3, pick one at
//   random, create CREATURE_OK_UNDEAD_SOLDIER_SWORD there for the caster's player (FACTION_HERO, the undead-servant brain,
//   full health = Health, the appearance seed), start its rise-up action and publish it through PUndeadFollower
// Grade: RELOCATION_MATCH (1083v1083) on the shared engine/CDefPointer.h (wave 2 sec4b: first reconstruction from the
//   disassembly; was the empty skeleton 1v1083)
// Graded: g1 straight draft 1098v1083/92; g2 `long i < positions.size()` (retail's unsigned jb/je on a signed index; an
//   unsigned index strength-reduces, 203) 87; g3 same-TU CDefinitionManager::GetDef<T> template body + header-shaped
//   CDefPointer (brain.Object stays in edi as retail) 1083v1083/59; g4 the delta as a C3DVector (X/Y only,
//   GetSquaredMagnitude2D: the 12-byte slot) 9; g5 CCreatureAction_RiseUp::FaceTarget (+0x114, retail size 0x118) MATCH;
//   g6 moved onto engine/CDefPointer.h (RELOCATION_MATCH unchanged).
// Header: engine/CDefClassBase.h + engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE); the header gained the ENullDef
//   constructor on 2026-10-31, so the local CDefPointer copy is gone (g6: same bytes on the shared header).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BC1A0, 1083 bytes, virtual PerformAction (vtable 0x0127D41C), plain `ret`.
// Layout: retail CCombatActionBase 0x114 (PCreature +0x08); PUndeadFollower +0x114, Health +0x118, AppearanceSeed +0x11C
//   (Ego_r +0x12C..+0x134 less the 0x18 base shrink); CThing PhysicsTC +0x60, PlayerNumber +0x90; CTCPhysicsBase::Radius +0x34;
//   CThingAICreatureInit (Ego_r, 0x1C) built inline.
// Callees (PDB spellings, Ego_r/FableWin): see the declarations; NCreatureHiding::GetValidPositions 0x0074A5D0, PeekThingSearchTools
//   0x00692990, GetWorldMap 0x00692C10, GetWorld 0x00692940, CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890, vector<long>
//   reserve 0x00660684 / _M_insert_overflow 0x00440990, CDefinitionManager::GetDef<CBrainDef> 0x0079BD80, ModifyHealth = slot 59.
// Leaf: ?PerformAction@CCreatureAction_CastWillRaiseDeadSpell@@UAEXXZ
class CBaseObject { public: virtual void v00(); };
class CThing;
class CThingPhysical;
class CThingBuilding;
class CThingCreatureBase;
class CWorldMap;
class CThingSearchTools;
class CTCShapeManager;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
class C3DVector { public:
    float X, Y, Z;
    __forceinline float GetSquaredMagnitude2D() const { return X * X + Y * Y; }
};
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
struct __true_type { };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    void reserve(size_type __n)
    {
        if (capacity() < __n) {
            const size_type __old_size = size();
            pointer __tmp;
            if (this->_M_start) {
                __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
                _M_clear();
            } else {
                __tmp = this->_M_end_of_storage.allocate(__n);
            }
            _M_set(__tmp, __tmp + __old_size, __tmp + __n);
        }
    }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    pointer _M_allocate_and_copy(size_type __n, const _Tp* __first, const _Tp* __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __copy_trivial(__first, __last, __result);
        return __result;
    }
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}

class CTCBase;
enum ETCInterfaceType { TC_RANDOM_APPEARANCE_MORPH = 3, TC_ENEMY = 0x49 };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPhysicsBase { public:
    unsigned char _pad00[0x34];
    float Radius;                                        // +0x34
    float GetRadius() const { return Radius; }
};
class CTCEnemy { public:
    void SetFaction(const CCharString& faction);         // 0x0076C810
};
class CTCRandomAppearanceMorph { public:
    void SetAppearanceSeed(unsigned long seed);          // 0x00728460
};
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0x1c];
    unsigned long   UsingComponents[9];                  // +0x20 bitset<274>
    CInterfaceMap   Components;                          // +0x44
    unsigned char   _pad4c[0x14];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    unsigned char   _pad64[0x2c];
    signed char     PlayerNumber;                        // +0x90
    const C3DVector& GetPos() const;                     // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    unsigned long GetPlayerNumber() const { return PlayerNumber; }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((it == map->last || type < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void ModifyHealth(float amount, bool flag);  // slot 59 (+0xEC)
    void SetMaxHealth(float health, bool flag);          // 0x006A5260
};
class CThingCreatureBase : public CThingPhysical { public:
    bool SetCurrentAction(const class CCreatureActionBase& action);   // 0x006644F0
};
class CBrainDef : public CDefClassBase { };
class CThingAICreatureInit { public:
    CThingPhysical* OriginalThing;                       // +0x00
    CThingPhysical* PreviousThing;                       // +0x04
    CCharString     ScriptName;                          // +0x08
    CCharString     OverridingBrainName;                 // +0x0C
    bool            PlayerCharacter;                     // +0x10
    bool            Drawable;                            // +0x11
    bool            PlaceAtEndOfList;                    // +0x12
    bool            EnableCreatureAutoPlacing;           // +0x13
    CThingBuilding* HomeBuilding;                        // +0x14
    CThingBuilding* WorkBuilding;                        // +0x18
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& brainName, bool drawable)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(playerCharacter), Drawable(drawable), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) { }
};
class CThingAICreature : public CThingCreatureBase { public:
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& pos, long player,
                                               const CThingAICreatureInit& init);   // 0x00833800
    void SetNewBrain(CDefPointer<const CBrainDef> brain);   // 0x00833010
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const    // <CBrainDef> 0x0079BD80
    {
        if (index > 0) {
            CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
            if (!def.IsNull()) {
                out->SetDynamicCast(def);
                return true;
            }
        }
        return false;
    }
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
};
class CWorld { public:
    unsigned long* GetWorldSeed() const;                 // 0x0049D810
};
long __fastcall GFROR13(unsigned long value);            // 0x00497890
inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
namespace NCreatureHiding {
void __fastcall GetValidPositions(std::vector<C3DVector>& positions, const CWorldMap& map, const CThingSearchTools& search,
                                  const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes);   // 0x0074A5D0
}
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline CIntelligentPointer(T* item) { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    __forceinline T* operator->() const { return (T*)GetPItem(); }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0x114 - 0x10];
protected:
    CWorld& GetWorld();                                  // 0x00692940
    CWorldMap& GetWorldMap();                            // 0x00692C10
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00692990
};
class CCombatActionBase : public CCreatureActionBase { public:
    virtual ~CCombatActionBase();                        // 0x0062E900
};
class CCreatureAction_RiseUp : public CCombatActionBase { public:
    bool FaceTarget;                                     // +0x114 (PDB +0x12C)
    CCreatureAction_RiseUp(CThingCreatureBase& creature);   // 0x008BD880
};
class CCreatureAction_CastWillRaiseDeadSpell : public CCombatActionBase { public:
    CIntelligentPointer<CThing>* PUndeadFollower;        // +0x114
    long Health;                                         // +0x118
    unsigned long AppearanceSeed;                        // +0x11C
    virtual void PerformAction();
};

void CCreatureAction_CastWillRaiseDeadSpell::PerformAction()
{
    std::vector<C3DVector> positions;
    NCreatureHiding::GetValidPositions(positions, GetWorldMap(), PeekThingSearchTools(), PCreature->GetPos(), 3.0f, false, 0);
    std::vector<long> valid;
    valid.reserve(positions.size());
    for (long i = 0; i < positions.size(); ++i) {
        const C3DVector& pos = positions[i];
        const C3DVector& creaturePos = PCreature->GetPos();
        C3DVector delta;
        delta.X = pos.X - creaturePos.X;
        delta.Y = pos.Y - creaturePos.Y;
        float radius = PCreature->GetPhysicsTC()->GetRadius() + 0.3f;
        if (delta.GetSquaredMagnitude2D() >= radius * radius)
            valid.push_back(i);
    }
    if (!valid.empty()) {
        long index = valid[GFRandom(valid.size(), GetWorld().GetWorldSeed())];
        CGameDefinitionManager& defs = CGameDefinitionManager::Get();
        long defIndex = defs.GetDefGlobalIndexFromName("CREATURE_OK_UNDEAD_SOLDIER_SWORD");
        CThingAICreature* creature = CThingAICreature::Create(defIndex, positions[index], PCreature->GetPlayerNumber(),
                                                              CThingAICreatureInit(0, 0, false, "", "", true));
        CTCEnemy* enemy;
        creature->GetInterface(&enemy, TC_ENEMY);
        enemy->SetFaction("FACTION_HERO");
        CDefPointer<const CBrainDef> brain(NULL_DEF);
        defs.GetDef(defs.GetDefGlobalIndexFromName("BRAIN_PLAYER_UNDEAD_SERVANT"), &brain);
        creature->SetNewBrain(brain);
        creature->SetMaxHealth((float)Health, false);
        creature->ModifyHealth((float)Health, true);
        CTCRandomAppearanceMorph* morph;
        if (creature->GetInterface(&morph, TC_RANDOM_APPEARANCE_MORPH))
            morph->SetAppearanceSeed(AppearanceSeed);
        creature->SetCurrentAction(CCreatureAction_RiseUp(*creature));
        *PUndeadFollower = CIntelligentPointer<CThing>(creature);
    }
}