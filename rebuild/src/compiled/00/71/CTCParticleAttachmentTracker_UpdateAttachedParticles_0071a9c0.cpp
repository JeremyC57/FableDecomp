// CTCParticleAttachmentTracker::UpdateAttachedParticles(float time) -- for every attached particle thing that still exists and is not being removed, update its particle-emitter component's attachment
// Leaf: UpdateAttachedParticles
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A9C0, 118 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined
//   CThing::GetInterface, interface 0x4B = CTCDParticleEmitter: bit 11 of the present-bit dword at CThing+0x28),
//   CTCDParticleEmitter::UpdateEmitterThingAttachment(bool, float) @ 0x006DFFB0.
// Layout: as the landed SetAsDrawable (0x0071A8A0): AttachedParticles at +0x0c is an STLport list held as its header-node pointer;
//   CThing +0x91 bit 0 = scheduled for removal, component bitset +0x20, interface map +0x44.
// Shape: `this` spilled into the push-ecx slot because esi is reused for the interface map; end() re-read every iteration. The
//   emitter comes from the out-parameter GetInterface(T**, type) template with the end/hit ternary inside (`*ppOut = (cond ?
//   last : it)->component`): the out-parameter form gives retail's this/time registers, the ternary loads the component straight
//   into ecx (the if-assign form goes through eax, HasTC + GetTC keeps this in ebx).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCDParticleEmitter { public:
    void UpdateEmitterThingAttachment(bool attach, float time);   // 0x006DFFB0
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct SParticleNode {
    SParticleNode*      m_next;
    SParticleNode*      m_prev;
    CIntelligentPointer<CThing> m_value;
};
struct CParticleList {
    SParticleNode* m_node;
    SParticleNode* begin() const { return m_node->m_next; }
    SParticleNode* end() const   { return m_node; }
};
class CTCParticleAttachmentTracker { public:
    void*         __vftable;                    // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    CParticleList AttachedParticles;            // +0x0c
    void UpdateAttachedParticles(float time);
};

void CTCParticleAttachmentTracker::UpdateAttachedParticles(float time)
{
    for (SParticleNode* it = AttachedParticles.begin(); it != AttachedParticles.end(); it = it->m_next) {
        CThing* thing = it->m_value.Get();
        if (thing && !thing->IsDead()) {
            CTCDParticleEmitter* particles;
            if (thing->GetTC(TCI_PARTICLE_EMITTER, &particles))
                particles->UpdateEmitterThingAttachment(true, time);
        }
    }
}