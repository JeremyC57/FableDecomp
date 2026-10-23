// CTCDoor::OnInitialActivate() -- set the door up when first activated: look up its def name, snap the animation to the end of the open or close animation (per CurrentOpen), switch the navigation blocker and fire the def's open/close event, update a building's damage state and keep proximity doors frame-updating
// Leaf: OnInitialActivate
//
// Retail 0x007306C0, 559 bytes, plain `ret`.
// Callees: CThing::GetDefName @ 0x004C7CC0 (CDefString by hidden pointer), CDefStringTable::GetString @ 0x009D49B0
// (g_DefStringTable 0x013CA828), CCharString copy ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, CVectorMap LowerBound @ 0x0040F020
// (0x5a CTCAnimationComplex, 0x79 CTCSwitchableNavigation), CTCAnimationComplex::GetAnimation @ 0x0070B460,
// CAnimateThing::Reset @ 0x0070EC30 / SetMainAnim(const CAnimInit&) @ 0x0070D920, CThingBaseDef::GetSubDef
// <CAnimatingObjectDef> @ 0x0042B0A2, CAnimationSet::GetAnimation @ 0x005DC340 (CAnimFilterNoGroup temporary),
// CAnimInit::Set @ 0x0070C050, CTCSwitchableNavigation::SetBlocking @ 0x00787580, the event sink's slot 85, the thing's
// slot 59, CThing::SetForceFrameUpdate @ 0x004C7C60.
// Globals: g_DoorOpenAnimName 0x013BAD34, g_DoorCloseAnimName 0x013BAD38 (CCharString).
// Model: the landed CTCDoor::Open (0x00730C00): engine/CTCDoor.h (+0x04 ParentThing, +0x10 CurrentOpen, +0x20 PDef);
// CDoorDef TriggerType +0x28, open/close events +0x30/+0x34; CThing type byte +0x10, event sink +0x60, PBaseDef +0x70.
#include "engine/CTCDoor.h"

struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                     // 0x0099EC30
    ~CCharString();                                            // 0x0099EAE0
};
extern const CCharString g_DoorOpenAnimName;    // 0x013BAD34
extern const CCharString g_DoorCloseAnimName;   // 0x013BAD38
struct CDefString {
    long Index;
    CDefString() {}
};
struct CDefStringTable {
    CCharString GetString(CDefString name);                    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                       // 0x013CA828
struct CAnimationEntry { long AnimIndex; };
struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
struct CAnimationSet {
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                              // 0x2c bytes
    long          Mode;
    unsigned char AnimType;
    bool          Flag05;
    long          AnimIndex;
    float         Scales[3];
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;
    long          Arg24;
    long          Percent;
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimateThing {
    unsigned char _pad00[0x1c];
    float CurrentPercentageComplete;            // +0x1c
    float LastPercentageComplete;               // +0x20
    void Reset();                               // 0x0070EC30
    void SetMainAnim(const CAnimInit& init);    // 0x0070D920 (forwards to the 3-argument SetMainAnim 0x0070D580 with 1, 0)
};
struct CTCAnimationComplex {
    CAnimateThing& GetAnimation();              // 0x0070B460
};
struct CTCSwitchableNavigation { void SetBlocking(bool blocking); };   // 0x00787580
struct CTCObstacle {                            // interface 9
    unsigned char _pad00[0x10];
    bool          Enabled;                      // +0x10
};
struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
__forceinline bool DecRefToZero(long& c) { return --c == 0; }
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    operator T*() const { return Object; }
    ~CDefPointer()
    {
        if (Object)
            if (DecRefToZero(Object->RefCount))
                Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CAnimatingObjectDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAnimatingObjectDef>* out) const;   // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef();         // stand-in callee of the stand-in body below
struct CThingEventSink {                        // the thing's +0x60 object; slot 85 (+0x154) takes the def's event word
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void TriggerEvent(long event);      // slot 85
};
struct CThing {
    virtual void t00();
    virtual void t01();
    virtual void t02();
    virtual void t03();
    virtual void t04();
    virtual void t05();
    virtual void t06();
    virtual void t07();
    virtual void t08();
    virtual void t09();
    virtual void t10();
    virtual void t11();
    virtual void t12();
    virtual void t13();
    virtual void t14();
    virtual void t15();
    virtual void t16();
    virtual void t17();
    virtual void t18();
    virtual void t19();
    virtual void t20();
    virtual void t21();
    virtual void t22();
    virtual void t23();
    virtual void t24();
    virtual void t25();
    virtual void t26();
    virtual void t27();
    virtual void t28();
    virtual void t29();
    virtual void t30();
    virtual void t31();
    virtual void t32();
    virtual void t33();
    virtual void t34();
    virtual void t35();
    virtual void t36();
    virtual void t37();
    virtual void t38();
    virtual void t39();
    virtual void t40();
    virtual void t41();
    virtual void t42();
    virtual void t43();
    virtual void t44();
    virtual void t45();
    virtual void t46();
    virtual void t47();
    virtual void t48();
    virtual void t49();
    virtual void t50();
    virtual void t51();
    virtual void t52();
    virtual void t53();
    virtual void t54();
    virtual void t55();
    virtual void t56();
    virtual void t57();
    virtual void t58();
    virtual void SetDamageState(float damage, bool b);   // slot 59 (+0xec, descriptive)
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingType;                    // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    unsigned char _pad4c[0x60 - 0x4c];
    CThingEventSink* PEventSink;                // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefPointer<CThingBaseDef> PBaseDef;        // +0x70
    unsigned char _pad74[0xb0 - 0x74];
    float MaxHealthLike;                        // +0xb0 (descriptive)
    float HealthLike;                           // +0xb4 (descriptive)
    CDefString GetDefName() const;              // 0x004C7CC0
    CCharString GetDefNameString() const
    {
        CCharString name = g_DefStringTable.GetString(GetDefName());
        return name;
    }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    CDefPointer<CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    void SetForceFrameUpdate(bool force);       // 0x004C7C60
};
// Stand-in body (the real 0x0042B0A2 is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x007F90B0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAnimatingObjectDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CDoorDef {
    unsigned char _pad00[0x28];
    long TriggerType;                           // +0x28 EDoorTriggerType
    unsigned char _pad2c[0x30 - 0x2c];
    long OpenEvent;                             // +0x30
    long CloseEvent;                            // +0x34
};

struct CTCDoor_Methods : CTCDoor {
    CThing* Parent() const { return (CThing*)m4; }
    const CDoorDef* Def() const { return (const CDoorDef*)PDef_Object; }
    __forceinline CTCAnimationComplex* GetAnimationComplex() const
    {
        CTCAnimationComplex* complex;
        if (Parent()->HasTC(0x5a))
            complex = Parent()->GetTC<CTCAnimationComplex>(0x5a);
        return complex;
    }
    void OnInitialActivate();
};

void CTCDoor_Methods::OnInitialActivate()
{
    CCharString defName = Parent()->GetDefNameString();
    CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
    animate.Reset();
    CThingBaseDef* baseDef = Parent()->GetPBaseDef();
    CDefPointer<CAnimatingObjectDef> def;
    baseDef->GetSubDef(&def);
    const CAnimationEntry* entry = def->Animation.GetAnimation(CurrentOpen ? g_DoorOpenAnimName : g_DoorCloseAnimName, 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry->AnimIndex, 1, false, 0, 100);
    animate.SetMainAnim(init);
    animate.CurrentPercentageComplete = 1.0f;
    animate.LastPercentageComplete = 1.0f;
    if (CurrentOpen) {
        CTCSwitchableNavigation* navigation;
        if (Parent()->GetInterface(&navigation, 0x79))
            navigation->SetBlocking(false);
        long event = Def()->OpenEvent;
        if (event)
            Parent()->PEventSink->TriggerEvent(event);
    } else {
        CTCSwitchableNavigation* navigation;
        if (Parent()->GetInterface(&navigation, 0x79))
            navigation->SetBlocking(true);
        long event = Def()->CloseEvent;
        if (event)
            Parent()->PEventSink->TriggerEvent(event);
    }
    CThing* parent = Parent();
    if (parent->ThingType == 5)
        parent->SetDamageState(parent->MaxHealthLike - parent->HealthLike, false);
    if (Def()->TriggerType == 0)
        Parent()->SetForceFrameUpdate(true);
}
