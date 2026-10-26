// CInputManager::CInputManager -- initialize input queues, devices, package recorder and counters
// Leaf: ??0CInputManager
// Retail009F5BD0 length161. Actual donor manager/event headers do not exist.
// Native vector12 versus donor16 gives manager0x80 versus0x90. Vector end pointer
// uses the genuine allocator-proxy representation; native file size0x1C unchanged.
// Calls CBaseClass0099A2D0,NonCopyable0099A2F0,CWideString0099AED0,
// vector<CInputEvent>::erase009F67E0 twice; vtables129C79C/122D06C.
// The native file->CAFile->NonCopyable->BaseClass hierarchy is independently RTTI-proven.
// All fixed-width int members project native32-bit long storage on the full-width host.
#define CDiskFileWin32 DonorCDiskFileWin32
#include "engine/CDiskFileWin32.h"
#undef CDiskFileWin32
#define CAFile DonorCAFile
#include "fable_filesystem.h"
#undef CAFile
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
class CBaseClass { public:
    CBaseClass(); // 0099A2D0; native vtable01231710
    virtual ~CBaseClass();
};
template<class T> struct NativeAllocator {};
template<class T> struct NativeStorageEnd:NativeAllocator<T> { T* Value; NativeStorageEnd(T* p):Value(p){} };
template<class T> struct NativeVector { T* Start; T* Finish; NativeStorageEnd<T> End; NativeVector():Start(0),Finish(0),End(Start){} T* erase(T*,T*); void clear(){erase(Start,Finish);} struct iterator { T* current; iterator(T* p):current(p){} iterator operator+(int n)const{return iterator(current+n);} T& operator*()const{return *current;} }; iterator begin()const{return iterator(Start);} T& operator[](int n) const { return *(begin()+n); } };
template<class T> struct NativeScoped { T* ptr; NativeScoped():ptr(0){} T* get() const {return ptr;} T& operator*() const {return *ptr;} T* operator->() const{return ptr;} };
class CChecksum { public: int Checksum1,Checksum2,Checksum3,Checksum4; CChecksum():Checksum1(0),Checksum2(0),Checksum3(0),Checksum4(0){} };
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

class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { FILE_READ=1,FILE_WRITE=2 };
#ifdef _MSC_VER
#define NATIVE_NOVTABLE __declspec(novtable)
#else
#define NATIVE_NOVTABLE
#endif
struct CBaseClassNonCopyable : CBaseClass { CBaseClassNonCopyable(); virtual ~CBaseClassNonCopyable(); };
struct NATIVE_NOVTABLE CAFile : CBaseClassNonCopyable {
    virtual ~CAFile();
    virtual void Open(const CWideString&,FileAccessType,unsigned long)=0;
    virtual void Close()=0;
    virtual void Read(void*,long,CASuspendableProcess*)=0;
    virtual void Write(const void*,long,CASuspendableProcess*)=0;
    virtual void SetPosition(long)=0;
    virtual void MovePosition(long)=0;
    virtual long GetPosition() const=0;
    virtual void SetLength(long)=0;
    virtual long GetLength()const=0;
    virtual bool IsOpen()const=0;
    virtual CWideString GetPathname()const=0;
    virtual bool IsIOFinished()const;
    virtual long GetAlignment()const;
    virtual CAFile* Clone()const=0;
    virtual CFileInformation GetFileInformation()=0;
};
class CDiskFileWin32 : public CAFile { public:
    FileAccessType AccessType;
    int Position,Length;
    void* WinFileHandle;
    bool Opened;
    CWideString Pathname;
    CDiskFileWin32(){Opened=false;}
    virtual ~CDiskFileWin32();
    virtual void Open(const CWideString&,FileAccessType,unsigned long);
    virtual void Close();
    virtual void Read(void*,long,CASuspendableProcess*);
    virtual void Write(const void*,long,CASuspendableProcess*);
    virtual void SetPosition(long);
    virtual void MovePosition(long);
    virtual long GetPosition()const;
    virtual void SetLength(long);
    virtual long GetLength()const;
    virtual bool IsOpen()const;
    virtual CWideString GetPathname()const;
    virtual CAFile* Clone()const;
    virtual CFileInformation GetFileInformation();
};
NATIVE_ONLY(sizeof(CDiskFileWin32)==sizeof(DonorCDiskFileWin32));
class CInputManagerInit { public: bool UseMouse,UseJoystick,UseKeyboard; };
class CInputManager : public CBaseClass { public:
    CInputManager(const CInputManagerInit&,bool);
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


CInputManager::CInputManager(const CInputManagerInit&,bool) {
    ProcessedEvents.clear();
    EventStore.clear();
    NoStoredEvents=0;
    NoEventScansRunning=0;
    TimeStamp=0;
    Loading=false;
    Saving=false;
    NoUpdates=0;
}