// CTCTavernTable::GetEatingPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& seat) -- claim free ACTION_POINT_SIT points in turn until one whose seat number (the digits after the prefix) has food on the table; give up when the number is out of range
// Grade: DIFFER(408v417) -- register-allocation tie only: retail keeps &pointIndex in ebp and the startIndex/this web in ebx (ours ebx/ebp, so every [ebp] deref is a byte longer in retail), loads the map start into eax then `mov esi,eax` (ours straight into esi; loop pad `mov edi,edi` v `lea esp,[esp]`), and duplicates the ParentThing load at the back edge through edx (ours reloads this into ecx and jumps to the shared load). Control flow, calls, frame and literals match. next: an inline helper retail may have used around the seat parse (GetFood inlined flips the registers to retail but adds its check, p18); else lane tie.
// Leaf: ?GetEatingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8B10, 417 bytes, thiscall `ret 14h`; Ego_r.pdb ?GetEatingPoint@CTCTavernTable@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z.
// Callees: CTCNamedUserActionPoints::GetFreeNamedActionPoint 0x007FF190 (Ego_r.pdb, by-value CCharString built in its slot),
//   CCharString() 0x0099E4B0, CTCNamedUserActionPoints::GetName 0x007FEFC0, CCharString::GetSubString 0x0099EC70,
//   GFCharStringToInt 0x0099E7F0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCUserActionPointBase::SetActionPointAsFree
//   0x007FEF40 (Ego_r.pdb QAEXJ; the free-index push_back), ~CCharString 0x0099EAE0. "ACTION_POINT_SIT" 0x0126DC30.
// Shape: CThing::GetTC<CTCNamedUserActionPoints>(T**) (FableWin.pdb template; the uninitialised `points` when the interface
//   bit is clear is retail's `mov esi,[esp+28h]`), the bitset test inlined (`test ch,40h`), and the landed 0x0040F020
//   CVectorMap::LowerBound body inlined (this TU is not size-optimised; GetSeat 0x007B8840 in the same TU calls it).
// Layout: engine/CTCBase.h (ParentThing +4); Ego_r.pdb CTCTavernTable with retail 12-byte vectors: Food +0x28,
//   ActionPointSitStrlen +0x48, AvailableForUse +0x5C (no engine header).
#include "engine/CTCBase.h"
enum ETCInterfaceType { TCI_NAMED_USER_ACTION_POINTS = 0x2E };   // descriptive enumerator name
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    typedef _Tp& reference;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    _Tp* begin() const { return _M_start; }
    _Tp* end() const { return _M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    reference operator[](size_type __n) const { return *(begin() + __n); }
};
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const { return (_M_w[__pos / 32] & (1UL << (__pos % 32))) != 0; }
};
}
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {     // the landed 0x0040F020 model
public:
    typedef std::pair<K, V> value_type;
    value_type* LowerBound(const K& key)
    {
        long count = this->_M_finish - this->_M_start;
        value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    C Compare;                                            // +0x0C
    bool Dirty;                                           // +0x0D
};
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;    // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template <class T> __forceinline bool GetTC(T** out)
    {
        long type = T::StatGetInterfaceType();
        if (HasTC((ETCInterfaceType)type)) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CVectorMap<ETCInterfaceType, CTCBase*>* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
            *out = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { };
class C3DVector;
class CRightHandedSet;
template <class T> class CBasicString {
public:
    T*            PData;                                  // +0x00
    unsigned long StringLength;                           // +0x04
    unsigned long GetLength() const { return StringLength; }
};
class CCharStringData {
public:
    CBasicString<char> Data;                              // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.GetLength(); }
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                        // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CTCUserActionPointBase : public CTCBase {
public:
    void SetActionPointAsFree(long index);                // 0x007FEF40
};
class CTCNamedUserActionPoints : public CTCUserActionPointBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_NAMED_USER_ACTION_POINTS; }
    bool GetFreeNamedActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, CCharString name, long& pointIndex,
                                 long startIndex, bool claim);   // 0x007FF190
    void GetName(long pointIndex, CCharString& name);     // 0x007FEFC0
};
class CThingAICreature;
class CTCTavernTable : public CTCBase {
public:
    bool BeingServed;                                     // +0x0C
    std::vector<CThingAICreature*> Occupants;             // +0x10
    std::vector<CIntelligentPointer<CThingObject> > Tankards;   // +0x1C
    std::vector<CIntelligentPointer<CThingObject> > Food;       // +0x28
    std::vector<bool> SeatsCurrentlyUsed;                 // +0x34 (12 bytes here)
    long NumPeopleSeated;                                 // +0x40
    bool SomeoneTalking;                                  // +0x44
    bool SomeoneLaughing;                                 // +0x45
    long ActionPointSitStrlen;                            // +0x48
    long ActionPointGameMasterStrlen;                     // +0x4C
    long ActionPointHeroStrlen;                           // +0x50
    long ActionPointTankardStrlen;                        // +0x54
    long ActionPointMaidStrlen;                           // +0x58
    bool AvailableForUse;                                 // +0x5C
    bool GetEatingPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& seat);
};

bool CTCTavernTable::GetEatingPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& seat)
{
    if (AvailableForUse) {
        long startIndex = 0;
        for (;;) {
            CTCNamedUserActionPoints* points;
            ParentThing->GetTC(&points);
            if (!points->GetFreeNamedActionPoint(user, pos, rh, "ACTION_POINT_SIT", pointIndex, startIndex, true)) {
                pointIndex = -1;
                return false;
            }
            CCharString name;
            points->GetName(pointIndex, name);
            CCharString number = name.GetSubString(ActionPointSitStrlen, name.GetLength());
            long chair = GFCharStringToInt(number);
            if (Food.size() <= (unsigned long)(chair - 1) || chair - 1 < 0) {
                points->SetActionPointAsFree(pointIndex);
                pointIndex = -1;
                return false;
            }
            if (Food[chair - 1].GetPItem()) {
                seat = chair - 1;
                return true;
            }
            points->SetActionPointAsFree(pointIndex);
            startIndex = pointIndex + 1;
        }
    }
    return false;
}
