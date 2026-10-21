#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCTrapBase::SetThingAsImmune(CThing&) -- appends an intelligent pointer to the thing to the ImmuneThings vector at +0x2C
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007898A3, length 41 bytes. Frame-pointer size TU (`push ebp ; mov ebp,esp`,
// `push dword ptr [ebp+8]` pass-through, `leave`): `optimize("y",off)` + `("s",on)`.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0, vector<CIntelligentPointer<CThing> >::
// push_back(const T&) @ 0x004AD520, ~CBaseIntelligentPointer @ 0x00A01C10.
// Label: the catalog/manifest name is the propagated CTCTrapBase one; the only callers (e.g. the trap's activation
// code at 0x0078836B) call it on the thing component fetched from the trap's obstruction thing (GetTC @ 0x007890EA),
// passing each of the trap's own ImmuneThings. So this is SetThingAsImmune of the obstruction's component class,
// whose ImmuneThings vector sits at +0x2C (the trap's own is at +0x18, see 0x0078839A). The model keeps the catalog
// class name with a descriptive layout.
// Shape: the bare temporary `CIntelligentPointer(&thing)` in the 8-byte frame, the constructor's returned `this` (eax)
// pushed straight into push_back; the temporary dies after the call.
struct CThing;

struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* thing);     // 0x004A96F0
};

struct CIntelligentPointerVector {          // retail STLport vector, 12 bytes
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    void push_back(const CIntelligentPointer& value);   // 0x004AD520
};

struct CTCTrapBase {
    void*                     __vftable;    // +0x00
    unsigned char             _pad_0x04[0x28];
    CIntelligentPointerVector ImmuneThings; // +0x2C
    void SetThingAsImmune(CThing& thing);
};

void CTCTrapBase::SetThingAsImmune(CThing& thing)
{
    ImmuneThings.push_back(CIntelligentPointer(&thing));
}