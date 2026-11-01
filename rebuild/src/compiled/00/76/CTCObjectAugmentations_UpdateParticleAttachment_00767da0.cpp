// CTCObjectAugmentations::UpdateParticleAttachment(CIntelligentPointer<CThing> particle, const CCharString& startPoint, const CCharString& endPoint) -- aim an augmentation particle effect between two points of the owner: read the owner's frame, take each named point from the graphic appearance's helper points (else its dummy objects), put both into a CParticleUpdateParameters block as the START / END vector parameters and hand it to the effect's particle emitter
// Leaf: ?UpdateParticleAttachment@CTCObjectAugmentations@@AAEXV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00767DA0, 622 bytes, __fastcall `ret 10h` (the 8-byte by-value CIntelligentPointer<CThing>
//   plus two CCharString references), frame 0x7C, no EH frame (the by-value pointer and the parameter block die at the end).
// Identity: Ego_r.pdb / FableWin.pdb name set ?UpdateParticleAttachment@CTCObjectAugmentations@@AAEXV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@1@Z
//   (private, void, by-value intelligent pointer + two const CCharString&): the only CTCObjectAugmentations member with a
//   16-byte argument list of that shape; the row sits inside the CTCObjectAugmentations TU (after AttachAugmentationParticles
//   0x007677E0, before OnInitialActivate 0x00768010, both landed); the callee-side ~CBaseIntelligentPointer 0x00A01C10 on
//   [esp+8Ch] (= the first argument slot) proves the by-value pointer; the literals are "START" 0x01264990 / "END" 0x01239B74.
//   No caller in retail (unreferenced out-of-line body). The catalog label "CTCLinkDisplay_SetupParticleNodes" is a forge stand-in.
// Callees: CTCPhysicsBase::GetLastRHSet (slot 72, +0x120), NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint 0x004C0560 /
//   GetDummyObject 0x004C08C0, CCRC::Calc 0x004014A0, CBaseClass() 0x0099A2D0, vector<pair<unsigned long, C3DVector> >::
//   _M_insert_overflow 0x0076ADA0 (landed), std::sort<pair<unsigned long, C3DVector>*, CParticleUpdateCCompareVector<unsigned long> >
//   0x0076C000 and the CParticleUpdateParamDesc instantiation 0x0076C1A0 (both __fastcall), CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCDParticleEmitter::SetEmitterUpdateParameters
//   0x006E11A0, ~CParticleUpdateParameters 0x006E2500, ~CBaseIntelligentPointer 0x00A01C10.
// Layout: CThing retail offsets as the landed CastFireball 0x007C89F0 (UsingComponents +0x20, Components +0x44, PhysicsTC +0x60,
//   PTCGraphicAppearance +0x64); CParticleUpdateParameters<unsigned long, unsigned long> (vtable 0x0125FFDC, Ego_r: VectorParams +4,
//   UpdateParams +0x14 at retail's 0x10-byte CVectorMap); CTCDParticleEmitter interface type 0x4B (`test ch,8` on word +0x28).
// Shape: the PDB's inline accessors GetPhysicsTC() / GetTCGraphicAppearance() put the [ParentThing+0x64] load after the pushed
//   arguments (direct member access loads `this` first: 76 diff lines); the PDB's CVectorMap::Insert (push_back + Dirty), Update
//   (sort when dirty) and CParticleUpdateParameters::Update inline into retail's two push_backs and two guarded sorts; the real
//   STLport 4.6 _M_insert_overflow body (and its __uninitialized_copy / __uninitialized_fill_n) in the TU, as in the landed
//   0x0076ADA0 TU, lets VC7.1 keep the END key in esi for the second push_back's in-place construct; the PDB's bool
//   GetTC(T**) with the one-ternary store `*out = ((it == end || T < it->first) ? end : it)->second` lands the emitter in ecx.
extern "C" void* __cdecl malloc(unsigned int);           // 0x00BFEA0E
extern "C" void __cdecl free(void*);                     // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
template <class _T1, class _T2> struct pair {             // STLport 4.6 pair
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type is_POD_type; };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{                                                          // 0x00768FC0 (same-TU body, as the landed 0x0076ADA0 TU)
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{                                                          // 0x00769000 (same-TU body)
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    typedef typename _Vector_base<_Tp, _Alloc>::allocator_type allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector();
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x) {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, typename __type_traits<_Tp>::is_POD_type(), 1UL, true);
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);     // 0x0076ADA0
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // STLport _algo.h (out of line)
}   // namespace std

enum ETCInterfaceType { };
class CTCBase;
class CCharString { public:
    void* PStringData;
};
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CRightHandedSet { public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
    CRightHandedSet() { }
};
class CCRC { public:
    static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                             // +0x00
    TCompare                Compare;                          // +0x0C
    bool                    Dirty;                            // +0x0D
    CVectorMap() : Dirty(false) { }
    value_type* LowerBound(const K& key);                     // 0x0040F020 for the TC map
    void Insert(const value_type& value) { Data.push_back(value); Dirty = true; }
    void Sort() { std::sort(Data.begin(), Data.end(), Compare); }
    void Update() { if (Dirty) { Sort(); Dirty = false; } }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
namespace NParticleEngine {
template <class K, class V> class CParticleUpdateParamCallback;
template <class K, class V> class CParticleUpdateParamDesc {   // Ego_r: 12 bytes
public:
    virtual ~CParticleUpdateParamDesc();
    CParticleUpdateParamCallback<K, V>* Callback;            // +0x04
    unsigned long Index;                                     // +0x08
};
template <class K, class V> class CParticleUpdateParameters : public CBaseClass {   // vtable 0x0125FFDC
public:
    template <class T> class CParticleUpdateCCompareVector { public:
        bool operator()(const std::pair<T, C3DVector>& a, const std::pair<T, C3DVector>& b) const { return a.first < b.first; }
    };
    template <class T> class CParticleUpdateCCompareParamDesc { public:
        bool operator()(const std::pair<T, CParticleUpdateParamDesc<K, V> >& a, const std::pair<T, CParticleUpdateParamDesc<K, V> >& b) const { return a.first < b.first; }
    };
    CVectorMap<K, C3DVector, CParticleUpdateCCompareVector<K> > VectorParams;                                 // +0x04
    CVectorMap<K, CParticleUpdateParamDesc<K, V>, CParticleUpdateCCompareParamDesc<K> > UpdateParams;        // +0x14
    CParticleUpdateParameters() { }
    virtual ~CParticleUpdateParameters();               // 0x006E2500
    void Update() { VectorParams.Update(); UpdateParams.Update(); }
};
}
class CTCPhysicsBase;
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& position, float scale) const;   // 0x004C0560
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& position,
        CRightHandedSet& frame, float scale) const;         // 0x004C08C0
};
}
class CThing { public:                                      // partial view, retail offsets
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad_0x00[0x20];                     // +0x00 vptr (vtable 0x01239ABC) .. +0x1C
    unsigned long      UsingComponents[9];                  // +0x20 (bitset<274>)
    TCMap              Components;                          // +0x44
    unsigned char      _pad_0x54[0x60 - 0x54];
    CTCPhysicsBase*    PhysicsTC;                           // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (HasTC(T::INTERFACE_TYPE)) {
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            *out = (T*)((it == map->Data.end() || T::INTERFACE_TYPE < it->first) ? map->Data.end() : it)->second;
            return true;
        }
        return false;
    }
};
class CTCPhysicsBase { public:                             // vtable 0x0125D2C4 (slot names as the landed CastFireball 0x007C89F0)
    // slots 0-68 are not called here (CTCBase's 28 + physics slots 28-68; see the landed CastFireball model)
    virtual void CUnprovenSlot00_0125d2c4(); virtual void CUnprovenSlot01_0125d2c4(); virtual void CUnprovenSlot02_0125d2c4(); virtual void CUnprovenSlot03_0125d2c4();
    virtual void CUnprovenSlot04_0125d2c4(); virtual void CUnprovenSlot05_0125d2c4(); virtual void CUnprovenSlot06_0125d2c4(); virtual void CUnprovenSlot07_0125d2c4();
    virtual void CUnprovenSlot08_0125d2c4(); virtual void CUnprovenSlot09_0125d2c4(); virtual void CUnprovenSlot10_0125d2c4(); virtual void CUnprovenSlot11_0125d2c4();
    virtual void CUnprovenSlot12_0125d2c4(); virtual void CUnprovenSlot13_0125d2c4(); virtual void CUnprovenSlot14_0125d2c4(); virtual void CUnprovenSlot15_0125d2c4();
    virtual void CUnprovenSlot16_0125d2c4(); virtual void CUnprovenSlot17_0125d2c4(); virtual void CUnprovenSlot18_0125d2c4(); virtual void CUnprovenSlot19_0125d2c4();
    virtual void CUnprovenSlot20_0125d2c4(); virtual void CUnprovenSlot21_0125d2c4(); virtual void CUnprovenSlot22_0125d2c4(); virtual void CUnprovenSlot23_0125d2c4();
    virtual void CUnprovenSlot24_0125d2c4(); virtual void CUnprovenSlot25_0125d2c4(); virtual void CUnprovenSlot26_0125d2c4(); virtual void CUnprovenSlot27_0125d2c4();
    virtual void CUnprovenSlot28_0125d2c4(); virtual void CUnprovenSlot29_0125d2c4(); virtual void CUnprovenSlot30_0125d2c4(); virtual void CUnprovenSlot31_0125d2c4();
    virtual void CUnprovenSlot32_0125d2c4(); virtual void CUnprovenSlot33_0125d2c4(); virtual void CUnprovenSlot34_0125d2c4(); virtual void CUnprovenSlot35_0125d2c4();
    virtual void CUnprovenSlot36_0125d2c4(); virtual void CUnprovenSlot37_0125d2c4(); virtual void CUnprovenSlot38_0125d2c4(); virtual void CUnprovenSlot39_0125d2c4();
    virtual void CUnprovenSlot40_0125d2c4(); virtual void CUnprovenSlot41_0125d2c4(); virtual void CUnprovenSlot42_0125d2c4(); virtual void CUnprovenSlot43_0125d2c4();
    virtual void CUnprovenSlot44_0125d2c4(); virtual void CUnprovenSlot45_0125d2c4(); virtual void CUnprovenSlot46_0125d2c4(); virtual void CUnprovenSlot47_0125d2c4();
    virtual void CUnprovenSlot48_0125d2c4(); virtual void CUnprovenSlot49_0125d2c4(); virtual void CUnprovenSlot50_0125d2c4(); virtual void CUnprovenSlot51_0125d2c4();
    virtual void CUnprovenSlot52_0125d2c4(); virtual void CUnprovenSlot53_0125d2c4(); virtual void CUnprovenSlot54_0125d2c4(); virtual void CUnprovenSlot55_0125d2c4();
    virtual void CUnprovenSlot56_0125d2c4(); virtual void CUnprovenSlot57_0125d2c4(); virtual void CUnprovenSlot58_0125d2c4(); virtual void CUnprovenSlot59_0125d2c4();
    virtual void CUnprovenSlot60_0125d2c4(); virtual void CUnprovenSlot61_0125d2c4(); virtual void CUnprovenSlot62_0125d2c4(); virtual void CUnprovenSlot63_0125d2c4();
    virtual void CUnprovenSlot64_0125d2c4(); virtual void CUnprovenSlot65_0125d2c4(); virtual void CUnprovenSlot66_0125d2c4(); virtual void CUnprovenSlot67_0125d2c4();
    virtual void CUnprovenSlot68_0125d2c4();
    virtual C3DVector GetFacingVector() const;                                   // 70 (+0x118) (overloads sit in reverse order)
    virtual C3DVector GetFacingVector(float time) const;                         // 69 (+0x114)
    virtual CRightHandedSet GetRHSet(float time) const;                          // 71 (+0x11C, 0x006B1450)
    virtual const CRightHandedSet& GetLastRHSet() const;                         // 72 (+0x120, 0x006B1290)
};
class CTCDParticleEmitter { public:
    enum { INTERFACE_TYPE = 0x4B };
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& parameters);   // 0x006E11A0
};
class CBaseObjectPointer;
class CBaseObject;
class CBaseIntelligentPointer { public:                     // vtable 0x0129C95C, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return GetPointer(); }
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
};
class CTCObjectAugmentations : public CTCBase { public:
private:
    void UpdateParticleAttachment(CIntelligentPointer<CThing> particle, const CCharString& startPoint, const CCharString& endPoint);
};

void CTCObjectAugmentations::UpdateParticleAttachment(CIntelligentPointer<CThing> particle, const CCharString& startPoint, const CCharString& endPoint)
{
    CRightHandedSet frame = ParentThing->GetPhysicsTC()->GetLastRHSet();
    C3DVector start;
    if (!ParentThing->GetTCGraphicAppearance()->GetHelperPoint(startPoint, 0, 0, start, 1.0f))
        ParentThing->GetTCGraphicAppearance()->GetDummyObject(startPoint, 0, 0, start, frame, 1.0f);
    C3DVector end;
    if (!ParentThing->GetTCGraphicAppearance()->GetHelperPoint(endPoint, 0, 0, end, 1.0f))
        ParentThing->GetTCGraphicAppearance()->GetDummyObject(endPoint, 0, 0, end, frame, 1.0f);
    std::pair<unsigned long, C3DVector> startParam(CCRC::Calc(0, "START", 5), start);
    std::pair<unsigned long, C3DVector> endParam(CCRC::Calc(0, "END", 3), end);
    NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
    parameters.VectorParams.Insert(startParam);
    parameters.VectorParams.Insert(endParam);
    parameters.Update();
    CTCDParticleEmitter* emitter;
    if (particle->GetTC(&emitter) && emitter)
        emitter->SetEmitterUpdateParameters(parameters);
}