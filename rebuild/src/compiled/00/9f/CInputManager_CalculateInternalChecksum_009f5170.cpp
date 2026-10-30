// CInputManager::CalculateInternalChecksum -- combine stored events, device axes and update count
// Retail009F5170 length211, const unsigned-long result; GetChecksum00A04160,
// intrinsic ftol2 thunk00BFEA70; pool0122DCB4=4294967296.0f for unsigned32 conversion.
// Native vector12 versus donor16 gives manager0x80 versus0x90. Devices retained
// from actual donor headers, with authentic virtual slot prefixes and base hierarchy.
// On the full-width host fable-style unsigned-int accumulators preserve native32 wrap.
#include "engine/CDiskFileWin32.h"
#ifdef _MSC_VER
#define NATIVE_ONLY(x) FABLE_STATIC_ASSERT(x)
#else
#define NATIVE_ONLY(x)
#endif
class CInputEvent;
class CInputEventPackage;
class CMouse;
class CKeyboard;
class CJoystick;
struct CCountedJoystick;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template<class T> struct NativeVector { T* Start; T* Finish; T* End; struct iterator { T* current; iterator(T* p):current(p){} iterator operator+(int n)const{return iterator(current+n);} T& operator*()const{return *current;} }; iterator begin()const{return iterator(Start);} T& operator[](int n) const { return *(begin()+n); } };
template<class T> struct NativeScoped { T* ptr; T* get() const {return ptr;} T& operator*() const {return *ptr;} T* operator->() const{return ptr;} };
class CChecksum { public: unsigned int Value[4]; };
enum EInputEventType { INPUT_NULL, INPUT_KEY_PRESSED, INPUT_KEY_HELD, INPUT_KEY_RELEASED };
enum EInputKey { INPUT_KEY_NONE };
class CInputEvent { public:
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
class CInputManagerInit { public: bool UseMouse,UseJoystick,UseKeyboard; };
class CInputManager : public CBaseClass { public:
    CInputManager(const CInputManagerInit&,bool); // authentic009F5BD0 dependency
    NativeVector<CInputEvent> ProcessedEvents;
    NativeVector<CInputEvent> EventStore;
    int NoStoredEvents;
    int NoEventScansRunning;
    NativeScoped<CMouse> PMouse;
    NativeScoped<CKeyboard> PKeyboard;
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
    unsigned long CalculateInternalChecksum() const;
};
NATIVE_ONLY(sizeof(CInputEvent)==0x34);
NATIVE_ONLY(sizeof(CInputManager)==0x80);
NATIVE_ONLY(offsetof(CInputManager,EventStore)==0x10);
NATIVE_ONLY(offsetof(CInputManager,NoStoredEvents)==0x1c);
NATIVE_ONLY(offsetof(CInputManager,PackageFile)==0x60);

// Retain actual donor device layouts under aliases. Native virtual views below
// use authentic slot prefixes; no storage/virtual-slot padding is introduced.
#define CMouse DonorCMouse
#include "engine/CMouse.h"
#undef CMouse
#define CJoystick DonorCJoystick
#include "engine/CJoystick.h"
#undef CJoystick
class C2DVector { public: float X,Y; C2DVector(){} C2DVector(float x,float y):X(x),Y(y){} };
struct CBaseClassNonCopyable : CBaseClass { CBaseClassNonCopyable(); virtual ~CBaseClassNonCopyable(); };
class CMouse : public CBaseClassNonCopyable { public:
    virtual ~CMouse();
    virtual void GetPos(C2DVector*) const =0; // native slot1; DX override00AB46E0
    unsigned char EventStore[0x3400];
    int NoStoredEvents,NoEventScansRunning;
    unsigned char MaintainedEventsList[4];   // retail STLport 4.6 list (head pointer), as engine/CMouse.h
    CInputManager* InputManager;
};
// RTTI proves CJoystick has no base class; axes are non-const protected virtuals.
class CJoystick { public:
    virtual ~CJoystick();
    virtual bool SetExclusiveMode(bool)=0;
    virtual void Update()=0;
    virtual float GetXAxis()=0; // native slot3 DX00AB6AD0
    virtual float GetYAxis()=0; // native slot4 DX00AB6AE0
    unsigned char EventStore[0xd00];
    CInputManager* InputManager;
    bool ExclusiveMode;
    unsigned char MaintainedEventsList[4];   // retail STLport 4.6 list (head pointer), as engine/CJoystick.h
    int NoStoredEvents,NoEventScansRunning;
};
struct CCountedJoystick { CJoystick* Data; void* Info; CJoystick* get() const{return Data;} CJoystick* operator->()const{return Data;} };
NATIVE_ONLY(sizeof(CMouse)==sizeof(DonorCMouse));
NATIVE_ONLY(sizeof(CJoystick)==sizeof(DonorCJoystick));
unsigned long CInputManager::CalculateInternalChecksum() const {
    unsigned int checksum=NoStoredEvents;
    C2DVector mousePos(0,0);
    C2DVector joystickPos(0,0);
    if(PMouse.get()) PMouse->GetPos(&mousePos);
    if(PJoysticks[0].get()) {
        joystickPos.X=PJoysticks[0]->GetXAxis();
        joystickPos.Y=PJoysticks[0]->GetYAxis();
    }
    for(int i=0;i<NoStoredEvents;++i) checksum+=EventStore[i].GetChecksum();
    checksum=(unsigned int)(checksum+joystickPos.Y+joystickPos.X);
    checksum=(unsigned int)(checksum+mousePos.X+mousePos.Y);
    return checksum+NoUpdates;
}