// NTCMessages::CMessageOnSetThingOwner::CMessageOnSetThingOwner(CThing* owner) -- base CBaseClass() (through CMessageBase), Owner
// stored at +0x4, then this vtable.
// Leaf: ??0CMessageOnSetThingOwner@NTCMessages@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8310, 27 bytes:
//   push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; mov eax,[esp+8] ; mov [esi+0x4],eax ;
//   mov dword ptr [esi],offset NTCMessages::CMessageOnSetThingOwner::`vftable' ; mov eax,esi ; pop esi ; ret 4
// Name (agentNF32, 2026-11-12; was the global struct CMessageOnSetThingOwner(void*) with member m_argument): FableWin.pdb 0x0258AAF0
//   ??0CMessageOnSetThingOwner@NTCMessages@@QAE@PAVCThing@@@Z (with ??1 / PeekOwner / GetType of the same class); Ego_r
//   struct_layouts CMessageOnSetThingOwner +4 CThing* Owner; rtti_bases NTCMessages::CMessageOnSetThingOwner : NTCMessages::CMessageBase
//   : CBaseClass; vtable 0x01270BF0 (slot 0 0x007E8350, slot 1 GetType 0x007E8330). The landed 0x007E8590 CTCOwnedEntity::SetPOwner
//   inlines this ctor over the same model (copied from that row; no shared header exists for NTCMessages).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CThing;

namespace NTCMessages {
class CMessageBase : public CBaseClass {
public:
    virtual unsigned long GetType() const;                      // slot 1 (FableWin ?GetType@CMessageBase@NTCMessages@@UBEKXZ)
};
class CMessageOnSetThingOwner : public CMessageBase {           // vtable 0x01270BF0: slot 0 dtor 0x007E8350, slot 1 GetType 0x007E8330
public:
    CMessageOnSetThingOwner(CThing* owner);
    virtual unsigned long GetType() const { return 4; }        // slot 1 0x007E8330 `mov eax,4 ; ret` (rtti-vtables 01270bf0)
    CThing* PeekOwner() const { return Owner; }
private:
    CThing* Owner;                                              // +0x04
};
}

NTCMessages::CMessageOnSetThingOwner::CMessageOnSetThingOwner(CThing* owner)
    : Owner(owner)
{
}
