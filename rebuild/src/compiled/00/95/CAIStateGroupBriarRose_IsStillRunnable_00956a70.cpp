// CAIStateGroup_BriarRose::IsStillRunnable() -- PEnemy is alive, the group is not stopping, and the creature's targeting component still targets PEnemy
// Leaf: ?IsStillRunnable@CAIStateGroup_BriarRose@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956A70, 135 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?IsStillRunnable@CAIStateGroup_BriarRose@@IAE_NXZ (donor 0x0097B5C0) sits beside the same neighbours; callers CAIStateGroup_BriarRose::Update 0x00956E70 (x2);
//   PEnemy is the Ego_r.pdb CIntelligentPointer<CThingCreatureBase> at retail +0x14.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CAIStateGroupBase::IsStopping 0x008FD010, CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound const 0x004365B0, the targeting component's virtual slot 29 (+0x74; CTCTargetingAI's returns its
//   targeted-thing pointer -- named GetPTargetedThing, the Ego_r.pdb CTCTargetingBase virtual of that shape: probable).
// Shape (family K047, representative 0x0094D930): IS_THING_ALIVE(PEnemy); `if (IsStopping()) return false;`; the GetTC<T>(T**)
//   lookup with the bitset reference test (`mov ecx,[eax+20h] ; test ch,1`) and the const lower bound, its result ignored (a
//   missing component reads the unset local, as the landed J018 GetTC rows); `if (PEnemy.GetPItem() == targeting->GetPTargetedThing()) return true; return false;` (the right operand -- the virtual
//   call -- is evaluated first; `sete`), the entry picked by the ternary so the component loads straight into ecx.
#include <stddef.h>
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    const_iterator end() const { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        size_t _M_bpos;
        reference(bitset<_Nb>& __b, size_t __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        ~reference() { }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](size_t __pos) { return reference(*this, __pos); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    const_iterator LowerBound(const K& key) const;            // 0x004365B0 for the TC map (the const overload)
};
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
class CTCBase;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      LifeFlags;                                         // +0x91 bit 0: dead
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::const_iterator it = map->LowerBound(type);
            *out = (T*)((it == map->Data.end() || T::INTERFACE_TYPE < it->first) ? map->Data.end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                               // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                                // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                            // 0x00A01B50
private:
    CBaseObjectPointer* PData;                                // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->LifeFlags & 1))
            return true;
    }
    return false;
}
class CTCTargetingBase {                                      // interface 8
public:
    enum { INTERFACE_TYPE = 8 };
    virtual ~CTCTargetingBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetPTargetedThing() const;                // slot 29 (+0x74)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                                        // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                         // +0x04
    long ScriptAIPriority;                                    // +0x08
    void* Name_PStringData;                                   // +0x0C
    unsigned char Terminated;                                 // +0x10
    unsigned char Priority;                                   // +0x11
    unsigned char Enabled;                                    // +0x12
    bool IsStopping();                                        // 0x008FD010
};
class CAIStateGroup_BriarRose : public CAIStateGroupBase {
protected:
    bool IsStillRunnable();
public:
    CIntelligentPointer<CThingCreatureBase> PEnemy;   // +0x14 (Ego_r.pdb +20)
};
bool CAIStateGroup_BriarRose::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy))
        return false;
    if (IsStopping())
        return false;
    CTCTargetingBase* targeting;
    PBrain->PCreature->GetTC(&targeting);
    if (PEnemy.GetPItem() == targeting->GetPTargetedThing())
        return true;
    return false;
}