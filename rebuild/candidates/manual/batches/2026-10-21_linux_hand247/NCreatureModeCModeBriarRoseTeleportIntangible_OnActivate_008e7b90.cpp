// NCreatureMode::CModeBriarRoseTeleportIntangible::OnActivate() -- Briar Rose turns intangible: read the teleport timings from her def (defaults 6 s, no theme change, scale 1), set the mode's frame count, suspend a player creature's animation, clear the 0x3a flag and the type-9 component's flag, stop her movement and run the environment theme transition on all six channels
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E7B90, length 342 bytes (padded to 352), `ret`.
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CDefClassBase::GetSubDef<T>(CDefPointer<T>*) @ 0x008E7790 (manifest instantiation
//   name is a byte twin), the defs' virtual slot 1 (release), __ftol2 @ 0x00BFEA70, CThingAnimatable::SetSuspendAnimation @ 0x00833A10,
//   CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, types 0x3a and 9), the creature's virtual slot 28 (+0x70),
//   CModeBase::GetGlobalComponents @ 0x0073A680, CEnvironment::TransitionToTheme(long, long, long, float) @ 0x006B3800.
// Globals: game frames per second (long) @ 0x01375550.
// Layout: CModeBase ParentThing +4; the mode's frame count +0x74; the creature's def +0x70, type byte +0x10, presence bits +0x20,
//   interface map +0x44; the teleport sub-def: duration +0x2c, theme-change count +0x3c, scale +0x40; the global components'
//   environment +0x18.
// Shape: as the landed CCreatureAction_ScorpionKingDeath::Initialise 0x008C4170: the creature def is held in a by-value add-ref'ed
//   copy only around GetSubDef (bool result kept in bl across its release), the sub-def is copied out of the out slot into a holder
//   released at the end; GetSubDef has a same-TU stand-in body (out pointer does not escape, so the holder lives in ebx); the
//   six channel transitions run only for a positive count, through an inline CGlobalComponents::GetEnvironment() (the getter call
//   then precedes the argument pushes).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
template <class T> struct CDefCopy {            // by-value CDefPointer copy: add-ref on entry, release on exit
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefCopy()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CBriarRoseTeleportDef : CDefPointeeBase {
    unsigned char _pad08[0x24];
    float         Duration;                     // +0x2c
    unsigned char _pad30[0x0c];
    long          ThemeChangeCount;             // +0x3c
    float         ThemeScale;                   // +0x40
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CBriarRoseTeleportDef>* out);   // 0x008E7790
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x008C4170 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CBriarRoseTeleportDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CModeBase;
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void StopMoving(bool immediately);  // slot 28 (+0x70)
    unsigned char  _pad04[0x0c];
    unsigned char  m_type;                      // +0x10 (1 = player creature)
    unsigned char  _pad11[0x0f];
    unsigned long  m_presentBits[9];            // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x24];
    CDefClassBase* PDef;                        // +0x70
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
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
};
struct CTCFlagged {                             // the type-0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_flag : 1;                   // +0x50 bit 0
};
struct CTCType9 {                               // the type-9 component
    unsigned char _pad00[0x10];
    bool          m_active;                     // +0x10
};
struct CEnvironment {
    void TransitionToTheme(long channel, long mode, long count, float scale);   // 0x006B3800
};
struct CGlobalComponents {
    unsigned char _pad00[0x18];
    CEnvironment* Environment;                  // +0x18
    CEnvironment* GetEnvironment() const { return Environment; }
};
extern long g_GameFramesPerSecond;              // 0x01375550

struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
    CGlobalComponents* GetGlobalComponents() const;   // 0x0073A680
};
struct CModeStandBase : CModeBase {
    unsigned char _body[0x4c];
    void OnActivate();                          // 0x008621A0
};

namespace NCreatureMode {

struct CModeBriarRoseTeleportIntangible : CModeStandBase {
    unsigned char _pad68[0x0c];
    long          FrameCount;                   // +0x74
    void OnActivate();
};

void CModeBriarRoseTeleportIntangible::OnActivate()
{
    CModeStandBase::OnActivate();
    CThing* thing = ParentThing;
    float duration = 6.0f;
    long themeChanges = 0;
    float scale = 1.0f;
    CDefPointer<CBriarRoseTeleportDef> out;
    bool found;
    {
        CDefCopy<CDefClassBase> def(thing->PDef);
        found = def->GetSubDef(&out);
    }
    CDefHolder<CBriarRoseTeleportDef> teleport(out.Object);
    if (found) {
        themeChanges = teleport->ThemeChangeCount;
        duration = teleport->Duration;
        scale = teleport->ThemeScale;
    }
    FrameCount = (long)(g_GameFramesPerSecond * duration);
    if (ParentThing->m_type == 1)
        ParentThing->SetSuspendAnimation(true, this);
    CTCFlagged* flagged;
    if (ParentThing->GetInterface(&flagged, 0x3a))
        flagged->m_flag = 0;
    CTCType9* type9;
    if (ParentThing->GetInterface(&type9, 9))
        type9->m_active = false;
    ParentThing->StopMoving(false);
    if (themeChanges > 0) {
        for (long channel = 0; channel < 6; ++channel)
            GetGlobalComponents()->GetEnvironment()->TransitionToTheme(channel, 2, themeChanges, scale);
    }
}

}
