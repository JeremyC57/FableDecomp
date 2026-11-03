#ifndef FABLE_ENGINE_CINPUTMANAGER_H
#define FABLE_ENGINE_CINPUTMANAGER_H
// Retail CInputManager (movie row 0x006286F0). RTTI (vtable 0x0129C79C) CInputManager : CBaseClass; slots 0 ??_E
// 0x009F6FC0, 1 0x009F4E70, 2 0x009F57A0. Virtual set (Ego_r.pdb, the only UAE members): ~CInputManager,
// SetExclusiveMode(bool) 0x00B48A60, Update() 0x00B49340, declared in that order (the landed CVideoSys::Init row 0x00A3B9D0
// calls slot 2 as Update). Size 0x80: the derived constructor at 0x00A60050 calls ??0CInputManager (0x009F5BD0) and
// constructs its own first member at +0x80; = Ego_r's 0x90 with its four Dinkumware vectors (0x10) at STLport size (0xC).
// The data members (Ego_r.pdb ProcessedEvents +0x04, EventStore +0x10, NoStoredEvents +0x1C, NoEventScansRunning +0x20,
// PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2C, Loading +0x38, Saving +0x39, Checksum +0x3C, LoadedEventPackages +0x4C,
// CurrentFilePackageIndex +0x58, TimeStamp +0x5C, PackageFile +0x60, NoUpdates +0x7C at STLport sizes; only the total is
// read back by retail code here) are kept opaque.
#include "engine/CBaseClass.h"
class CInputEvent; class CInputEventScanInfo;
class CInputManager : public CBaseClass {
public:
    virtual ~CInputManager();                                            // slot 0
    virtual bool SetExclusiveMode(bool);                                 // slot 1 (+0x04) ?SetExclusiveMode@CInputManager@@UAE_N_N@Z
    virtual void Update();                                               // slot 2 (+0x08) ?Update@CInputManager@@UAEXXZ
    bool EventScanStart(CInputEventScanInfo&,CInputEvent&) const;        // 0x009F4ED0 ?EventScanStart@CInputManager@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z
    bool EventScanNext(CInputEventScanInfo&,CInputEvent&) const;         // 0x009F4F10 ?EventScanNext@CInputManager@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z
private:
    unsigned char _pad_0x04[0x80 - 0x04];                                // +0x04 data members (opaque, see above)
};
FABLE_STATIC_ASSERT(sizeof(CInputManager) == 0x80);
#endif // FABLE_ENGINE_CINPUTMANAGER_H
