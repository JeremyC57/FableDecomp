// Grade: DIFFER(159v159) -- slot assignment only: retail homes the variation copy at [esp+8] and `1.0f + variation` at [esp+0Ch] (and loads the variation before `push ecx`); ours swaps the two frame slots. Instruction order otherwise identical.
// CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired() -- schedule the next frame the scripted group may fire: now plus the scratchpad's ActionFrequency seconds scaled by a random factor in [1 - variation, 1 + variation]
// Leaf: Fired
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009350C0, length 159 bytes (+ int3 padding); called by
// CAIStateGroup_PerformAction::Update (landed 0x009352C0) at 0x009353E9 / 0x009354E5.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xAA = CTCAIScratchpad, bit 10 of the bitset dword at +0x34),
// CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0 (landed), GFFloatRandom(float range, unsigned long* seed) @ 0x004978A0
// (__fastcall: the seed in ecx, the float on the stack), __ftol2 @ 0x00BFEA70, GetWorldFrame @ 0x0049D870 (manifest
// "GFGetPreMainMemoryUsed" is propagated; landed naming of 0x008CF370). Globals: g_framesPerSecond @ 0x01375550, 1.0f @ 0x0122DED8.
// Layout: CAIStateGroupBase PBrain +4 (engine/CAIStateGroupBase.h), CAIBrain Creature +0x18 (engine/CAIBrain.h), Ego_r.pdb
// CAIStateGroup_ScriptingBase NextFrameFireable +0x14, CTCAIScratchpad ActionFrequency +0x40 / ActionFrequencyVariation +0x44.
// Modelled locally (the generated headers carry the vptr as a data member, so a polymorphic derived model cannot reuse them).
// Shape: the UNCHECKED out-parameter lookup (the absent arm reads the never-written out local); the variation is copied into a
// float local (integer move), `1.0f + variation` is spilled before the random call, the seed argument is evaluated first
// (right to left), then `fsubr ; fimul` by the frames-per-action long and `__ftol2`; the world frame is fetched last.
struct CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

struct CTCAIScratchpad {                        // interface type 0xaa component
    unsigned char _pad00[0x40];
    long          ActionFrequency;              // +0x40 seconds
    float         ActionFrequencyVariation;     // +0x44
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThing*       Creature;                     // +0x18 CThingAICreature*
};

extern long g_framesPerSecond;                  // 0x01375550
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
long GetWorldFrame();                           // 0x0049D870

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain*     PBrain;                       // +0x04
    unsigned char _pad08[0x14 - 8];
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};

template<class TBase> struct CAIStateGroup_ScriptingBase : TBase {
    long NextFrameFireable;                     // +0x14
    void Fired();
    __forceinline CTCAIScratchpad* GetScratchpad()
    {
        CTCAIScratchpad* scratchpad;
        this->PBrain->Creature->GetInterface(&scratchpad, 0xaa);
        return scratchpad;
    }
};

template<> void CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired()
{
    CTCAIScratchpad* scratchpad = GetScratchpad();
    long frames = scratchpad->ActionFrequency * g_framesPerSecond;
    float variation = scratchpad->ActionFrequencyVariation;
    float scale = 1.0f + scratchpad->ActionFrequencyVariation;
    long delay = (long)((scale - GFFloatRandom(variation * 2.0f, GetPWorldSeed())) * frames);
    NextFrameFireable = GetWorldFrame() + delay;
}
