// CTCWife::OnHeroViolence() -- the hero's violence in front of his wife: before the marriage frame it only blocks courting, clears her love value and hides the attraction icon; once married it applies the def's hate spike (peak, run-out frames, persisting effect) to the parent's opinion-of-hero component as attitude transients 4 and 3
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CFDA0, 209 bytes (+15 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::SetAttractionIcon(bool, float, unsigned char) 0x006CFC90;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; __ftol2 0x00BFEA70;
//          CTCCreatureOpinionOfHero::ApplyOpinionTransientOffset(long, float, long, long, float, bool) 0x00682140 (manifest
//          label, module unattributed; called on interface 0x4f). Globals: g_framesPerSecond 0x01375550.
// Shape: the unmarried arm stores CourtingBlocked (+0x15) and LoveAttitudeValue (+0x8c, PDB +0x90: the retail map at +0x78
//        is 12 bytes) between the pushes and the SetAttractionIcon(false, 1.0f, 0xff) call. The married arm uses the
//        UNCHECKED out-parameter GetInterface for 0x4f (bitset word 2 bit 15: `test ch,ch ; jns`): retail ignores the
//        result, so the absent path reads the key word as the component (`mov ebx,[esp+0Ch]`). The frame count is
//        `(long)(SecondsRunOut * (float)g_framesPerSecond)` (fild first, fmul the def float) and the def pointer is re-read
//        from the member for each call (it is a pointer member; the callee may replace it).
// Layout: CTCWife ParentThing +0x04, PHeroMarriageDef +0x0c, CourtingBlocked +0x15, FrameGotMarriedToThePlayer +0x20,
//         LoveAttitudeValue +0x8c; CHeroMarriageDef SecondsRunOutWifeOnViolenceHateSpike +0x90, PeakEffect.. +0x94,
//         PersistEffect.. +0x98 (PDB 144/148/152); retail CThing bitset +0x20, interface map +0x44.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (the other TU copy is 0x004365B0)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
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
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
class CHeroMarriageDef { public:
    unsigned char _pad00[0x90];
    float SecondsRunOutWifeOnViolenceHateSpike; // +0x90
    float PeakEffectWifeOnViolenceHateSpike;    // +0x94
    float PersistEffectWifeOnViolenceHateSpike; // +0x98
};
class CTCCreatureOpinionOfHero { public:
    void ApplyOpinionTransientOffset(long attitude, float peak, long unused, long runOutFrames, float persist, bool ignorePersonality);   // 0x00682140 (6th parameter bool: retail tests it as a byte)
};
class CTCWife { public:
    virtual void v00();
    CThing*          ParentThing;                // +0x04
    unsigned char    _pad_0x08[4];
    CHeroMarriageDef* PHeroMarriageDef;          // +0x0c CDefPointer<CHeroMarriageDef const>::Object
    unsigned char    _pad_0x10[5];
    bool             CourtingBlocked;            // +0x15
    unsigned char    _pad_0x16[0x20 - 0x16];
    long             FrameGotMarriedToThePlayer; // +0x20
    unsigned char    _pad_0x24[0x8c - 0x24];
    float            LoveAttitudeValue;          // +0x8c
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);   // 0x006CFC90
    void OnHeroViolence();
};
void CTCWife::OnHeroViolence()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetWorldFrame()) {
        CourtingBlocked = true;
        LoveAttitudeValue = 0.0f;
        SetAttractionIcon(false, 1.0f, 0xff);
        return;
    }
    CTCCreatureOpinionOfHero* opinion;
    ParentThing->GetInterface(&opinion, 0x4f);
    opinion->ApplyOpinionTransientOffset(4, PHeroMarriageDef->PeakEffectWifeOnViolenceHateSpike, 0,
        (long)(PHeroMarriageDef->SecondsRunOutWifeOnViolenceHateSpike * (float)g_framesPerSecond),
        PHeroMarriageDef->PersistEffectWifeOnViolenceHateSpike, 1);
    opinion->ApplyOpinionTransientOffset(3, PHeroMarriageDef->PeakEffectWifeOnViolenceHateSpike, 0,
        (long)(PHeroMarriageDef->SecondsRunOutWifeOnViolenceHateSpike * (float)g_framesPerSecond),
        PHeroMarriageDef->PersistEffectWifeOnViolenceHateSpike, 1);
}