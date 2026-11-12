// CTCTavernTankard::SetBarmaid(CThingAICreature*) -- routes the argument through a temporary CIntelligentPointer<CThingAICreature> into PBarmaid (+0x34)
// Leaf: ?SetBarmaid@CTCTavernTankard@@QAEXPAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094B400, oracle length 71 bytes.
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esp+4] ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov eax,[esp+10h] ; push eax ; lea ecx,[esp+8] ; mov dword ptr [esp+8],offset CIntelligentPointer::`vftable' ; call SetPItem ; lea ecx,[esp+4] ; call GetPItem ; push eax ; lea ecx,[esi+0x34] ; call SetPItem ; lea ecx,[esp+4] ; call ~CBaseIntelligentPointer ; pop esi ; add esp,8 ; ret 4
// A stack CIntelligentPointer (8 bytes: vtable 0x0123E7E0 from rtti-vtables.tsv + item; constructed by the
// inlined default ctor over CBaseIntelligentPointer() @ 0x00A01B10) is pointed at the argument
// with SetPItem @ 0x00A01B90, read back with GetPItem @ 0x00A01B50 into the member pointer at
// +0x34, and destroyed @ 0x00A01C10. The class comes from the manifest label when unique.
// Naming (agentNF31, 2026-11-12): was CClass_0094b400::AssignTarget_0094b400(CBaseObject*) over local intelligent-pointer models.
//   Ego_r.pdb 0x0096F7D0 (next to ReplaceTankard) + FableWin.pdb 0x017C46E2 ?SetBarmaid@CTCTavernTankard@@QAEXPAVCThingAICreature@@@Z;
//   Ego_r layout CTCTavernTankard (sizeof 60) PBarmaid = CIntelligentPointer<CThingAICreature> at +0x34; the landed caller
//   ThankBarmaid 0x00949380 binds this symbol. Shared engine/CIntelligentPointer.h (retail branch); the class model is that
//   caller's (no engine header).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCBase.h"
class CTCTavernTankard : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_TAVERN_TANKARD; }
    unsigned char _pad[0x34 - sizeof(CTCBase)];
    CIntelligentPointer<CThingAICreature> PBarmaid;     // +0x34
    void SetBarmaid(CThingAICreature* barmaid);          // 0x0094B400
};

void CTCTavernTankard::SetBarmaid(CThingAICreature* barmaid)
{
    PBarmaid = CIntelligentPointer<CThingAICreature>(barmaid);
}
