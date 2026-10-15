#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCTrapBase::SetThingAsImmune(CThing&) -- appends an intelligent pointer to the thing to the trap's ImmuneThings vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078839A, length 41 bytes. Frame-pointer size TU (`push ebp ; mov ebp,esp`,
// `push dword ptr [ebp+8]` pass-through, `leave`): `optimize("y",off)` + `("s",on)`.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0, vector<CIntelligentPointer<CThing> >::
// push_back(const T&) @ 0x004AD520, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the bare temporary `CIntelligentPointer(&thing)` lives in the 8-byte frame (`push ecx ; push ecx`) and the
// constructor's returned `this` (eax) is pushed straight into push_back; the temporary dies after the call.
// Layout: the engine header CTCTrapBase.h is the PDB layout (12+16-byte vectors); the retail constructor 0x0078918D
// shows 12-byte STLport vectors (HitThings +0x0C, ImmuneThings +0x18), so a local model is used.
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
    unsigned char             _pad_0x04[0x8];
    CIntelligentPointerVector HitThings;    // +0x0C (pair<CIntelligentPointer, long> elements)
    CIntelligentPointerVector ImmuneThings; // +0x18
    void SetThingAsImmune(CThing& thing);
};

void CTCTrapBase::SetThingAsImmune(CThing& thing)
{
    ImmuneThings.push_back(CIntelligentPointer(&thing));
}