#ifndef FABLE_ENGINE_CINPUTEVENT_H
#define FABLE_ENGINE_CINPUTEVENT_H
// CInputEvent (Ego_r.pdb, class V, 0x34 bytes; the landed members' model, rows 0x00A03B40 / 0x00A03B70 / 0x00A04410):
// UInputEventData Data +0x00 (32-byte union), EInputDeviceType DeviceType +0x20, long DeviceNumber +0x24,
// EInputEventType Type +0x28, float CurrentTime +0x2C, float StartTime +0x30.
// Member functions (Ego_r.pdb, in donor order ??1 0x00B6E5F0, GetType 0x00B6E600, GetKey 0x00B6E630, ??0 0x00B6EFE0;
// retail 0x00A03B30 `ret`, 0x00A03B40, 0x00A03B70, 0x00A04410): the constructor and the empty destructor are out of line.
// The enum types' enumerator names are not in the repo's PDB extracts: declared without enumerators (landed convention).
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CINPUTEVENT_ACCESSORS
class C2DVector;
#endif
enum EInputKey { };
enum EInputDeviceType { };
enum EInputEventType { };
union UInputEventData {                                                  // 32 bytes (Ego_r.pdb: two anonymous structs)
    unsigned char Raw[0x20];
};
class CInputEvent {
public:
    CInputEvent();                                                       // 0x00A04410 ??0CInputEvent@@QAE@XZ
    ~CInputEvent();                                                      // 0x00A03B30 ??1CInputEvent@@QAE@XZ
#ifdef FABLE_RETAIL_CINPUTEVENT_COPY
    // Opt-in (chunk 760, row 0x009F5690 CInputEventPackage(CAFile&)): FableWin.pdb ??0CInputEvent@@QAE@ABV0@@Z, out of line in
    // retail (0x009E4850, called by push_back's placement copy).
    CInputEvent(const CInputEvent&);                                     // 0x009E4850
#endif
    EInputEventType GetType() const;                                     // 0x00A03B40 ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ
    EInputKey GetKey() const;                                            // 0x00A03B70 ?GetKey@CInputEvent@@QBE?AW4EInputKey@@XZ
#ifdef FABLE_RETAIL_CINPUTEVENT_ACCESSORS
    // Retail opt-in (chunk 791, row 0x004963A0 CFrontendGameComponent::Input): three more out-of-line accessors, declarations only
    // (no layout change). Ego_r.pdb / FableWin.pdb ?GetDeviceNumber@CInputEvent@@QBEJXZ (0x00A03B60), ?GetJoystickButton@CInputEvent@@QBEJXZ
    // (0x00A03B90) and ?GetJoystickPosition@CInputEvent@@QBE?AVC2DVector@@XZ (0x00A043A0, hidden result: 0x00496433 pushes the
    // result slot and passes the event in ecx).
    long GetDeviceNumber() const;                                        // 0x00A03B60
    long GetJoystickButton() const;                                      // 0x00A03B90
    C2DVector GetJoystickPosition() const;                               // 0x00A043A0
#endif
    UInputEventData  Data;                                               // +0x00
    EInputDeviceType DeviceType;                                         // +0x20
    long             DeviceNumber;                                       // +0x24
    EInputEventType  Type;                                               // +0x28
    float            CurrentTime;                                        // +0x2C
    float            StartTime;                                          // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CInputEvent) == 0x34);
#endif // FABLE_ENGINE_CINPUTEVENT_H
