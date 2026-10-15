// CCreatureAction_ShowTrophy::CCreatureAction_ShowTrophy(CThingCreatureBase*, CThing*, const CCharString&, long) -- base CCreatureActionBase(owner, -1.0, 0.95, the owner's animation for the name, zeroed CSoundPair, 100, last), own vtable, the trophy pointer, two cleared fields and a second, empty intelligent pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DF730, length 143 bytes (+ int3 padding).
// Callees: GetTableAnimation(CThingCreatureBase*, const CCharString&) @ 0x006924B0 (register arguments: ecx owner, edx name; it forwards to
// 0x00662FA0(name, 1); the manifest "CMainGameComponent::BeginInputLoading" label is propagated), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*,
// float, float, CAnimationEntry*, const CSoundPair&, long, long) @ 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (twice),
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90. Own vtable 0x0125F7B4 (rtti-vtables.tsv); the members' CIntelligentPointer vtable 0x01238C6C.
// Shape: `ret 10h`; the base arguments are evaluated right to left (the last parameter, 100, the sound-pair temporary zeroed from ebx, the
// lookup, 0.95f, -1.0f, the owner); ebx = 0 is shared by the sound pair and the two cleared fields. The second intelligent pointer sits at the
// UNALIGNED offset +0xb5 (after the byte at +0xb4): the class is packed to 1 (Ego_r.pdb packs it the same way).
// The PDB header (CCreatureAction_ShowTrophy, 0x0c bytes high) flattens the base, so `this` is modelled locally.
#include "fable_string.h"
struct CThing;
struct CThingCreatureBase;
struct CAnimationEntry;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

#pragma pack(push, 1)
struct CCreatureAction_ShowTrophy : CCreatureActionBase {      // vtable 0x0125F7B4; retail sizeof 0xbd
    CIntelligentPointer PTrophy;            // +0xa8 CIntelligentPointer<CThing>
    long TrophyState;                       // +0xb0
    unsigned char Shown;                    // +0xb4
    CIntelligentPointer PTrophyObject;      // +0xb5 CIntelligentPointer<CThing> (unaligned)
    virtual void v00();
    CCreatureAction_ShowTrophy(CThingCreatureBase* owner, CThing* trophy, const CCharString& animation, long last);
};
#pragma pack(pop)

CCreatureAction_ShowTrophy::CCreatureAction_ShowTrophy(CThingCreatureBase* owner, CThing* trophy, const CCharString& animation, long last)
    : CCreatureActionBase(owner, -1.0f, 0.95f, GetTableAnimation(owner, animation), CSoundPair(), 100, last)
    , PTrophy(trophy)
    , TrophyState(0)
    , Shown(0)
    , PTrophyObject()
{
}