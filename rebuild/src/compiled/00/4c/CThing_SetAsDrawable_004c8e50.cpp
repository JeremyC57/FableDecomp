// CThing::SetAsDrawable(bool drawable) -- record the drawable flag (flagging the graphic appearance as changed when it flips) and pass it on to the thing-owner, carrying, emote-icon, particle-attachment-tracker and particle-emitter components the thing has
// Leaf: SetAsDrawable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C8E50, length 341 bytes (`ret 4`, no frame pointer, no frame: the lookup
//   keys live in the dead argument slot; Ego_r.pdb `?SetAsDrawable@CThing@@UAEX_N@Z`).
// Callees: NGraphicAppearance::CTCGraphicAppearance::SetAsChanged(unsigned long) 0x004BF9E0 (reasons 0xF);
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetTC); the components' SetAsDrawable(bool):
//   CTCThingOwner 0x0071B2D0 (interface 0x21; the landed file calls this address CTCThingOwner::OnSetInLimbo -- see
//   NOTES), CTCCarrying 0x006A1370 (0x46), CTCEmoteIcon 0x0071AC20 (0x45), CTCParticleAttachmentTracker 0x0071A8A0
//   (0xD8), CTCDParticleEmitter 0x006E1020 (0x4B) (Ego_r.pdb `?SetAsDrawable@<class>@@QAEX_N@Z` for all five; manifest).
// Layout: retail CThing (Ego_r.pdb minus 0x10 from UpdateComponentsList on: the 4-byte STLport list and the CVectorMap):
//   interface bitset +0x20, interface map +0x44 (first, last), PTCGraphicAppearance +0x64, the flag byte +0x92 (PDB +0xA2:
//   DeletedOnOwnerLevelOutOfScope bit 0, Drawable bit 1).
// Shape: `if (Drawable != drawable) { Drawable = drawable; if (appearance) appearance->SetAsChanged(15); }` (the bitfield
//   read compared against the argument, the appearance pointer read before the bitfield store); then one
//   `if (GetInterface(&tc, t)) tc->SetAsDrawable(drawable)` per interface -- the out-parameter lookup leaves the component
//   in eax (`mov eax,[eax+4] ; push ebx ; mov ecx,eax`; a pointer-returning GetTC loads straight into ecx, GOTCHAS wave
//   24k); the same-TU LowerBound stand-in lets VC7.1 put every key in the argument's slot (the argument lives in ebx).
class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte entries; the landed
// 0x00848730 idiom): VC7.1 then knows the key's address does not escape.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAsChanged(unsigned long reasons);               // 0x004BF9E0
};
}
class CTCThingOwner { public: void SetAsDrawable(bool drawable); };                  // 0x0071B2D0 (interface 0x21)
class CTCCarrying { public: void SetAsDrawable(bool drawable); };                    // 0x006A1370 (interface 0x46)
class CTCEmoteIcon { public: void SetAsDrawable(bool drawable); };                   // 0x0071AC20 (interface 0x45)
class CTCParticleAttachmentTracker { public: void SetAsDrawable(bool drawable); };   // 0x0071A8A0 (interface 0xD8)
class CTCDParticleEmitter { public: void SetAsDrawable(bool drawable); };            // 0x006E1020 (interface 0x4B)

class CThing {
public:
    virtual void SetAsDrawable(bool drawable);
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char _pad68[0x92 - 0x68];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;        // +0x92 bit 0
    unsigned char Drawable : 1;                             // +0x92 bit 1
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

void CThing::SetAsDrawable(bool drawable)
{
    if (Drawable != drawable) {
        Drawable = drawable;
        if (PTCGraphicAppearance)
            PTCGraphicAppearance->SetAsChanged(15);
    }
    CTCThingOwner* owner;
    if (GetInterface(&owner, 0x21))
        owner->SetAsDrawable(drawable);
    CTCCarrying* carrying;
    if (GetInterface(&carrying, 0x46))
        carrying->SetAsDrawable(drawable);
    CTCEmoteIcon* emoteIcon;
    if (GetInterface(&emoteIcon, 0x45))
        emoteIcon->SetAsDrawable(drawable);
    CTCParticleAttachmentTracker* tracker;
    if (GetInterface(&tracker, 0xd8))
        tracker->SetAsDrawable(drawable);
    CTCDParticleEmitter* emitter;
    if (GetInterface(&emitter, 0x4b))
        emitter->SetAsDrawable(drawable);
}