// CCompositeSurfaceInfo::`scalar deleting destructor'(unsigned int flags) -- run the implicit destructor inline (release the counted pointer at +0x04, clearing Data then Info; then CBaseClass::~CBaseClass 0x0099A2E0) and free the object when bit 0 of the flags is set
// Leaf: ??_GCCompositeSurfaceInfo@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00beda30, 81 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): vtable 0x012A8154 (RTTI .?AVCCompositeSurfaceInfo@@, base CBaseClass) slot 0; Ego_r.pdb CCompositeSurfaceInfo::Cache (CCountedPointer<CCompositeSurfaceCache>, +4).
// Callees: the CCPPointerInfo deleter (fastcall), operator delete 0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0.
// Shape (family L036): ~CCountedPointer inline (`--Info->RefCount`, deleter, delete Info, Data = Info = 0), then the
//   base or first-member destructor out of line; `new` emits the vtable and the ??_G, the explicit `p->X::~X()` keeps the ??_G inlining the implicit destructor.
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
class CCPPointerInfo {
public:
    long RefCount;                                        // +0x00
    void (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                           // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes: Data, Info
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    ~CCountedPointer()                                    // clears Data before Info
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CBaseClass {                                        // Fable.exe RTTI root
public:
    CBaseClass();
    virtual ~CBaseClass();                                // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                     // 0x0099A300
};
class CCompositeSurfaceCache;
class CCompositeSurfaceInfo : public CBaseClass {          // Ego_r.pdb, 12 bytes
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;        // +0x04
};
CCompositeSurfaceInfo* MakeCCompositeSurfaceInfo() { return new CCompositeSurfaceInfo; }
void KillCCompositeSurfaceInfo(CCompositeSurfaceInfo* p) { p->CCompositeSurfaceInfo::~CCompositeSurfaceInfo(); }