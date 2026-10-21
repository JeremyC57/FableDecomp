// CTCDoor::Open(bool) -- start the door opening: play the open sound and the def's open animation, switch the navigation
// blocker and the obstacle flag, optionally fire the def's open event on the thing, and force a frame update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730C00, 454 bytes (+ int3 padding), `ret 4`.
// Leaf: Open
// Grade: DIFFER(456v456) -- one register pick in the inline base-def temp copy+release: retail counts in ecx
// (`mov ecx,[esi+4] ; inc ecx ; mov eax,ecx`), ours reuses edx (the parent's register); everything else byte-identical.
// Layout: engine/CTCDoor.h (+0x04 m4 = CTCBase::ParentThing, +0x12 Opening, +0x13 Closing, +0x20 PDef); CThing +0x60 event sink,
// +0x70 PBaseDef (retail); CDoorDef +0x30/+0x34 open/close event words (names inferred).
#include "engine/CTCDoor.h"

struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
extern const CCharString g_DoorOpenAnimName;    // 0x013BAD34
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
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
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
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer()
    {
        if (Object)
            if (--Object->RefCount == 0)
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
struct CThingSoundEmitter;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    unsigned char _pad4c[0x60 - 0x4c];
    CThingEventSink* PEventSink;                // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefPointer<CThingBaseDef> PBaseDef;        // +0x70
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
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
    unsigned char _pad00[0x30];
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
    void PlaySoundW(const CCharString& soundName);   // 0x00730A00
    void Open(bool fireEvent);
};

void CTCDoor_Methods::Open(bool fireEvent)
{
    CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
    Opening = true;
    Closing = false;
    {
        CCharString sound("DOOR_OPEN");
        PlaySoundW(sound);
    }
    const CThingBaseDef* baseDef = Parent()->GetPBaseDef().Object;
    CDefPointer<CAnimatingObjectDef> def;
    baseDef->GetSubDef(&def);
    const CAnimationEntry* entry = def->Animation.GetAnimation(g_DoorOpenAnimName, 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry->AnimIndex, 1, false, 0, 100);
    animate.SetMainAnim(init, 1, 0);
    CTCSwitchableNavigation* navigation;
    if (Parent()->GetInterface(&navigation, 0x79))
        navigation->SetBlocking(false);
    CTCObstacle* obstacle;
    if (Parent()->GetInterface(&obstacle, 9))
        obstacle->Enabled = false;
    if (fireEvent) {
        long event = Def()->OpenEvent;
        if (event) {
            CThingEventSink* sink = Parent()->PEventSink;
            if (sink)
                sink->TriggerEvent(event);
        }
    }
    Parent()->SetForceFrameUpdate(true);
}
