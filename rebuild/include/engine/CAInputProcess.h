#ifndef FABLE_ENGINE_CAINPUTPROCESS_H
#define FABLE_ENGINE_CAINPUTPROCESS_H
#ifdef FABLE_RETAIL_INPUTPROCESS
// Retail opt-in (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): RTTI CAInputProcess : CBaseClassNonCopyable : CBaseClass
// (ghidra_out/rtti_bases.tsv); the root of every input process. Ego_r.pdb ??0CAInputProcess@@QAE@XZ (retail 0x00A0D290, called by
// the landed CInputProcessGameBase constructor 0x00687A30) and ??1CAInputProcess@@UAE@XZ (virtual, slot 0 through the scalar deleting
// destructor: InitInterfaces deletes the old process with `push 1 ; call [vtable]`). InputProcessListPos is a CLinkedListPosition
// (Data, Next, Previous), flattened as in the generated layout below. sizeof 0x14.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
class CBaseClass;
class CInputProcessManager;
class CLinkedListPosition;
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
// Opt-in (NF477, r467d / r459a): the retail virtual list of the input-process family, declaration only. Defined by a TU that emits
// the family's vftables (0x00488D10 CPlayer::InitInterfaces), so each table comes out at retail's counted function slots. This
// class's own list is retail vftable 0x0129CA10 (9 slots, RTTI .?AVCAInputProcess@@): ~ / AddToInterface / Init / Cleanup /
// ProcessInput (_purecall) / PreparePrimitivesForEngine / Update / ProcessWhilePaused / IsInputEventToBeSentToThisInputProcess.
// Spellings: FableWin.pdb ?Cleanup@CAInputProcess@@UAEXXZ, ?PreparePrimitivesForEngine@CAInputProcess@@UBEXABVCEnginePrimitivePrepareInfo@@@Z,
// ?Update@CAInputProcess@@UAEXAAVCProcessedInput@@@Z, ?ProcessWhilePaused@CAInputProcess@@UBE_NXZ,
// ?IsInputEventToBeSentToThisInputProcess@CAInputProcess@@UBE_NABVCInputEvent@@@Z (FableWin donor order Init 0x01AC7D70, Cleanup
// 0x01AC7D80, PreparePrimitivesForEngine 0x01AC7D90, Update 0x01AC7DA0, ProcessWhilePaused 0x01AC7F10 = the slot order 2, 3, 5, 6, 7).
// Needs FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE and FABLE_RETAIL_CAINPUTPROCESS_INIT (slots 1 and 2 are declared by them).
#if !defined(FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE) || !defined(FABLE_RETAIL_CAINPUTPROCESS_INIT)
#error FABLE_RETAIL_INPUTPROCESS_VTABLE needs FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE and FABLE_RETAIL_CAINPUTPROCESS_INIT
#endif
class CEnginePrimitivePrepareInfo;
class CInputEvent;
class CProcessedInput;
namespace NInventory { class CTCInventoryBase; }
#endif
class CAInputProcess : public CBaseClassNonCopyable {
public:
    CAInputProcess();                                                    // 0x00A0D290
    virtual ~CAInputProcess();                                           // slot 0
#ifdef FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
    // Nested opt-in (wf1, row 0x00688D40 CInputProcessZTarget::AddToInterface): Ego_r.pdb ?AddToInterface@CAInputProcess@@UAE_NAAVCInputProcessManager@@@Z
    // (public virtual, slot 1 behind the destructor: CInputProcessZTarget vtable 0x01238028 slot 1 = 0x00688D40), out of line at 0x00A0D2B0. Declaration only.
    virtual bool AddToInterface(CInputProcessManager& manager);          // 0x00A0D2B0, slot 1
#endif
#ifdef FABLE_RETAIL_CAINPUTPROCESS_INIT
    // Nested opt-in (NF325, 2026-11-24; needs FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE): declaration only. Retail vftable 0x0129CA10
    // slot 2 = 0x00486360 ?Init@CAInputProcess@@UAEXXZ (RTTI-port label + FableWin.pdb), called virtually by AddToInterface (`call [eax+8]`).
    virtual void Init();                                                 // 0x00486360, slot 2
#endif
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
    virtual void Cleanup();                                              // slot 3, 0x00486370
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed_input) = 0;   // slot 4, _purecall
    virtual void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& prepare_info) const;   // slot 5, 0x00486380
    virtual void Update(CProcessedInput& processed_input);               // slot 6, 0x00486390
    virtual bool ProcessWhilePaused() const;                             // slot 7, 0x004863F0
    virtual bool IsInputEventToBeSentToThisInputProcess(const CInputEvent& event) const;   // slot 8, 0x00A0D400
#endif
#ifdef FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE
    // Nested opt-in (agentFAM12, rows 0x006336E0 .. CPlayer::UninitMode<X>): Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ,
    // out of line at 0x00A0D2D0 (called on the mode's input process). Declaration only.
    void RemoveFromInterface();                                          // 0x00A0D2D0
#endif
    CInputProcessManager* PPlayerInterface;                              // +0x04
    CBaseClass*           InputProcessListPos_Data;                      // +0x08 CLinkedListPosition::Data
    CLinkedListPosition*  InputProcessListPos_Next;                      // +0x0c CLinkedListPosition::Next
    CLinkedListPosition*  InputProcessListPos_Previous;                  // +0x10 CLinkedListPosition::Previous
};
FABLE_STATIC_ASSERT_AT(33, sizeof(CAInputProcess) == 0x14);
#else // the generated Ego_r layout (the landed default, unchanged)
#ifdef FABLE_RETAIL_INPUTPROCESS_VTABLE
#error FABLE_RETAIL_INPUTPROCESS_VTABLE needs FABLE_RETAIL_INPUTPROCESS (the retail class view)
#endif
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CAInputProcess = 0x14.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CBaseClass;
class CInputProcessManager;
class CLinkedListPosition;

#pragma pack(push, 1)
class CAInputProcess {
public:
    void*                 __vftable;                         // +0x00 vptr, or first dword of a flattened base subobject
    CInputProcessManager* PPlayerInterface;                  // +0x04
    CBaseClass*           InputProcessListPos_Data;          // +0x08 CLinkedListPosition::Data
    CLinkedListPosition*  InputProcessListPos_Next;          // +0x0c CLinkedListPosition::Next
    CLinkedListPosition*  InputProcessListPos_Previous;      // +0x10 CLinkedListPosition::Previous
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(58, sizeof(CAInputProcess) == 0x14);
FABLE_STATIC_ASSERT_AT(59, offsetof(CAInputProcess, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(60, offsetof(CAInputProcess, PPlayerInterface) == 0x4);
FABLE_STATIC_ASSERT_AT(61, offsetof(CAInputProcess, InputProcessListPos_Data) == 0x8);
FABLE_STATIC_ASSERT_AT(62, offsetof(CAInputProcess, InputProcessListPos_Next) == 0xc);
FABLE_STATIC_ASSERT_AT(63, offsetof(CAInputProcess, InputProcessListPos_Previous) == 0x10);

#endif // FABLE_RETAIL_INPUTPROCESS
#endif // FABLE_ENGINE_CAINPUTPROCESS_H
