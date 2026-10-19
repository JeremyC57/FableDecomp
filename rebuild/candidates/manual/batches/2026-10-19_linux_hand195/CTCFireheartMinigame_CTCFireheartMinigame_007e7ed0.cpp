// CTCFireheartMinigame::~CTCFireheartMinigame() -- release the minigame def, destroy and free the primitive-handle vector, release the scripted-thing handle, then the CTCBase destructor
// Leaf: ??1CTCFireheartMinigame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E7ED0, length 163 bytes (tail `jmp` to the base destructor).
// Label correction: the catalog "CMeshData::~CMeshData" is propagated; the only caller is the deleting destructor 0x007E7EB0 in slot 0 of
// vtable 0x012709CC (CTCFireheartMinigame, rtti-vtables.tsv).
// Callees: the def's virtual Delete (slot 1), each CEnginePrimitiveHandle's virtual destructor (slot 0 with 0), free @ 0x00BFEA14 (the
// vector block), the counted block's destroy member pointer and operator delete @ 0x00BFE9BC, CBaseObject::~CBaseObject @ 0x0099A430,
// CTCBase::~CTCBase @ 0x00686830 (labelled ~CChunkedFileChunk).
// Layout: the landed CTCFireheartMinigame ctor 0x007E7640 model (Ego_r.pdb names): ScriptData +0x78 (CBaseObject + PImp {Data, Info}),
// Handles +0x8C (12-byte STLport vector of 8-byte handles), PDef +0x98.
// Shape: no vtable store of the class itself (`__declspec(novtable)` in this TU) and the scripted-thing class has an implicit destructor,
// so the only store is CScriptGameResourceObjectBase's (0x0126008C, an inline empty destructor) right before ~CBaseObject; the def is
// released through a local copy (`dec [ecx+4] ; jne`); the counted block holds {count, destroy member pointer, object}.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
void operator delete(void* p);                  // 0x00BFE9BC
struct CThing;
struct CFireheartMinigameDef {
    virtual void v00();
    virtual void Delete() const;                // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            T* object = Object;
            if (--const_cast<CFireheartMinigameDef*>(object)->RefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};
struct CBaseObject {
    virtual ~CBaseObject();                     // 0x0099A430
    void* IntelligentPointer;                   // +0x04
};
struct CScriptThingImp {                        // complete before the member pointer: single-inheritance 4-byte form
    void DestroySelf();
};
struct SCountBlock {                            // the counted pointer's shared block
    long Count;                                 // +0x00
    void (CScriptThingImp::*Destroy)();         // +0x04
    CScriptThingImp* Object;                    // +0x08
};
struct CCountedPointer {
    CScriptThingImp* Data;                      // +0x00
    SCountBlock*     Info;                      // +0x04
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                (Info->Object->*Info->Destroy)();
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptGameResourceObjectBase : CBaseObject {   // vtable 0x0126008C
    ~CScriptGameResourceObjectBase() { }
};
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, implicit dtor
    CCountedPointer PImp;                       // +0x08
};
struct CEnginePrimitiveHandle {                 // 8 bytes, virtual destructor
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};
template <class T> __forceinline void _Destroy(T* first, T* last)   // STLport: bounds by value
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CRetailVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    ~CTCBase();                                 // 0x00686830
};
struct __declspec(novtable) CTCFireheartMinigame : CTCBase {
    CThing*  Markers[4][4];                     // +0x0c
    bool     InSession;                         // +0x4c
    bool     ForceQuit;                         // +0x4d
    CThing*  CurrentMarker;                     // +0x50
    bool     Initialised;                       // +0x54
    bool     BlocksActive[4][4];                // +0x55
    long     XPos;                              // +0x68
    long     YPos;                              // +0x6c
    bool     Moving;                            // +0x70
    CTCBase* PTCOldPhysics;                     // +0x74
    CScriptGameResourceObjectScriptedThingBase ScriptData;   // +0x78
    bool     QuitWithDelay;                     // +0x88
    CRetailVector<CEnginePrimitiveHandle> Handles;           // +0x8c
    CDefPointer<const CFireheartMinigameDef> PDef;           // +0x98
    ~CTCFireheartMinigame();
};

CTCFireheartMinigame::~CTCFireheartMinigame()
{
}
