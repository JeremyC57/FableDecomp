// CTCEffectOnDie::FrameUpdate() -- count the effect's life down (unless paused); inside the fade-out window fade the thing and everything it carries with alpha 255 * timer / fadeout; once the timer has run out kill the thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0BF0, 267 bytes (+ int3 padding), plain `ret`.
// Callees: __ftol2 @ 0x00BFEA70 (x2), CTCGraphicAppearance::SetAlpha(unsigned char) @ 0x004BFAC0 (on the thing's
//   appearance TC at +0x64), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of CTCCarrying,
//   interface 0x46 = word 2 bit 6), the carrying TC's carried-things collector (vector<CThing*>&) @ 0x006A2220 (manifest
//   "COptimisedPrimitive::RemapGroups" is propagated; named descriptively), free @ 0x00BFEA14, CThing::Kill(bool) @
//   0x004C9B80. Constant: 255.0f @ 0x01230014.
// Layout = Ego_r.pdb CTCEffectOnDie (0x24): Timer +0x14, FadeOutTimer +0x18, Pausable +0x20, Paused +0x21; CThing
//   appearance TC +0x64.
// Shape: `long timer = Timer - 1; Timer = timer;` stores the frame word before the member (`timer = --Timer` stores
//   the member first); the decremented timer and the fade-out length are named long locals (their frame words feed fild/fidiv) and
//   the alpha is a named float local reusing the fade-out word; each SetAlpha is behind a null test of the appearance
//   TC; the carrying TC is a named local fetched before the local STLport vector<CThing*> is declared (its zero stores
//   follow the lookup), the vector is filled by the collector, walked with cached begin/end pointers (esi/ebp) and freed
//   by its inline destructor; one zero register (ebx) serves every zero compare and store.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    void Kill(bool immediately);                // 0x004C9B80
};
struct CThingVector {                           // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
};
struct CTCCarrying {                            // interface 0x46
    void GetCarriedThings(CThingVector& things);    // 0x006A2220
};
struct CTCEffectOnDie {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    StartedNonRecoverableFadeout;       // +0x0c
    long    Effect;                             // +0x10
    long    Timer;                              // +0x14
    long    FadeOutTimer;                       // +0x18
    void*   DeathSoundCriteria;                 // +0x1c CCharString
    bool    Pausable;                           // +0x20
    bool    Paused;                             // +0x21
    void FrameUpdate();
};

void CTCEffectOnDie::FrameUpdate()
{
    if (Timer > 0) {
        if (!Paused) {
            long timer = Timer - 1;
            Timer = timer;
            long fadeOut = FadeOutTimer;
            if (timer <= fadeOut) {
                Pausable = false;
                float alpha = (float)timer / fadeOut * 255.0f;
                CTCGraphicAppearance* appearance = ParentThing->PGraphicAppearance;
                if (appearance)
                    appearance->SetAlpha((unsigned char)(long)alpha);
                CThing* thing = ParentThing;
                if (thing->HasTC(0x46)) {
                    CTCCarrying* carrying = thing->GetTC<CTCCarrying>(0x46);
                    CThingVector things;
                    carrying->GetCarriedThings(things);
                    for (CThing** it = things._M_start, **end = things._M_finish; it != end; ++it) {
                        CTCGraphicAppearance* carried = (*it)->PGraphicAppearance;
                        if (carried)
                            carried->SetAlpha((unsigned char)(long)alpha);
                    }
                }
            }
        }
    } else
        ParentThing->Kill(true);
}
