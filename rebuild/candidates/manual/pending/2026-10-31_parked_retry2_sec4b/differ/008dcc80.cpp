// NCreatureMode::CModeJackOfBladesFlash::Flash() -- Jack of Blades' flash: when the creature still perceives its live
//   target, burn it (CTCBurntEffect) and hit it once the burn counter passes 5; otherwise put its flames out; then hit
//   and burn every other AI creature (GetAllThings with CThingFilter_IsNot) that is alive and conscious
// Grade: DIFFER(1133v1136) 72 difflines (wave 2 sec4b: first reconstruction from the disassembly; was the empty stub 1v1136)
// Tried (graded): g1 straight draft 1169v1136/428; g2 same-TU STLport vector + GetAllThings template bodies (the vector stays
//   in registers across the loop as retail) 1146/162; g3 landed-sibling Normalise (operator*(v, const float&)) 1133/144;
//   g4 filter temporary + block-scoped things 1135/135; g5 perceived as a named local, `PTarget.Get() == perceived` 131;
//   g6 member Normalise via operator*=(const float&) 1126/128; g7 block-1 delta written element-wise 1142/88 (frame 0x88
//   = retail); g8 the def as the shared engine/CDefPointer.h + the same-TU CDefClassBase::GetSubDef<T> template body (the
//   release loses its null test as retail) 1133/75; g9 ParentThing cached in a local before PTarget.Get() (retail's ebx)
//   72. No gain: member/memberwise operator- (190), operator-= chain (189), in-place `dir = Normalise(dir)` (177),
//   element-wise in the loop too (130), ctor from components (130), dx/dy locals (111); after g8: explicit C3DVector copy
//   ctor (75, inert), block 1 as `target->GetPos() - ParentThing->GetPos()` with free or member ctor-form operator- (107),
//   member element-wise operator- (106 / 117).
// Residue: block 1 keeps dir.X/dir.Y on the x87 stack and spills only Z with `fst` (ours stores X/Y and keeps Z), so the
//   Normalise squares and scales read different slots; the loop's delta evaluates Z first (ctor arguments right-to-left;
//   retail X first); the OnHit vtable call / def release colour eax/edx (ours) vs edx/eax; loop alignment nop.
// Next: a delta form that leaves all three components on the x87 stack (an inline helper returning C3DVector by value
//   whose X is computed first), the CHitParameters ctor via an engine header.
// Leaf: ?Flash@CModeJackOfBladesFlash@NCreatureMode@@AAEXXZ
class CTCBase;
class CTCParamsBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
extern unsigned long g_InvSqrtMantissaTable[128];        // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
class C3DVector { public:
    float X, Y, Z;
    __forceinline C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline const C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline C3DVector& Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); return *this; }
};
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector Normalise(const C3DVector& v) { return v * FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
enum ETCInterfaceType { TC_PERCEIVE_THING = 0x34, TC_BURNT_EFFECT = 0x89 };
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

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
class CThingBaseDef : public CDefClassBase { };
class CBaseObject { public: virtual void v00(); };
namespace NHitLocations { class CHitLocation; }
class CThing : public CBaseObject { public:
    unsigned char   _pad04[0x1c];
    unsigned long   UsingComponents[9];                  // +0x20 bitset<274>
    CInterfaceMap   Components;                          // +0x44
    unsigned char   _pad4c[0x20];
    unsigned long   ThingTypeFlags;                      // +0x6c
    CDefPointer<const CThingBaseDef> PBaseDef;           // +0x70
    unsigned char   _pad74[0x1d];
    unsigned char   LifeFlags;                           // +0x91 bit 0 dead
    __forceinline bool IsDead() const { return (LifeFlags & 1) != 0; }
    __forceinline bool IsIncludedInThingScans() const
    {
        unsigned char f = LifeFlags;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    const C3DVector& GetPos() const;                     // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool, const CTCParamsBase* params);   // 0x004C9D60
    void RemoveTC(ETCInterfaceType type);                // 0x004C9840
    __forceinline bool HasTC(long type) const { return ((UsingComponents[type >> 5] >> (type & 31)) & 1) != 0; }
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
class CHitParameters { public:
    unsigned char _pad00[0x20];
    bool PlayHitResponse;                                // +0x20
    bool PlayHitResponseOverrideSet;                     // +0x21
    unsigned char _pad22[0x60 - 0x22];
    CHitParameters(NHitLocations::CHitLocation* location, CThing* hitter, CThing* weapon, const C3DVector& pos,
                   const C3DVector& dir, bool, bool, bool, bool);   // 0x00789238
    ~CHitParameters();                                   // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);   // 0x00868CA0
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { public:
    enum { ThingTypeMask = 0x1f };
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06();
    virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12();
    virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18();
    virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24();
    virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30();
    virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36();
    virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41(); virtual void s42();
    virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48();
    virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54();
    virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60();
    virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64();
    virtual void OnHit(CHitParameters& hit);             // slot 65 (+0x104)
};
class CTCPerceiveThing { public:
    unsigned char _pad00[0x2d];
    bool LineOfSight;                                    // +0x2d
    const CThing* GetPerceivedThing() const;             // 0x007446A0
};
class CJackOfBladesBattleDef : public CSubDefClassBase { public:   // RTTI .?AVCJackOfBladesBattleDef@@
    static CCharString __fastcall StatGetTypeName() { return CCharString("CJackOfBladesBattleDef", -1); }
    unsigned char _pad25[0x68 - 0x25];
    float FlashDamage;                                   // +0x68
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const   // <CJackOfBladesBattleDef> 0x008C33E0
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
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
    __forceinline T* Get() const { return (T*)GetPItem(); }
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
// The thing manager's list: std::list<CThing*> (STLport node = next, prev, value).
struct CThingList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
};
class CThingFilter_IsNot { public:
    const CThing* Thing;
    __forceinline CThingFilter_IsNot(const CThing& thing) : Thing(&thing) { }
    bool operator()(const CThing& thing) const;           // 0x006A76A0
};
class CWorld;
class CThingSearchTools { public:
    const CThingList& PeekAllThings() const;               // 0x0049C770
    template <class T, class C, class F> void GetAllThings(const F& filter, C& result) const
    {
        const CThingList& things = PeekAllThings();
        for (CThingList::SNode* it = things.m_pHead->m_pNext; it != things.m_pHead; it = it->m_pNext) {
            CThing* thing = it->m_value;
            if (thing->IsIncludedInThingScans() && IsThingOfType<T>(thing) && filter(*(T*)thing))
                result.push_back((T*)thing);
        }
    }
};
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                 // +0x20
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}
namespace NCreatureMode { bool __fastcall IsUnconscious(const CThing& thing); }   // 0x0085EE40

class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual ~CModeBase();
        CThing* ParentThing;                             // +0x04
        unsigned char _pad08[0x14];
        CWorld& GetWorld() const;                        // 0x0073A680
    };
};
namespace NCreatureMode {
class CModeStandBase : public CTCCreatureModeManager::CModeBase { public:
    unsigned char _pad1c[0x68 - 0x1c];
};
class CModeJackOfBladesFlash : public CModeStandBase {
public:
    long Timer;                                          // +0x68
    long BurnCounter;                                    // +0x6c
    unsigned char Filter[0xc];                           // +0x70 CAnimFilterOfGroup
    CThingCreatureBase* PCreature;                       // +0x7c
    CIntelligentPointer<CThingCreatureBase> PTarget;     // +0x80
private:
    void Flash();
};

void CModeJackOfBladesFlash::Flash()
{
    CDefPointer<const CJackOfBladesBattleDef> def;
    ParentThing->PBaseDef->GetSubDef(&def);
    float damage = def->FlashDamage;
    if (PTarget.Get() && !PTarget.Get()->IsDead()) {
        CTCPerceiveThing* perceive;
        if (PCreature->GetInterface(&perceive, TC_PERCEIVE_THING) && IS_THING_ALIVE(perceive->GetPerceivedThing())) {
          const CThing* perceived = perceive->GetPerceivedThing();
          if (PTarget.Get() == perceived) {
            bool burnt = PTarget.Get()->HasTC(TC_BURNT_EFFECT);
            if (perceive->LineOfSight) {
                if (!burnt)
                    PTarget.Get()->AddTC("CTCBurntEffect", false, 0);
                long hitDamage = 0;
                if (BurnCounter > 5)
                    hitDamage = (long)damage;
                ++BurnCounter;
                CThing* parent = ParentThing;
                CThingCreatureBase* target = PTarget.Get();
                C3DVector dir;
                const C3DVector& from = parent->GetPos();
                const C3DVector& to = target->GetPos();
                dir.X = to.X - from.X; dir.Y = to.Y - from.Y; dir.Z = to.Z - from.Z;
                dir.Normalise();
                CHitParameters hit(0, ParentThing, 0, PTarget.Get()->GetPos(), dir, false, false, false, false);
                hit.PlayHitResponseOverrideSet = true;
                hit.PlayHitResponse = false;
                hit.AddDamageWithAttribute(3, (float)hitDamage);
                ((CThingAICreature*)PTarget.Get())->OnHit(hit);
            } else if (burnt) {
                PTarget.Get()->RemoveTC(TC_BURNT_EFFECT);
            }
          }
        }
    }
    {
        std::vector<CThingAICreature*> things;
        GetWorld().ThingSearchTools->GetAllThings<CThingAICreature, std::vector<CThingAICreature*>, CThingFilter_IsNot>(CThingFilter_IsNot(*PCreature), things);
        long count = (long)things.size();
        for (long i = 0; i < count; ++i) {
            if (things[i] && !things[i]->IsDead() && !IsUnconscious(*things[i])) {
                if (!(things[i]->UsingComponents[4] & 0x200))
                    things[i]->AddTC("CTCBurntEffect", false, 0);
                long hitDamage = 0;
                if (BurnCounter > 5)
                    hitDamage = (long)damage;
                C3DVector dir = things[i]->GetPos() - ParentThing->GetPos();
            dir.Normalise();
                CHitParameters hit(0, ParentThing, 0, things[i]->GetPos(), dir, false, false, false, false);
                hit.PlayHitResponseOverrideSet = true;
                hit.PlayHitResponse = false;
                hit.AddDamageWithAttribute(3, (float)hitDamage);
                things[i]->OnHit(hit);
            }
        }
    }
}
}
