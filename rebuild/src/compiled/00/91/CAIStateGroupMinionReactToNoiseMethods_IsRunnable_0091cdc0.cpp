// CAIStateGroup_MinionReactToNoise::IsRunnable() -- within two minutes of the last interaction, when not already investigating: a perceived noise maker inside the creature's home with no earlier noise message from home starts an investigation (records the noise position and posts a 2-second noise message)
//   Bind2 first-pointer store hoist above the GetPItem call as in retail), a local copy of FramesPerSecond taken before the
//   AddMessage argument calls (retail spills it), and two unused named float locals at entry (E-19 symbol-count re-key: fiadd)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091CDC0, length 384 bytes.
// Callees: CWorld::GetFrame @ 0x0049D870, the inlined GetInterface 0x34 (CTCPerceiveThing, bit 20 of +0x24),
//          CTCPerceiveThing::GetPerceivedThing @ 0x007446A0, CThing::GetPos @ 0x004C73D0, CBaseIntelligentPointer::
//          GetPItem @ 0x00A01B50 (the creature's PHome +0x168), CAIStateGroupBase::GetPWorld @ 0x008FCFC0,
//          CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF290, CMessageEventManager::FindMostRecentMessage<
//          CFilter_Bind2<...>> @ 0x008D46D0 / AddMessage @ 0x00687540 (world +0x60), CThingBuilding::IsPointInside
//          @ 0x0082D000, __ftol2 @ 0x00BFEA70.
// Globals: FramesPerSecond @ 0x01375550, 120.0f @ 0x0128FF64.
// Shape: the frame-rate operand is read before the frame call (right operand first); the filter chain is
//          `IsOfType(0x1f) && IsOriginatedByThing(home)` bound by pointers; every exit returns InvestigatingNoise.
// Layout: engine/CAIStateGroup_MinionReactToNoise.h (NoisePosition +0x14, InvestigatingNoise +0x20, FoundNoiseMaker
//          +0x21, FrameOfLastInteraction +0x24); PBrain is the first dword at +0x04.
#include "engine/CAIStateGroup_MinionReactToNoise.h"

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* f = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        if (f[half].type < *type) { f += half + 1; count -= half + 1; } else count = half;
    }
    return f;
}
#pragma auto_inline(on)
class C3DVector { public:
    float X, Y, Z;
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPos() const;             // 0x004C73D0
    bool IsPointInside(const C3DVector& point); // 0x0082D000 (CThingBuilding)
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
class CTCPerceiveThing { public:                       // interface 0x34
    unsigned char _pad[0x32];
    bool CanHear;                               // +0x32 (descriptive)
    CThing* GetPerceivedThing();                // 0x007446A0
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThingAICreature : public CThing { public:
    unsigned char _pad2[0x168 - 0x4c];
    CIntelligentPointer<CThing> PHome;              // +0x168
};
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2<CMessageEventFilter_IsOfType,
                                         CMessageEventFilter_IsOriginatedByThing>& filter);   // 0x008D46D0
    CMessageEvent* AddMessage(long type, unsigned long duration, const C3DVector* pos, CThing* from, CThing* about,
                              float a, float b, long c);   // 0x00687540
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageManager;      // +0x60
public:
    static long __fastcall GetFrame();   // 0x0049D870
};
extern long FramesPerSecond;                    // 0x01375550
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

struct CAIStateGroupMinionReactToNoiseMethods : CAIStateGroup_MinionReactToNoise {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingAICreature* GetCreature() const { return Brain()->PCreature; }
    C3DVector& NoisePosition() { return *(C3DVector*)&NoisePosition_X; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsRunnable();
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CFilter_Bind2<CMessageEventFilter_IsOfType,
                                         CMessageEventFilter_IsOriginatedByThing>& filter)
{
    int hc = 0;
    { const unsigned char* b = (const unsigned char*)&filter; for (unsigned i = 0; i < sizeof(filter); ++i) hc += b[i]; }
    { const unsigned char* b = (const unsigned char*)filter.m_pFirst; for (unsigned i = 0; i < sizeof(*filter.m_pFirst); ++i) hc += b[i]; }
    { const unsigned char* b = (const unsigned char*)filter.m_pSecond; for (unsigned i = 0; i < sizeof(*filter.m_pSecond); ++i) hc += b[i]; }
    return (CMessageEvent*)StandInSink(hc, 10);
}
#pragma auto_inline(on)
bool CAIStateGroupMinionReactToNoiseMethods::IsRunnable()
{
    float u0 = 0.0f;   // E-19 symbol-count re-key (no bytes)
    float u1 = 0.0f;
    long fps = FramesPerSecond;
    if ((float)CWorld::GetFrame() > (float)(fps * 120.0f + FrameOfLastInteraction))
        return false;
    if (!InvestigatingNoise) {
        CTCPerceiveThing* perceive;
        GetCreature()->GetInterface(&perceive, 0x34);
        CThing* noise = perceive->GetPerceivedThing();
        if (noise) {
            const C3DVector& noisePos = noise->GetPos();
            CThing* home = GetCreature()->PHome.GetPItem();
            if (home) {
                CMessageEventManager* messages = GetPWorld()->PMessageManager;
                if (!messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x1f)
                        && CMessageEventFilter_IsOriginatedByThing(GetCreature()->PHome.GetPItem()))
                    && perceive->CanHear && home->IsPointInside(noisePos)) {
                    NoisePosition() = noisePos;
                    CThingAICreature* creature = GetCreature();
                    long fps2 = FramesPerSecond;
                    messages->AddMessage(0x1f, (unsigned long)((float)fps2 * 2.0f), &NoisePosition(),
                        creature, creature->PHome.GetPItem(), 0.0f, 0.0f, 0);
                    InvestigatingNoise = true;
                    FoundNoiseMaker = false;
                }
            }
        }
    }
    return InvestigatingNoise;
}