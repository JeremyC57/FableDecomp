#include <stdio.h>
#include <string.h>
// CTCDoor::Open(bool) -- start the door opening: play the open sound and the def's open animation, switch the navigation
// blocker and the obstacle flag, optionally fire the def's open event on the thing, and force a frame update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730C00, 454 bytes (+ int3 padding), `ret 4`.
// Leaf: Open
// Shape: (AdvB + B) the base def is read through `CThingBaseDef* baseDef = Parent()->GetPBaseDef();` (CDefPointer
//   `operator T*`: the by-value temporary is converted and released at the `;`, giving retail's folded inc/dec
//   copy+release before GetSubDef), `def` declared after that line, DecRefToZero(long&) in the release, and the
//   GetInterface out-store as one ternary (this in edi, map in esi as retail).
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
    CThingBaseDef* baseDef = Parent()->GetPBaseDef();
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


// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static char g_log[512];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static const char* g_lastText;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; g_lastText = text; }
CCharString::~CCharString() {}
const CCharString g_DoorOpenAnimName("ANIM");
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
static CAnimationEntry g_entry = { 17 };
static const CCharString* g_animName; static long g_animIndex; static const void* g_animSet;
const CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase&) const
{ g_animName = &name; g_animIndex = index; g_animSet = this; L("getanim"); return &g_entry; }
static long g_setArgs[7];
void CAnimInit::Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
{ g_setArgs[0] = animType; g_setArgs[1] = arg24; g_setArgs[2] = animIndex; g_setArgs[3] = mode; g_setArgs[4] = flag05; g_setArgs[5] = (long)flags; g_setArgs[6] = percent; }
static CAnimateThing g_animate; static long g_mainMode, g_mainDelay;
void CAnimateThing::SetMainAnim(const CAnimInit&, long mode, long delay) { g_mainMode = mode; g_mainDelay = delay; L("setmain"); }
static void* g_complexThis;
CAnimateThing& CTCAnimationComplex::GetAnimation() { g_complexThis = this; L("complex"); return g_animate; }
static int g_blocking = -1;
void CTCSwitchableNavigation::SetBlocking(bool b) { g_blocking = b; L("blocking"); }
static int g_destroys;
void CDefPointeeBase::Destroy0() {}
void CDefPointeeBase::Destroy() { g_destroys++; }
static void* g_subDef;
void* FindDef_CThingBaseDef_GetSubDef() { L("subdef"); return g_subDef; }
void CThingEventSink::v00() {}
void CThingEventSink::v01() {}
void CThingEventSink::v02() {}
void CThingEventSink::v03() {}
void CThingEventSink::v04() {}
void CThingEventSink::v05() {}
void CThingEventSink::v06() {}
void CThingEventSink::v07() {}
void CThingEventSink::v08() {}
void CThingEventSink::v09() {}
void CThingEventSink::v10() {}
void CThingEventSink::v11() {}
void CThingEventSink::v12() {}
void CThingEventSink::v13() {}
void CThingEventSink::v14() {}
void CThingEventSink::v15() {}
void CThingEventSink::v16() {}
void CThingEventSink::v17() {}
void CThingEventSink::v18() {}
void CThingEventSink::v19() {}
void CThingEventSink::v20() {}
void CThingEventSink::v21() {}
void CThingEventSink::v22() {}
void CThingEventSink::v23() {}
void CThingEventSink::v24() {}
void CThingEventSink::v25() {}
void CThingEventSink::v26() {}
void CThingEventSink::v27() {}
void CThingEventSink::v28() {}
void CThingEventSink::v29() {}
void CThingEventSink::v30() {}
void CThingEventSink::v31() {}
void CThingEventSink::v32() {}
void CThingEventSink::v33() {}
void CThingEventSink::v34() {}
void CThingEventSink::v35() {}
void CThingEventSink::v36() {}
void CThingEventSink::v37() {}
void CThingEventSink::v38() {}
void CThingEventSink::v39() {}
void CThingEventSink::v40() {}
void CThingEventSink::v41() {}
void CThingEventSink::v42() {}
void CThingEventSink::v43() {}
void CThingEventSink::v44() {}
void CThingEventSink::v45() {}
void CThingEventSink::v46() {}
void CThingEventSink::v47() {}
void CThingEventSink::v48() {}
void CThingEventSink::v49() {}
void CThingEventSink::v50() {}
void CThingEventSink::v51() {}
void CThingEventSink::v52() {}
void CThingEventSink::v53() {}
void CThingEventSink::v54() {}
void CThingEventSink::v55() {}
void CThingEventSink::v56() {}
void CThingEventSink::v57() {}
void CThingEventSink::v58() {}
void CThingEventSink::v59() {}
void CThingEventSink::v60() {}
void CThingEventSink::v61() {}
void CThingEventSink::v62() {}
void CThingEventSink::v63() {}
void CThingEventSink::v64() {}
void CThingEventSink::v65() {}
void CThingEventSink::v66() {}
void CThingEventSink::v67() {}
void CThingEventSink::v68() {}
void CThingEventSink::v69() {}
void CThingEventSink::v70() {}
void CThingEventSink::v71() {}
void CThingEventSink::v72() {}
void CThingEventSink::v73() {}
void CThingEventSink::v74() {}
void CThingEventSink::v75() {}
void CThingEventSink::v76() {}
void CThingEventSink::v77() {}
void CThingEventSink::v78() {}
void CThingEventSink::v79() {}
void CThingEventSink::v80() {}
void CThingEventSink::v81() {}
void CThingEventSink::v82() {}
void CThingEventSink::v83() {}
void CThingEventSink::v84() {}
static long g_event;
void CThingEventSink::TriggerEvent(long event) { g_event = event; L("event"); }
static int g_force = -1;
void CThing::SetForceFrameUpdate(bool force) { g_force = force; L("force"); }
static const char* g_sound;
void CTCDoor_Methods::PlaySoundW(const CCharString& s) { g_sound = (const char*)s.PStringData; L("sound"); }
static unsigned char g_thingBuf[0x100], g_doorBuf[0x40];
static void Reset() { g_log[0] = 0; g_event = 0; g_force = -1; g_blocking = -1; g_destroys = 0; g_sound = 0; }
int main()
{
    CThing* thing = (CThing*)g_thingBuf;
    CTCDoor_Methods* door = (CTCDoor_Methods*)g_doorBuf;
    door->m4 = (long)thing;
    static CTCAnimationComplex complex; static CTCSwitchableNavigation nav; static CTCObstacle obstacle;
    CInterfaceMap::SEntry e[] = { { 9, (CTCBaseInterface*)&obstacle }, { 0x5a, (CTCBaseInterface*)&complex }, { 0x79, (CTCBaseInterface*)&nav } };
    thing->m_interfaces.first = e; thing->m_interfaces.last = e + 3;
    thing->m_presentBits[0] = 1UL << 9; thing->m_presentBits[0x5a >> 5] |= 1UL << (0x5a & 31); thing->m_presentBits[0x79 >> 5] |= 1UL << (0x79 & 31);
    static CThingBaseDef baseDef; static CAnimatingObjectDef animDef;
    baseDef.RefCount = 1; *(CThingBaseDef**)&thing->PBaseDef = &baseDef;
    animDef.RefCount = 2; g_subDef = &animDef;
    static unsigned char doorDef[0x40]; *(long*)(doorDef + 0x30) = 0x55; *(long*)(doorDef + 0x34) = 0x66;
    door->PDef_Object = (CDefPointeeBase*)doorDef;
    static CThingEventSink sink; thing->PEventSink = &sink;
    door->Opening = false; door->Closing = true; obstacle.Enabled = true; Reset();
    door->Open(true);
    CHECK(strcmp(g_log, "complex;sound;subdef;getanim;setmain;blocking;event;force;") == 0);
    CHECK(g_complexThis == &complex && strcmp(g_sound, "DOOR_OPEN") == 0 && g_animName == &g_DoorOpenAnimName && g_animIndex == 0 && g_animSet == (char*)&animDef + 0x34);
    CHECK(g_setArgs[0] == 6 && g_setArgs[1] == 0 && g_setArgs[2] == 17 && g_setArgs[3] == 1 && g_setArgs[4] == 0 && g_setArgs[5] == 0 && g_setArgs[6] == 100);
    CHECK(g_mainMode == 1 && g_mainDelay == 0 && g_force == 1);
    CHECK(door->Opening == true && door->Closing == false && g_blocking == 0 && obstacle.Enabled == false && g_event == 0x55);
    CHECK(baseDef.RefCount == 1 && animDef.RefCount == 1 && g_destroys == 0);
    // no navigation/obstacle interfaces, no event word: only the animation, the frame update and the def release
    thing->m_presentBits[0] = 0; thing->m_presentBits[0x79 >> 5] &= ~(1UL << (0x79 & 31));
    *(long*)(doorDef + 0x30) = 0; *(long*)(doorDef + 0x34) = 0; animDef.RefCount = 1; Reset();
    door->Open(true);
    CHECK(strcmp(g_log, "complex;sound;subdef;getanim;setmain;force;") == 0 && g_destroys == 1 && animDef.RefCount == 0 && g_blocking == -1);
    // event word set but no event sink
    *(long*)(doorDef + 0x30) = 0x55; *(long*)(doorDef + 0x34) = 0x66; thing->PEventSink = 0; animDef.RefCount = 2; Reset();
    door->Open(true);
    CHECK(strcmp(g_log, "complex;sound;subdef;getanim;setmain;force;") == 0 && g_event == 0);
    // fireEvent false: no event even with a sink
    thing->PEventSink = &sink; Reset();
    door->Open(false);
    CHECK(strcmp(g_log, "complex;sound;subdef;getanim;setmain;force;") == 0 && g_event == 0);
    if (fails == 0) printf("00730c00_TEST PASS\n"); else printf("00730c00_TEST FAIL\n");
    return fails;
}