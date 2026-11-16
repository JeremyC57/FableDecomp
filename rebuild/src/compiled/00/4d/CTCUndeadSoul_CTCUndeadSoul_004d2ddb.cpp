#pragma optimize("s",on)
// CTCUndeadSoul::CTCUndeadSoul(CThing& thing) -- construct the undead-soul thing component: the CTCBase part for the owner thing, then
// this class's vtable
// Leaf: ??0CTCUndeadSoul@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2DDB, 24 bytes: push esi ; push [esp+8] ; mov esi,ecx ; call CTCBase::CTCBase 0x00686800 ;
//   mov [esi],offset CTCUndeadSoul::`vftable' (0x0123A364) ; mov eax,esi ; pop esi ; ret 4.
// Identity: FableWin.pdb ??0CTCUndeadSoul@@QAE@AAVCThing@@@Z; Ego_r.pdb ?Alloc@CTCUndeadSoul@@SIPAVCTCBase@@AAVCThing@@@Z (the registry
//   factory that news one from a CThing&) and the overrides GetInterfaceType / GetClassTypeName / HasFrameUpdate / FrameUpdate
//   (0x00743990). The base call is CTCBase::CTCBase(CThing&) 0x00686800 (??0CTCBase@@QAE@AAVCThing@@@Z).
// (Landed earlier as one of 68 generated bodies of this shape with a made-up CEngineSubPrimitive2DClockSprite base and a pointer
//   argument; re-authored 2026-11-16 on the shared engine/CTCBase.h.)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCUndeadSoul : public CTCBase {
public:
    CTCUndeadSoul(CThing& thing);
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();                                          // 0x00743990
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
};

CTCUndeadSoul::CTCUndeadSoul(CThing& thing)
    : CTCBase(thing)
{
}
