// CThingSwitch::OnThingWithScriptNameApplied(const CCharString& name, const C3DVector& position, CThingCreatureBase& creature) -- a switch of trigger type 6 that is not yet triggered fires when the applied thing's script name is its TriggerableObjectScriptName and the thing is within the trigger radius (2D): queue a CEventTriggered for the creature (component 0x42) and latch Triggered
// Leaf: ?OnThingWithScriptNameApplied@CThingSwitch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083CC10, length 340 bytes (`ret 0Ch`); CThingSwitch vtable 0x012453FC slot 71.
//   Name and signature from Ego_r.pdb (`?OnThingWithScriptNameApplied@CThingSwitch@@UAEXABVCCharString@@ABVC3DVector@@AAVCThingCreatureBase@@@Z`);
//   the catalog calls it "CThingSwitch::vfunc_71".
// Callees: CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDef<CThingSwitchDef>(long, CDefPointer&)
//   @ 0x0083CFA0 (manifest "GetDef<CThingPhysicalSwitchDef>", propagated), the def's virtual slot 1 on the last release,
//   Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString equality), CThing::GetPosition()
//   const @ 0x006A4D00, the switch's virtual GetTriggerRadius (slot 62, +0xf8), LowerBound @ 0x0040F020, operator new
//   @ 0x00BFEA1A, NEntityEvents::CEventTriggered(CThing*) @ 0x0083CF20, CTCEntityEvents::AddEvent @ 0x008113C0.
// Layout (retail): interface bitset +0x20, interface map +0x44, def global index +0x8c, Triggered +0xdc,
//   TriggerableObjectScriptName +0xe0 (Ego_r.pdb +0xf4/+0xf8, CThingPhysical 0x18 lower); CThingSwitchDef::TriggerType
//   +0x70 (PDB +0x74). CCharString data block {chars, length}.
// Shape: PeekDefinition() returns the def pointer by value (PDB) and is inlined: the out-slot local is copied into the
//   result (+1) and destroyed (-1), the trigger type is read before the result is released (ebx pushed only around it).
//   GetDef has a same-TU noinline stand-in (the def address does not escape, so VC forwards the pointer and merges the
//   null tests); the release goes through the def's inline Release() (load / dec / store order). The distance is a named
//   float and the test is written `distance < GetTriggerRadius()` (computed and spilled before the virtual call); the
//   squared terms are written dx-first (VC7.1 evaluates the right operand of a float `+` first).
struct CTCBase;
struct C3DVector { float X, Y, Z; };

struct CCharStringData {
    const char*   m_pData;                              // +0x00
    unsigned long m_length;                             // +0x04
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

struct CEventBase { virtual void v00(); };
struct CThing;
namespace NEntityEvents {
struct CEventTriggered : CEventBase {
    unsigned char _body[0x10 - 4];
    CEventTriggered(CThing* triggerer);                 // 0x0083CF20
};
}
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                   // 0x008113C0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CDefPointeeBase {
    virtual void v000();
    virtual void DestroyDef() const;                    // slot 1
    void Release() const
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
    mutable long m_refCount;                            // +4
};

struct CThingSwitchDef : CDefPointeeBase {
    unsigned char _pad08[0x70 - 8];
    long          TriggerType;                          // +0x70 ESwitchTriggerType
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<const CThingSwitchDef>& out);   // 0x0083CFA0
};
void* FindSwitchDef(long index);                        // stand-in callee of the stand-in body below
// Same-TU stand-in for GetDef<CThingSwitchDef> (0x0083CFA0 sits in this TU, 0x390 bytes after this function): it only
// writes the out slot, so VC7.1 knows the def pointer's address does not escape (the 0x0083DDC0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CThingSwitchDef>& out)
{
    void* def = FindSwitchDef(index);
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    // vtable slots 0..61 (GetTriggerRadius is slot 62)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    unsigned char  _pad04[0x20 - 4];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short m_defGlobalIndex;                    // +0x8c
    unsigned short _pad8e;
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CThingCreatureBase : CThing {
};

struct CThingPhysical : CThing {
    const C3DVector& GetPosition() const;               // 0x006A4D00
};

#include <math.h>
#pragma intrinsic(sqrt, fabs)

struct CThingSwitch : CThingPhysical {
    unsigned char _pad90[0xdc - 0x90];
    bool          Triggered;                            // +0xdc
    unsigned char _paddd[3];
    CCharString   TriggerableObjectScriptName;          // +0xe0
    virtual float GetTriggerRadius() const;             // slot 62 (+0xf8)
    __forceinline CDefPointer<const CThingSwitchDef> PeekDefinition() const
    {
        CDefPointer<const CThingSwitchDef> def;
        GetDefinitionManager()->GetDef(m_defGlobalIndex, def);
        return def;
    }
    virtual void OnThingWithScriptNameApplied(const CCharString& name, const C3DVector& position, CThingCreatureBase& creature);
};

void CThingSwitch::OnThingWithScriptNameApplied(const CCharString& name, const C3DVector& position, CThingCreatureBase& creature)
{
    long triggerType = PeekDefinition()->TriggerType;
    if (triggerType == 6 && !Triggered && TriggerableObjectScriptName == name) {
        const C3DVector& here = GetPosition();
        float dx = here.X - position.X;
        float dy = here.Y - position.Y;
        float distance = (float)fabs(sqrt(dx * dx + dy * dy));
        if (distance < GetTriggerRadius()) {
            if (HasTC(0x42)) {
                CTCEntityEvents* events = (CTCEntityEvents*)GetTC(0x42);
                events->AddEvent(new NEntityEvents::CEventTriggered(&creature));
            }
            Triggered = true;
        }
    }
}