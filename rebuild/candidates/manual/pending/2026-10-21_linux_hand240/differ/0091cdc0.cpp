// CAIStateGroup_MinionReactToNoise::IsRunnable() -- within two minutes of the last interaction, when not already investigating: a perceived noise maker inside the creature's home with no earlier noise message from home starts an investigation (records the noise position and posts a 2-second noise message)
// Grade: DIFFER(ours v retail) -- the time test: retail spills FramesPerSecond before the frame call and uses fiadd on the member; ours loads the member with fild/faddp; frame 0x1c vs 0x18 (filter temporaries)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091CDC0, length 384 bytes.
// Callees: GFGetCurrentFrame @ 0x0049D870, the inlined GetInterface 0x34 (CTCPerceiveThing, bit 20 of +0x24),
//          CTCPerceiveThing::GetPerceivedThing @ 0x007446A0, CThing::GetPosition @ 0x004C73D0, CBaseIntelligentPointer::
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

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct C3DVector {
    float X, Y, Z;
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
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
struct CTCPerceiveThing {                       // interface 0x34
    unsigned char _pad[0x32];
    bool CanHear;                               // +0x32 (descriptive)
    CThing* GetPerceivedThing();                // 0x007446A0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CThingAICreature : CThing {
    unsigned char _pad2[0x168 - 0x4c];
    CBaseIntelligentPointer PHome;              // +0x168
};
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2<CMessageEventFilter_IsOfType,
                                         CMessageEventFilter_IsOriginatedByThing>& filter);   // 0x008D46D0
    CMessageEvent* AddMessage(long type, unsigned long duration, const C3DVector* pos, CThing* from, CThing* about,
                              float a, float b, long c);   // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageManager;      // +0x60
};
long __fastcall GFGetCurrentFrame();            // 0x0049D870
extern long FramesPerSecond;                    // 0x01375550
struct CAIBrain {
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

bool CAIStateGroupMinionReactToNoiseMethods::IsRunnable()
{
    long fps = FramesPerSecond;
    if ((float)GFGetCurrentFrame() > (float)(fps * 120.0f + FrameOfLastInteraction))
        return false;
    if (!InvestigatingNoise) {
        CTCPerceiveThing* perceive;
        GetCreature()->GetInterface(&perceive, 0x34);
        CThing* noise = perceive->GetPerceivedThing();
        if (noise) {
            const C3DVector& noisePos = noise->GetPosition();
            CThing* home = GetCreature()->PHome.GetPItem();
            if (home) {
                CMessageEventManager* messages = GetPWorld()->PMessageManager;
                if (!messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x1f)
                        && CMessageEventFilter_IsOriginatedByThing(GetCreature()->PHome.GetPItem()))
                    && perceive->CanHear && home->IsPointInside(noisePos)) {
                    NoisePosition() = noisePos;
                    CThingAICreature* creature = GetCreature();
                    messages->AddMessage(0x1f, (unsigned long)((float)FramesPerSecond * 2.0f), &NoisePosition(),
                        creature, creature->PHome.GetPItem(), 0.0f, 0.0f, 0);
                    InvestigatingNoise = true;
                    FoundNoiseMaker = false;
                }
            }
        }
    }
    return InvestigatingNoise;
}
