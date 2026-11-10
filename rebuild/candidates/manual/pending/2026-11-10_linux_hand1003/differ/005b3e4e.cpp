// CTCInventoryClothing::GlobalUpdate(bool) -- base update, then keep the mannequin playing DEFAULT (or EMBARRASSED_TWIRL when it wears nothing covering) and advance its animation one frame
// Grade: DIFFER(538v543) -- residue: on this LOCAL model (no shared headers exist for CAnimateThing / CAnimInit / CAnimSet / CTCAnimationComplex / CAppearanceDef / CAnimationSet / CAnimateThingUpdate) the if-statement empty() test and `anim = defaultAnim` initialiser now give retail's sete-bl-before-dtor shape, but the frame is 0x54 (retail 0x58: init / update / frame-temp words), twirlAnim homes at [ebp-0x20] (retail -0x1C) and `this` is reloaded from its spill into eax, not edi; 1 grade this session (prior 16)
// Leaf: ?GlobalUpdate@CTCInventoryClothing@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3E4E, 543 bytes, thiscall `leave ; ret 4`, frame 0x58. Ego_r.pdb / FableWin.pdb
//   ?GlobalUpdate@CTCInventoryClothing@@UAEX_N@Z (27 name-set hits). Callee spellings (Ego_r / FableWin): CTCInventoryBase::GlobalUpdate
//   0x005BDD39, ?GetSubDef<CAppearanceDef> 0x0042B0A2, ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z
//   0x005DC340 (returns a reference), ?GetAppearanceModifierListWithCoversBodyAreaFlags@CTCHeroAttachableAppearanceModifiers@@QBE?AV?$list@J..
//   0x00706B50, ?EditFrameUpdate@CTCHeroAttachableAppearanceModifiers@@UAEXXZ 0x00707930 (called direct), ?GetAnimation@CTCAnimationComplex@@QAEAAVCAnimateThing@@XZ
//   0x0070B460, ?GetAnimateThingFlags@CAnimationEntry@@QBEKXZ 0x005DC390, ?GetTransitionTime@CAnimationEntry@@QBEJXZ 0x005DC1D0,
//   ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z 0x0070C050, ?SetMainAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ@Z 0x0070D580,
//   0x0070E8B0 = CAnimateThing update taking a 12-byte CAnimateThingUpdate (TimePassed / DistanceMoved / PercentageComplete; Ego_r
//   ?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z is the likely name, unproven), g_FrameRate 0x01375550.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "rebuild_abi.h"
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CAnimationEntry {
    long AnimIndex;                                            // +0x00
    unsigned long GetAnimateThingFlags() const;                // 0x005DC390
    long GetTransitionInTime() const;                          // 0x005DC1D0
};
struct CAnimFilterBase {                                       // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {                  // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
};
struct CAnimationSet {
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                                    // slot 1
    long RefCount;                                             // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    FABLE_FORCEINLINE ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                                   // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();
#pragma auto_inline(off)
bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CAnimFrameInfo {
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
    CAnimFrameInfo(const CAnimFrameInfo& o) : Current(o.Current), Last(o.Last) {}
};
struct CAnimInit {                                             // 0x2c bytes
    long          UpdateMode;
    unsigned char Type;
    bool          Looping;
    long          Anim;
    float         Speed;
    float         Alpha;
    float         LastAlpha;
    CAnimFrameInfo Frame;                                      // +0x18
    unsigned long Flags;
    unsigned long TypeFlags;
    long          Priority;
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
              long priority)
    {
        Set(type, typeFlags, anim, updateMode, looping, flags, priority);
    }
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
             long priority);                                   // 0x0070C050
};
struct CAnimFrameUpdate {
    float Time;
    float Reserved0;
    float Reserved1;
    CAnimFrameUpdate(float time) { Time = time; Reserved0 = 0; Reserved1 = 0; }
};
struct CAnimRef { short Index; };
struct CAnimSet {
    long          UpdateMode;                                  // +0x00
    CAnimRef*     Anims;                                       // +0x04 vector _M_start
    unsigned char _pad08[0x1c - 0x08];
    CAnimFrameInfo Frame;                                      // +0x1c
};
struct CAnimateThing {
    CAnimSet MainAnimSet;
    CAnimFrameInfo GetFrameInfo() const { return MainAnimSet.Frame; }
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    void UpdateFrameLogic(const CAnimFrameUpdate& update);     // 0x0070E8B0
};
struct CTCAnimationComplex {
    CAnimateThing& GetAnimation();                             // 0x0070B460
};
struct SLongListNode { SLongListNode* _M_next; SLongListNode* _M_prev; long _M_data; };
struct CLongList {
    SLongListNode* _M_node;
    ~CLongList();                                              // 0x0042AB73
    bool empty() const { return _M_node->_M_next == _M_node; }
};
struct CTCHeroAttachableAppearanceModifiers {                  // interface 0x5E
    CLongList GetAppearanceModifierListWithCoversBodyAreaFlags(long flags, long type) const;   // 0x00706B50
    void EditFrameUpdate();                                    // 0x00707930
};
struct CPresenceBits { bool test(long bit); };                 // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                      // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                                  // +0x20
    unsigned char  _pad21[0x23];
    CInterfaceMap  m_interfaces;                               // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CThingBaseDef* PBaseDef;                                   // +0x70
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
extern long g_FrameRate;                                       // 0x01375550

struct CTCInventoryBase {
    virtual void GlobalUpdate(bool flag);                      // 0x005BDD39
};
// Layout: retail CTCInventoryClothing (12-byte containers): Active +0x58, PMannequin +0x158.
struct CTCInventoryClothing : CTCInventoryBase {
    unsigned char _pad04[0x54];
    bool    Active;                                            // +0x58
    unsigned char _pad59[0x158 - 0x59];
    CThing* PMannequin;                                        // +0x158
    virtual void GlobalUpdate(bool flag);
};
void CTCInventoryClothing::GlobalUpdate(bool flag)
{
    CTCInventoryBase::GlobalUpdate(flag);
    if (Active && PMannequin) {
        CDefPointer<CAppearanceDef> appearance;
        CDefPointer<CThingBaseDef>(PMannequin->PBaseDef)->GetSubDef(&appearance);
        const CAnimationEntry* defaultAnim = appearance->Animation.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
        const CAnimationEntry* twirlAnim = appearance->Animation.GetAnimation("EMBARRASSED_TWIRL", 0, CAnimFilterNoGroup());
        const CAnimationEntry* anim = defaultAnim;
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (PMannequin->GetInterface(&modifiers, 0x5e)) {
            if (modifiers->GetAppearanceModifierListWithCoversBodyAreaFlags(4, 2).empty())
                anim = twirlAnim;
            else
                anim = defaultAnim;
            modifiers->EditFrameUpdate();
        }
        CTCAnimationComplex* complex = 0;
        PMannequin->GetInterface(&complex, 0x5a);
        CAnimateThing& animator = complex->GetAnimation();
        if (animator.MainAnimSet.Anims[0].Index != anim->AnimIndex) {
            CAnimInit init(6, 0, anim->AnimIndex, 1, true, anim->GetAnimateThingFlags(), 100);
            init.Frame = animator.GetFrameInfo();
            animator.SetMainAnim(init, anim->GetTransitionInTime(), 0);
        }
        animator.UpdateFrameLogic(CAnimFrameUpdate(1.0f / g_FrameRate));
    }
}
