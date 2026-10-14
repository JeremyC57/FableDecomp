// CBase::GetObservedThingCurrentMeshHeight() -- the observed thing's mesh height, or 1.0 when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084D8B0, oracle length 81 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3, through the manager's inlined accessor),
//          CThing::GetCurrentMeshHeight @ 0x00661AA0.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+40h] ; lea ecx,[eax+100h] ; call GetPItem ; test eax,eax ; je D ;
//                test byte ptr [eax+91h],1 ; jne D ; mov eax,[esi+40h] ; lea ecx,[eax+100h] ; call GetPItem ;
//                test byte ptr [eax+6Ch],8 ; je D ; mov esi,[esi+40h] ; lea ecx,[esi+100h] ; call GetPItem ;
//                mov ecx,eax ; call GetCurrentMeshHeight ; pop esi ; ret ; D: fld dword ptr [__real@1.0] ; pop esi ; ret
// The camera manager at +0x40 holds the observed thing as an intelligent pointer at +0x100 and hands
// it out through an inline accessor (the manager pointer is materialised in a register and the
// pointer sub-object formed with `lea`; a direct member call forms `add ecx,100h` instead).  The thing
// is fetched once for the liveness test (null or dead bit 0 of +0x91), again for the "has a mesh"
// bit (bit 3 of +0x6c) and a third time for the call, whose float result is a named local so the
// call is not turned into a tail jump.  The fallback is the float literal 1.0.
struct CThing {
    unsigned char _pad[0x6c];
    unsigned char m_meshFlags;        // +0x6c bit 3: has a current mesh
    unsigned char _pad2[0x91 - 0x6d];
    unsigned char m_lifeFlags;        // +0x91 bit 0: dead
    float GetCurrentMeshHeight();     // 0x00661AA0
};

struct CBaseIntelligentPointer {
    CThing* GetPItem();               // 0x00A01B50
};

struct CCameraManager_0084d8b0 {
    unsigned char _pad[0x100];
    CBaseIntelligentPointer m_observedThing;   // +0x100
    CThing* GetObservedThing() { return m_observedThing.GetPItem(); }
};

struct CBase {
    unsigned char _pad[0x40];
    CCameraManager_0084d8b0* m_pManager;       // +0x40
    float GetObservedThingCurrentMeshHeight(); // 0x0084D8B0
};

float CBase::GetObservedThingCurrentMeshHeight()
{
    CThing* thing = m_pManager->GetObservedThing();
    if (thing != 0 && (thing->m_lifeFlags & 1) == 0) {
        if (m_pManager->GetObservedThing()->m_meshFlags & 8) {
            float height = m_pManager->GetObservedThing()->GetCurrentMeshHeight();
            return height;
        }
    }
    return 1.0f;
}