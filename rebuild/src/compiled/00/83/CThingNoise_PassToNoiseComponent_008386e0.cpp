// CThingNoise::PassToNoiseComponent(CThing*) -- hand a thing to this thing's own noise component (interface 0x37)
// Leaf: PassToNoiseComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008386E0, length 86 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup,
// type 0x37 = bit 23 of the bitset dword at +0x24, tested with `test dword ptr [ecx+24h],800000h`), then
// the component's method @ 0x00749880 (it sits among CTCNoise's methods; the manifest's
// "CVillageTaskCarryCrate::AssignCreatureToTask" is a propagated name) taking the thing.
// Identity: the manifest label "CTCActionUseAcceptBoast::OnActionUse" is propagated over the byte-identical
// GetInterface shape. Here `this` IS the thing (the bitset is read straight off ecx); the body sits among
// CThingNoise's methods (0x00838550..0x008385B0) and is called on a live thing with another thing
// (CTCStealth::UpdateHearability 0x00747AF2, CTCShotDefinitionBase::OnCollisionProper 0x007C18C6), so it
// is modelled as a CThingNoise member; the method name is descriptive.
//
// Shape: the inlined GetInterface template (landed 0x00949320) with an UNINITIALISED component local: hit
// path -> the lookup result in eax; miss path -> the never-written local, homed in the argument's own
// stack slot (`mov eax,[esp+8]` beside the argument load). Frame: one `push ecx` (the key temporary).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

struct CTCNoise {                               // interface type 0x37 component
    void TakeThing(CThing* thing);              // 0x00749880
};

struct CThingNoise : CThing {
    void PassToNoiseComponent(CThing* thing);
};

void CThingNoise::PassToNoiseComponent(CThing* thing)
{
    CTCNoise* noise;
    GetInterface(&noise, 0x37);
    noise->TakeThing(thing);
}