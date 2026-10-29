// CAIStateGroup_MazeTutorialBattleUseShield::IsRunnable() -- in boss phase 2, with a live combat target, runnable once 8 seconds have passed since the last shield: restamp the frame, fetch the whisper battle def and latch the target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954CD0, length 352 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xad = CTCBoss, bit 13 of +0x34) and its const
//          copy @ 0x004365B0 (interface 8, bit 8 of +0x20, the enemy tracker), CTCBoss::GetBossPhase @ 0x007AED10, the
//          tracker's virtual slot 29 (+0x74) GetTarget, CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame
//          @ 0x0049D870 (static), CDefClassBase::GetSubDef<CWhisperBattleDef> @ 0x00956590 on the creature def (+0x70),
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary vtable 0x01238C6C), def virtual slot 1.
// Globals: FramesPerSecond @ 0x01375550, 8.0f @ 0x012944C4.
// Shape: both lookups are the out-parameter GetInterface with the miss path reading the never-written slot (the key's
//          home); a same-TU GetSubDef stand-in fixes the holder zero store and the short ecx release; the second goes through the const overload (retail calls the TU's const LowerBound copy); the
//          elapsed frames are stored into that dead slot for the fild; the def holder is released by hand.
// Layout: engine/CAIStateGroup_MazeTutorialBattleUseShield.h (FrameOfLastShield +0x20); the target intelligent pointer
//          at +0x14 as in the landed sibling 0x00954B50; PBrain is the first dword at +0x04.
#include "engine/CAIStateGroup_MazeTutorialBattleUseShield.h"

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};
class CWhisperBattleDef : public CDefPointee { public: };
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CWhisperBattleDef>* out);   // 0x00956590 (GetSubDef<CWhisperBattleDef>)
};

void* FindDef_GetSubDef();                      // stand-in callee of the stand-in body below
// Stand-in body (the real GetSubDef<CWhisperBattleDef> is the landed 0x00956590): it reads the old out pointer and
// writes the new one, so VC7.1 sees the holder's address does not escape (the retry-10f GetSubDef idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CWhisperBattleDef>* out)
{
    void* def = FindDef_GetSubDef();
    if (!def)
        return false;
    if (out->Object)
        out->Object->Release();
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad2[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
    }
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
        }
    }
};

#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct CTCEnemyTracker {                        // interface 8
    V16(a) V4(b) V4(c)
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget();                // slot 29 (+0x74)
};
class CTCBoss { public:                                // interface 0xad
    long GetBossPhase();                        // 0x007AED10
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
extern long FramesPerSecond;                    // 0x01375550

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    __forceinline CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupMazeTutorialBattleUseShieldMethods : CAIStateGroup_MazeTutorialBattleUseShield {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointer<CThing>& PTarget() { return *(CIntelligentPointer<CThing>*)_pad_0x14; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsRunnable();
};

bool CAIStateGroupMazeTutorialBattleUseShieldMethods::IsRunnable()
{
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
    if (boss->GetBossPhase() == 2) {
        CTCEnemyTracker* tracker;
        ((const CThing*)GetCreature())->GetInterface(&tracker, 8);
        CThing* target = tracker->GetTarget();
        if (target && !(target->m_flags91 & 1)) {
            float elapsed = (float)(GetPWorld()->GetFrame() - FrameOfLastShield);
            if (elapsed > (float)FramesPerSecond * 8.0f) {
            FrameOfLastShield = GetPWorld()->GetFrame();
            CDefPointer<CWhisperBattleDef> def;
            GetCreature()->PDef->GetSubDef(&def);
            PTarget() = CIntelligentPointer<CThing>(target);
            if (def.Object)
                def.Object->Release();
            return true;
            }
        }
    }
    return false;
}