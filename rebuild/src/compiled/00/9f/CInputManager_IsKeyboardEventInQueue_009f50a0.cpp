// CInputManager::IsKeyboardEventInQueue -- search a keyboard template and copy the matched event
// Retail 009F50A0 length201. Event has native/donor size0x34. No manager/event header exists.
// Manager donor0x90 uses16-byte debug vectors; native STL vectors12 give size0x80.
// Calls event ctor A04410, setters A03BF0/A03C20/A03C50, search9F4D40, dtor A03B30.
// Literal double0.0 = native pool0122ED70. PDB method signatures are const-qualified.
#include "engine/CDiskFileWin32.h"
#ifdef _MSC_VER
#define NATIVE_ONLY(x) FABLE_STATIC_ASSERT(x)
#else
#define NATIVE_ONLY(x)
#endif
struct CInputEvent;
struct CInputEventPackage;
struct CMouse;
struct CKeyboard;
struct CJoystick;
struct CCountedJoystick;
struct CBaseClass {
    CBaseClass(); // 0099A2D0; native vtable01231710
    virtual ~CBaseClass();
};
template<class T> struct NativeVector { T* Start; T* Finish; T* End; };
struct CChecksum { unsigned int Value[4]; };
enum EInputEventType { INPUT_NULL, INPUT_KEY_PRESSED, INPUT_KEY_HELD, INPUT_KEY_RELEASED };
enum EInputKey { INPUT_KEY_NONE };
struct CInputEvent {
    unsigned char Data[32]; // actual UInputEventData; no member interpreted in this method
    int DeviceType;
    int DeviceNumber;
    EInputEventType Type;
    float CurrentTime;
    float StartTime;
    CInputEvent(); // 00A04410
    ~CInputEvent(); // 00A03B30
    void SetAsKeyboardKeyPressed(EInputKey,double); // 00A03BF0
    void SetAsKeyboardKeyHeld(EInputKey,double,double); // 00A03C20
    void SetAsKeyboardKeyReleased(EInputKey,double,double); // 00A03C50
    unsigned long GetChecksum() const; // 00A04160
};
struct CInputManagerInit { bool UseMouse,UseJoystick,UseKeyboard; };
struct CInputManager : CBaseClass {
    CInputManager(const CInputManagerInit&,bool); // authentic009F5BD0 dependency
    NativeVector<CInputEvent> ProcessedEvents;
    NativeVector<CInputEvent> EventStore;
    int NoStoredEvents;
    int NoEventScansRunning;
    CMouse* PMouse;
    CKeyboard* PKeyboard;
    NativeVector<CCountedJoystick> PJoysticks;
    bool Loading;
    bool Saving;
    CChecksum Checksum;
    NativeVector<CInputEventPackage> LoadedEventPackages;
    int CurrentFilePackageIndex;
    int TimeStamp;
    CDiskFileWin32 PackageFile;
    unsigned int NoUpdates;
    virtual ~CInputManager();
    virtual bool SetExclusiveMode(bool); // vtable slot1
    virtual void Update(); // slot2
    bool IsEventInQueue(const CInputEvent&) const; // 009F4D40
    bool IsKeyboardEventInQueue(EInputEventType,EInputKey,CInputEvent&) const;
};
NATIVE_ONLY(sizeof(CInputEvent)==0x34);
NATIVE_ONLY(sizeof(CInputManager)==0x80);
NATIVE_ONLY(offsetof(CInputManager,EventStore)==0x10);
NATIVE_ONLY(offsetof(CInputManager,NoStoredEvents)==0x1c);
NATIVE_ONLY(offsetof(CInputManager,PackageFile)==0x60);
bool CInputManager::IsKeyboardEventInQueue(EInputEventType type,EInputKey key,CInputEvent& event) const {
    CInputEvent search;
    switch(type) {
    case INPUT_KEY_PRESSED: search.SetAsKeyboardKeyPressed(key,0.0); break;
    case INPUT_KEY_HELD: search.SetAsKeyboardKeyHeld(key,0.0,0.0); break;
    case INPUT_KEY_RELEASED: search.SetAsKeyboardKeyReleased(key,0.0,0.0); break;
    }
    if(IsEventInQueue(search)==true) { event=search; return true; }
    return false;
}