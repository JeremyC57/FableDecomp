// _unattributed::ApplyByPassColorFilter() -- on a live thing, pass the stored flag to its graphic appearance's SetByPassColorFilter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00883AA0, length 55 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on `this` itself), CTCGraphicAppearance::
// SetByPassColorFilter(bool) @ 0x004BFB00.
// The manifest label "CCreatureAction_AddRealObjectToInventory::PerformAction" is rejected: `this` IS an intelligent
// pointer (GetPItem is called with ecx = this, reading +4), followed by one flag byte at +8 -- a 9-byte request object
// {CIntelligentPointer<CThing> PThing; bool ByPass}, not a creature action (whose pointer sits at +8 behind the vptr).
// Shape: the landed IS_THING_ALIVE idiom (pointer set, dead bit +0x91 bit 0 clear, each test re-fetching GetPItem), then
// the appearance component at +0x64 read through a named thing local, null-tested and CSE'd into ecx (the call's `this`;
// a named appearance local lands in eax and costs a `mov ecx,eax`), the bool member pushed zero-extended
// (`xor eax,eax ; mov al`; an unsigned char member would add a `setne` conversion).
struct CTCGraphicAppearance {
    void SetByPassColorFilter(bool byPass); // 0x004BFB00
};

class CThing { public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    unsigned char _pad68[0x29];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CByPassColorFilterRequest {
    CIntelligentPointer<CThing> PThing;         // +0x00 CIntelligentPointer<CThing>
    bool ByPass;                            // +0x08 (a bool member: pushed zero-extended with no setne)
    void ApplyByPassColorFilter();
};

void CByPassColorFilterRequest::ApplyByPassColorFilter()
{
    if (PThing.GetPItem() != 0) {
        if (!(PThing.GetPItem()->m_flags91 & 1)) {
            CThing* thing = PThing.GetPItem();
            if (thing->m_pGraphicAppearance)
                thing->m_pGraphicAppearance->SetByPassColorFilter(ByPass);
        }
    }
}