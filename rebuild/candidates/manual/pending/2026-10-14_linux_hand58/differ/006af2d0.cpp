// CMessageEventFilter_IsOriginatorAlive::operator()(CMessageEvent* event) -- true when the event's originator (the intelligent pointer at +0x48, GetPItem @ 0x00A01B50, through the inline GetPOriginator<CThing>) exists and is not dead (bit 0 of +0x91)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AF2D0, length 63 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls). The inlined GetPOriginator<CThing> spills the item to the dead argument slot and tests a constant-false type check (`xor al,al ; jne`) that the lane folds (the parked 0x006AF4E0 shape). Shape: the pointer member in esi, `mov al,1` / `xor al,al` tails, `ret 4`.
// Grade: DIFFER(ours v retail) -- the inlined GetPOriginator<CThing> type check: retail spills the item to the dead argument slot and tests a constant-false bool (`mov [esp+8],eax ; xor al,al ; jne`), the lane folds it for `return false`, `thing != thing` and `(unsigned long)thing < 0` through a `CThing* const&` (the parked 0x006AF4E0 shape).
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem();                            // 0x00A01B50
};

template<class T> struct STypeCheck_006af2d0 {
    static __forceinline bool Mismatch(CThing* const& thing) { return false; }   // the generic template: CThing never mismatches
};

struct CMessageEvent {                             // PDB layout: POriginator at +0x48
    unsigned char           _pad00[0x48];
    CBaseIntelligentPointer POriginator;           // +0x48
    template<class T> T* GetPOriginator()
    {
        T* result = 0;
        if (POriginator.GetPItem()) {
            CThing* thing = POriginator.GetPItem();
            if (STypeCheck_006af2d0<T>::Mismatch(thing))
                return result;
            return (T*)POriginator.GetPItem();
        }
        return result;
    }
};

struct CMessageEventFilter_IsOriginatorAlive {
    bool operator()(CMessageEvent* event);
};

bool CMessageEventFilter_IsOriginatorAlive::operator()(CMessageEvent* event)
{
    CThing* originator = event->GetPOriginator<CThing>();
    if (originator && !originator->m_dead)
        return true;
    return false;
}
