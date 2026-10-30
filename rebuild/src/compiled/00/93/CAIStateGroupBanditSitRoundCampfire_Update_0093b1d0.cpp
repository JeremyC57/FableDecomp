// CAIStateGroup_BanditSitRoundCampfire::Update() -- claim a free seat at the campfire, walk to it, face the fire, sheathe any weapon and sit on the floor
// Leaf: ?Update@CAIStateGroup_BanditSitRoundCampfire@@UAEXXZ
//
// Retail 0x0093B1D0, 1011 bytes (ends `ret` at 0x0093B5C2), `sub esp,0x168`, no EH frame. Virtual Update of the
// bandit "sit round campfire" AI state group (Ego_r.pdb names CAIStateGroup_BanditSitRoundCampfire::Update; it is
// the last member of the class in donor order, right after OnReplace 0x0093AFC0).
//
// Flow: FrameToNextCheckIsRunnable = frame + 5 s; the campfire's CTCUserActionPointBase (TC 0x2d, non-const
// LowerBound 0x0040F020) hands out a free action point (pos + orientation, SeatPosIndex); five random tries push
// the seat outward along (seat - fire) by GFFloatRandom(1) rotated by GFFloatRandom(1), kept when the creature's
// CTCCreatureNavigation (TC 7, const LowerBound 0x004365B0) says it is navigable; GotSeatPos; MoveToPos; LookAtPos
// (pos + seat forward); if carrying a weapon, PerformActionAndWait(CCreatureAction_SheatheWeapons); then a random
// "ST_SIT_ON_FLOOR" anim (CAnimFilterAnyGroup), a CModeSitFloor on the anim's group wrapped in a
// CCountedPointer<NCreatureMode::CModeSitBase>, Sitting, PerformAction(CCreatureAction_SitDown), SitMain, Terminate.
// Every failing step is `Terminate(); return;`.
//
// Callees: CWorld::GetFrame 0x0049D870, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap LowerBound
// 0x0040F020 / 0x004365B0, CTCUserActionPointBase::GetFreeActionPoint 0x007FEDE0, CThing::GetPhysPos 0x004C7950,
// C2DVector::Normalise 0x00A14540, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0,
// C2DVector::Rotate 0x00A141D0, CTCCreatureNavigation::IsPosNavigable 0x006CBA50, AIAction_MoveToPos 0x0097EC00,
// CThingPhysical::PeekPos 0x006A4D00, AIAction_LookAtPos 0x0097D580, CTCCarrying::IsThingCarryingWeapon 0x006A1540,
// CCreatureAction_SheatheWeapons ctor 0x008CC370, AIAction_PerformActionAndWait 0x0097DB70, ~CCombatActionBase
// 0x0062E900, CThingCreatureBase::GetRandomAnimFromNameWithFilter 0x00662EF0, operator new 0x00BFEA1A,
// CDefStringTable::GetString 0x009D49B0, CModeSitBase ctor 0x008D7CA0, CCharString ctor/operator=/dtor
// 0x0099E4B0/0x0099EFB0/0x0099EAE0, CCRC::Calc 0x004014A0, CCountedPointer<CModeSitBase> ctor 0x008AB380 / dtor
// 0x008AB3D0, CCreatureAction_SitDown ctor 0x008AB9D0 / dtor 0x008ABAA0, AIAction_PerformAction 0x0097D8B0,
// SitMain 0x0093AD70, CAIStateGroupBase::Terminate 0x008FCF40.
//
// Shape notes: the frame packing (one 12-byte slot at esp+0x10 shared by the TC keys, the C2DVector temps, the seat
// offset, `forward`, the filter temporary and `sitMode`) only comes out with same-TU noinline stand-ins for the
// CCountedPointer<CModeSitBase> ctor and dtor (GOTCHAS R&D I-3); with them declared only, `sitMode` keeps its own
// slot and the frame is 0x170. The stand-in bodies follow retail 0x008AB380 / 0x008AB3D0 and are not graded.
// [esp+0x20] is the conditional-temporary flag word (bit0 the SheatheWeapons temporary, bit1 the CCharString from
// the anim's GroupName inside the new-expression). 0x013BB704 is (anonymous namespace)::SIT_ON_FLOOR_ANIM_NAME
// ("ST_SIT_ON_FLOOR") of this source file; it is file-local in retail, so it is reached here through an extern.
//
// Layout (Ego_r.pdb): CAIStateGroupBase 0x14 (+0x04 PBrain .. +0x12 Enabled); +0x14 PCampfire
// (CIntelligentPointer<CThingObject>), +0x1C SeatPosIndex, +0x20 GotSeatPos, +0x21 Sitting, +0x24
// FrameToNextSitOnSeat, +0x28 FrameToNextCheckIsRunnable, +0x2C WasCarryingWeapon. No generated engine header models
// this polymorphic class (CAIStateGroupBase.h and CAIStateGroup_SitRoundCampfire.h are flat), so it is modelled here.
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include <stddef.h>

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(size_t __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    bool operator[](size_t __pos) const { return _Unchecked_test(__pos); }
    bool _Unchecked_test(size_t __pos) const { return (this->_M_getword(__pos) & this->_S_maskbit(__pos)) != 0; }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;
    TCompare           Compare;
    bool               Dirty;
    iterator LowerBound(const K& key);                  // 0x0040F020
    const_iterator LowerBound(const K& key) const;      // 0x004365B0
};


class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const;
    const char* GetPString() const;
};
class CCharStringData {
public:
    const char*   PData;
    unsigned long StringLength;
};
inline unsigned long CCharString::GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
inline const char* CCharString::GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long crc, const void* data, unsigned long length);   // 0x004014A0
};
class CDefStringTable {
public:
    CCharString GetString(long tablePos) const;         // 0x009D49B0
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828
class CDefString {
public:
    long TablePos;
    operator CCharString() const { return GDefStringTable.GetString(TablePos); }
};
class CAnimationEntry {
public:
    long       BankIndex;                               // +0x00
    CDefString AnimName;                                // +0x04
    CDefString GroupName;                               // +0x08
};

class CMainGameComponent {
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
protected:
    static long ConstantFPS;                            // 0x01375550
};

class C2DVector;
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    const C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C2DVector& v) const;
    const C3DVector& operator+=(const C2DVector& v);
};
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    const C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector& Normalise();                             // 0x00A14540
    void Rotate(float fraction);                        // 0x00A141D0
};
inline C3DVector C3DVector::operator+(const C2DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z); }
inline const C3DVector& C3DVector::operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
class CRightHandedSet {
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0C
    CRightHandedSet() { }
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

enum ETCInterfaceType { TC_USER_ACTION_POINT = 0x2d, TC_CREATURE_NAVIGATION = 7 };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObject : public CBaseClass { };
class CClass : public CBaseObject { };
class CTCBase;
class CThing : public CClass {
public:
    unsigned char      _pad04[0x14];
    unsigned __int64   UID;                             // +0x18
    std::bitset<274>   UsingComponents;                 // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        bool has = HasTC((ETCInterfaceType)T::INTERFACE_TYPE);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            *out = (T*)((it == map->Data.end() || T::INTERFACE_TYPE < it->first) ? map->Data.end() : it)->second;
        }
        return has;
    }
    template <class T> __forceinline bool PeekTC(T** out) const
    {
        bool has = HasTC((ETCInterfaceType)T::INTERFACE_TYPE);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::const_iterator it = map->LowerBound(type);
            *out = (T*)((it == map->Data.end() || T::INTERFACE_TYPE < it->first) ? map->Data.end() : it)->second;
        }
        return has;
    }
    C3DVector GetPhysPos() const;                       // 0x004C7950
};
class CThingPhysical : public CThing {
public:
    const C3DVector& PeekPos() const;                   // 0x006A4D00
};
class CThingGameObject : public CThingPhysical { };
class CThingObject : public CThingGameObject { };
class CAnimFilterBase {
public:
    CAnimFilterBase() { }
    virtual ~CAnimFilterBase() { }
    virtual bool operator()(const CAnimationEntry& entry) const = 0;
};
class CAnimFilterAnyGroup : public CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const;
};
class CThingCreatureBase : public CThingGameObject {
public:
    const CAnimationEntry& GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x00662EF0
};
class CThingAICreature : public CThingCreatureBase { };

class CTCUserActionPointBase {
public:
    enum { INTERFACE_TYPE = 0x2d };
    bool GetFreeActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& orientation, long& index);   // 0x007FEDE0
};
class CTCCreatureNavigation {
public:
    enum { INTERFACE_TYPE = 7 };
    bool IsPosNavigable(const C3DVector& pos) const;    // 0x006CBA50
};
class CTCCarrying {
public:
    static bool __fastcall IsThingCarryingWeapon(const CThing& thing);   // 0x006A1540
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};

class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    static void __fastcall DeleteData(void* data);      // 0x008AA810 (<CModeSitBase>)
    __declspec(noinline) CCountedPointer(T* data)       // 0x008AB380 (<CModeSitBase>), same-TU stand-in
    {
        Data = data;
        if (data) {
            CCPPointerInfo* info = new CCPPointerInfo;
            if (info) {
                info->RefCount = 1;
                info->DeleteFunc = &DeleteData;
                info->Data = Data;
            }
            Info = info;
        } else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __declspec(noinline) ~CCountedPointer()             // 0x008AB3D0 (<CModeSitBase>), same-TU stand-in
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    CCharString   Group;                                // +0x04
    unsigned long GroupCRC;                             // +0x08
    CAnimFilterOfGroup(const CCharString& group)
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
    virtual bool operator()(const CAnimationEntry& entry) const;
};

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual ~CModeBase();
    };
};
namespace NCreatureMode {
class CModeSitBase : public CTCCreatureModeManager::CModeBase {
public:
    unsigned char _pad04[0x28];
    CModeSitBase(CThing& thing);                        // 0x008D7CA0
};
class CModeSitFloor : public CModeSitBase {
public:
    CAnimFilterOfGroup Filter;                          // +0x2c
    CModeSitFloor(CThing& thing, const CCharString& group) : CModeSitBase(thing), Filter(group) { }
    virtual ~CModeSitFloor();
};
}

class CCreatureActionBase : public CBaseObject {
public:
    unsigned char _pad04[0xa4];
};
class CCombatActionBase : public CCreatureActionBase {
public:
    unsigned char _pad_a8[0x114 - 0xa8];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_SheatheWeapons : public CCombatActionBase {
public:
    bool _flag114;
    CCreatureAction_SheatheWeapons(CThingCreatureBase& creature);   // 0x008CC370
};
class CCreatureAction_SitDown : public CCreatureActionBase {
public:
    CCountedPointer<NCreatureMode::CModeSitBase> PSitMode;   // +0xa8
    CCreatureAction_SitDown(CThingCreatureBase& creature, const CAnimationEntry& anim, CCountedPointer<NCreatureMode::CModeSitBase> mode);   // 0x008AB9D0
    virtual ~CCreatureAction_SitDown();                 // 0x008ABAA0
};

extern CCharString g_CCharString_013bb704;              // 0x013BB704 "ST_SIT_ON_FLOOR"

enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
enum EScriptAIPriority { };
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*         PBrain;                           // +0x04
    EScriptAIPriority ScriptAIPriority;                 // +0x08
    CCharString       Name;                             // +0x0C
    bool              Terminated;                       // +0x10
    unsigned char     Priority;                         // +0x11
    bool              Enabled;                          // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& arrived, EMovementType movement,
        bool a, bool b, const CThing* thing, bool c, float d);   // 0x0097EC00
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    EAIActionReturn AIAction_PerformAction(const CCreatureActionBase& action);   // 0x0097D8B0
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
};

class CAIStateGroup_BanditSitRoundCampfire : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThingObject> PCampfire;        // +0x14
    long          SeatPosIndex;                         // +0x1c
    bool          GotSeatPos;                           // +0x20
    bool          Sitting;                              // +0x21
    unsigned long FrameToNextSitOnSeat;                 // +0x24
    unsigned long FrameToNextCheckIsRunnable;           // +0x28
    bool          WasCarryingWeapon;                    // +0x2c
    virtual void Update();
private:
    bool SitMain();                                     // 0x0093AD70
};

void CAIStateGroup_BanditSitRoundCampfire::Update()
{
    FrameToNextCheckIsRunnable = CWorld::GetFrame() + 5 * CMainGameComponent::GetConstantFPS();
    C3DVector pos;
    CRightHandedSet orientation;
    bool arrived;
    CTCUserActionPointBase* actionPoints;
    PCampfire->GetTC(&actionPoints);
    if (!actionPoints->GetFreeActionPoint(GetCreature(), pos, orientation, SeatPosIndex)) {
        Terminate();
        return;
    }
    const CTCCreatureNavigation* navigation;
    GetCreature().PeekTC(&navigation);
    C2DVector dir = C2DVector(pos - PCampfire->GetPhysPos()).Normalise();
    for (long i = 0; i < 5; ++i) {
        C2DVector offset = dir * GFFloatRandom(1.0f, GetPWorldSeed());
        offset.Rotate(GFFloatRandom(1.0f, GetPWorldSeed()));
        if (navigation->IsPosNavigable(pos + offset)) {
            pos += offset;
            break;
        }
    }
    GotSeatPos = true;
    if (AIAction_MoveToPos((EAIActionRepeat)1, pos, arrived, (EMovementType)3, true, true, 0, true, 0.0f) != 2) {
        Terminate();
        return;
    }
    C3DVector forward = orientation.Forward;
    C3DVector lookAt = forward + GetCreature().PeekPos();
    if (AIAction_LookAtPos((EAIActionRepeat)1, lookAt, 0x11, 0.0027777f) != 2) {
        Terminate();
        return;
    }
    if ((WasCarryingWeapon = CTCCarrying::IsThingCarryingWeapon(GetCreature())) != false
        && AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()), false) != 2) {
        Terminate();
        return;
    }
    const CAnimationEntry& anim = GetCreature().GetRandomAnimFromNameWithFilter(g_CCharString_013bb704, CAnimFilterAnyGroup());
    CCountedPointer<NCreatureMode::CModeSitBase> sitMode(new NCreatureMode::CModeSitFloor(GetCreature(), anim.GroupName));
    Sitting = true;
    if (AIAction_PerformAction(CCreatureAction_SitDown(GetCreature(), anim, sitMode)) != 2) {
        Terminate();
        return;
    }
    SitMain();
    Terminate();
}