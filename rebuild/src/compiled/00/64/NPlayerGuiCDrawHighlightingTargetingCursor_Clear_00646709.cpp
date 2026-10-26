#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawHighlightingTargetingCursor::Clear() -- drop the highlight mesh effect from the targeted thing's graphic appearance (when the thing is alive and has one), release the intelligent pointer and reset the cursor type
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646709, length 89 bytes.
// Callees: IS_THING_ALIVE<CIntelligentPointer<const CThingPhysical>> @ 0x004CC340 (`__fastcall`, the pointer in ecx), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (twice), CTCGraphicAppearance::ClearMeshEffect(void*) @ 0x004BFFC0 (on the thing's component at +0x64, key = this), CIntelligentPointer<const CThing>::
// CIntelligentPointer(CThing*) @ 0x004A96F0 (a null temporary in the 8-byte frame, built by the implicit conversion of
// the null pointer, so it is re-addressed with `lea` for the assignment), CIntelligentPointer::operator=(const CIntelligentPointer&) @ 0x004A9760,
// ~CBaseIntelligentPointer @ 0x00A01C10. Frame-pointer size TU (`push ecx ; push ecx` opens the temporary, `and dword ptr [esi+44h],0`, `leave`). PDB
// CDrawHighlightingTargetingCursor: PTargetedThing +0x2c, CursorType +0x48 -> retail +0x28 / +0x44 (the CDrawBase vector is 4 bytes smaller).
class CThing;
class CThingPhysical;

struct CTCGraphicAppearance {
    void ClearMeshEffect(void* key);               // 0x004BFFC0
};

struct CThingWithAppearance {
    unsigned char _pad[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;    // +0x64
    CTCGraphicAppearance* GetGraphicAppearance() const { return m_pGraphicAppearance; }   // inline: the pointer lands in ecx directly
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    CThingWithAppearance* GetPItem() const;        // 0x00A01B50
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
    CIntelligentPointer_CThing(CThing* thing);     // 0x004A96F0
    CIntelligentPointer_CThing& operator=(const CIntelligentPointer_CThing& other);   // 0x004A9760
};

bool __fastcall IS_THING_ALIVE(CIntelligentPointer_CThing* pointer);   // 0x004CC340

namespace NPlayerGui {

class CDrawHighlightingTargetingCursor { public:
    unsigned char              _pad[0x28];
    CIntelligentPointer_CThing PTargetedThing;     // +0x28
    unsigned char              _pad30[0x14];       // +0x30 CurrentColour, widths, fade step, width
    long                       CursorType;         // +0x44
    void Clear();
};

void CDrawHighlightingTargetingCursor::Clear()
{
    if (IS_THING_ALIVE(&PTargetedThing)) {
        if (PTargetedThing.GetPItem()->m_pGraphicAppearance != 0)
            PTargetedThing.GetPItem()->GetGraphicAppearance()->ClearMeshEffect(this);
    }
    PTargetedThing = (CThing*)0;                   // implicit conversion temporary: re-addressed with lea for the operator= call
    CursorType = 0;
}

}