// Grade: DIFFER(613v588) -- 156 difflines: frame 0x24 v 0x20 and an ebx/ebp role swap (retail keeps &TurningAnims in ebx and reloads PAppearanceDef into ebp before the slot-4 GetAnimFilter call); retail also builds the CTurningAnim pair before the push_back capacity test and tests the GetInterface result with `test eax,eax`. Tried: ternary and if-form GetInterface, sign via `!(rotation < 0)`, inline push_back over a same-TU _M_insert_overflow stand-in.
// CTCLook::UpdateTurningAnims() -- rebuild the table of turning animations: collect the appearance's "TURNING_" animations for the current mode's filters (or with no group filter when none match), keep each one that rotates the creature, store its clamped signed rotation with it, and track the smallest rotation as the minimum turn handled by animation
// Leaf: UpdateTurningAnims
// Retail: Fable.exe (TLC, VC7.1) @ 0x00754050, 588 bytes, plain `ret`.
// Callees: LowerBound 0x0040F020 (interface 0x31, the creature mode manager: word 1 bit 17), CTCCreatureModeManager::
//   GetCurrentMode 0x0073A6E0 (x3), the mode's virtual slots 1 (+4, result unused) and 4 (+0x10, its animation filter),
//   vector<pair<float, const CAnimationEntry*> >::clear 0x00755320 (out of line) and _M_insert_overflow 0x00755350,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CAnimationSet::GetAnimsWithNameStartingWith
//   0x005DCB00 (x3), CAnimationEntry::GetRotationInAnimation 0x005DBFD0, free 0x00BFEA14. "TURNING_" 0x0125AB08;
//   CAnimFilterNoGroup vtable 0x0122FFDC (base CAnimFilterBase 0x0122FFCC); 0.5f 0x0122F59C, g_FloatEpsilon 0x0129BA3C,
//   0.0f 0x0122DEDC.
// Layout: retail CTCLook (PDB minus 4 after the 12-byte TurningAnims vector, as the landed CTCLook rows): PAppearanceDef
//   +0x18, TurningAnims +0x8c, TurningViaAnimMinAngle +0x98; the appearance def's CAnimationSet +0x34 (landed 0x00662C10);
//   the mode's second filter at +8.
#include <math.h>
#include <new>
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
extern const float g_FloatEpsilon;                           // 0x0129BA3C
struct CAnimationEntry {
    void GetRotationInAnimation(float* pRotation) const;     // 0x005DBFD0
};
struct CAnimationEntryVector {                               // vector<const CAnimationEntry*> (12 bytes)
    const CAnimationEntry** _M_start;
    const CAnimationEntry** _M_finish;
    const CAnimationEntry** _M_end_of_storage;
    CAnimationEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CAnimationEntryVector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
};
struct CAnimFilterBase {                                     // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
};
struct CAnimFilterNoGroup : CAnimFilterBase {                // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
};
struct CAnimationSet {
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims,
                                      const CAnimFilterBase& filter) const;     // 0x005DCB00
};
struct CAppearanceDef {
    unsigned char _pad00[0x34];
    CAnimationSet AnimationSet;                              // +0x34
};
struct CCreatureMode {
    virtual void v0();
    virtual long GetModeType();                              // slot 1 (+4)
    virtual void v2(); virtual void v3();
    virtual const CAnimFilterBase& GetAnimFilter();          // slot 4 (+0x10)
    CAnimFilterBase* _pad04;
    CAnimFilterBase  SecondFilter;                           // +0x08
};
struct CTCCreatureModeManager {
    CCreatureMode* GetCurrentMode();                         // 0x0073A6E0
};
struct CTCBaseComponent;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
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
};
struct __true_type { };
struct CTurningAnim {                                        // pair<float, const CAnimationEntry*>
    float first;
    const CAnimationEntry* second;
    CTurningAnim(float a, const CAnimationEntry* b) : first(a), second(b) { }
};
struct CTurningAnimVector {                                  // vector<pair<float, const CAnimationEntry*> > (12 bytes)
    CTurningAnim* _M_start;
    CTurningAnim* _M_finish;
    CTurningAnim* _M_end_of_storage;
    void clear();                                            // 0x00755320
    void _M_insert_overflow(CTurningAnim* position, const CTurningAnim& x, const __true_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00755350
    void push_back(const CTurningAnim& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CTurningAnim(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct CTCLook {
    void*           __vftable;
    CThing*         ParentThing;                             // +0x04
    unsigned char   _pad08[0x18 - 0x08];
    const CAppearanceDef* PAppearanceDef;                    // +0x18 (CDefPointer)
    unsigned char   _pad1c[0x8c - 0x1c];
    CTurningAnimVector TurningAnims;                         // +0x8c
    float           TurningViaAnimMinAngle;                  // +0x98
    void UpdateTurningAnims();
};

void CTCLook::UpdateTurningAnims()
{
    CTCCreatureModeManager* modeManager;
    if (!ParentThing->GetInterface(&modeManager, 0x31))
        return;
    if (!PAppearanceDef)
        return;
    modeManager->GetCurrentMode()->GetModeType();
    TurningAnims.clear();
    TurningViaAnimMinAngle = 0.5f;
    CAnimationEntryVector anims;
    PAppearanceDef->AnimationSet.GetAnimsWithNameStartingWith("TURNING_", anims,
                                                              modeManager->GetCurrentMode()->GetAnimFilter());
    PAppearanceDef->AnimationSet.GetAnimsWithNameStartingWith("TURNING_", anims,
                                                              modeManager->GetCurrentMode()->SecondFilter);
    if (anims.empty())
        PAppearanceDef->AnimationSet.GetAnimsWithNameStartingWith("TURNING_", anims, CAnimFilterNoGroup());
    for (const CAnimationEntry** it = anims._M_start; it != anims._M_finish; ++it) {
        const CAnimationEntry* anim = *it;
        float rotation;
        anim->GetRotationInAnimation(&rotation);
        if (fabs(rotation) > g_FloatEpsilon) {
            TurningAnims.push_back(CTurningAnim(rotation, anim));
            float maxRotation = 0.5f - g_FloatEpsilon;
            float size = fabs(rotation);
            if (size > maxRotation)
                size = maxRotation;
            long sign = -1;
            if (!(rotation < 0.0f))
                sign = 1;
            rotation = sign * size;
            if (fabs(rotation) < TurningViaAnimMinAngle)
                TurningViaAnimMinAngle = fabs(rotation);
        }
    }
}
