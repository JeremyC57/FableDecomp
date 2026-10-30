// CAIStateGroup_MinionWorkInInn::IsRunnable() -- runnable when the villager's home exists and its material
// Grade: DIFFER(100v100) -- scratch-register choice only: retail chains the creature re-read through ecx (`mov ecx,[esi+4] ; mov ecx,[ecx+18h]`) and passes the hidden result slot in edx, ours uses edx/ecx the other way; the final vtable load is eax in retail, edx in ours (same instructions otherwise).
// definition carries the "inn" flag (byte +0x3c bit 5).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090BBF0, length 100 bytes (+ int3 padding). Vtable slot 11.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on the creature's PHome at +0x170, CThingBuilding::
// PeekPMaterial @ 0x0082D2D0 (returns a CDefPointer<const CMaterialDef> through a hidden pointer, `ret 4`), the
// def's virtual slot 1 (destroy at reference count zero).
// Shape: three early `return false` (shared `xor al,al` tail): no home, no villager interface on the creature
// (`test byte ptr [ecx+24h],8`, the presence bit alone -- no lookup), null def; then the bit is read into bl and
// the temporary's destructor releases the def through the ref-count base's two inline accessors (`mov esi,
// [eax+4] ; add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi ; jne`, the landed 0x0083E0D0
// shape; the null path skips the release) before `mov al,bl`. Layout local (the engine header
// flattens the base; the material def's flag name is descriptive).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

// The def handle and the def chain: the retail shared headers (FABLE_RETAIL_DEF_CLASS_BASE; CDefPointer<T>::Release is the
// `mov esi,[eax+4] ; ... dec esi ; ... mov [eax],esi ; jne ; call [vtbl+4]` release retail expands).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"

class CSimBuildingDef : public CDefClassBase {      // the building's sim definition (retail CDefClassBase is 0x25, packed)
public:
    unsigned char _pad25[0x3c - 0x25];
    unsigned long _bits3c_0 : 5;
    unsigned long IsInn : 1;                    // +0x3c bit 5 of a DWORD flag word (descriptive; `shr ebx,5`)
};

class CThingBuilding {
public:
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;   // 0x0082D2D0 (Ego_r.pdb QBE; hidden result pointer, `ret 4`)
};

struct CThingCreatureHome {
    unsigned char _pad[0x170];
    CBaseIntelligentPointer PHome;              // +0x170 CIntelligentPointer<CThingBuilding>
};

struct CAIStateGroup_MinionWorkInInn : CAIStateGroupBase {
    virtual bool IsRunnable();
};

// the creature's home: a named creature parameter keeps the creature in eax and forms the pointer address with `lea`
__forceinline CThing* HomeOf(CThingCreatureHome* creature)
{
    return creature->PHome.GetPItem();
}

bool CAIStateGroup_MinionWorkInInn::IsRunnable()
{
    CThingBuilding* home = (CThingBuilding*)HomeOf((CThingCreatureHome*)PBrain->PCreature);
    if (home == 0)
        return false;
    CThing* creature = PBrain->PCreature;
    if (!creature->HasTC(0x23))
        return false;
    CDefPointer<const CSimBuildingDef> def = home->PeekPSimDefinition();
    if (def.Object)
        return def->IsInn;
    return false;
}
