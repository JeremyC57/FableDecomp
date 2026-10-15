// NUISystem::CMovie::CMovie(const CMovie&) -- copy-construct the movie component: copies only the draggable base, rebuilds the handle, movie name and playback info fresh and re-reads the def
// Leaf: ??0CMovie@NUISystem@@QAE@ABV01
// Retail: Fable.exe (TLC, VC7.1) @ 0x00548350, 108 bytes (+ int3 padding; `ret 4`; manifest label CMovie::CopyConstructor).
// Callees: NUISystem::CDraggableInto::CDraggableInto(const CDraggableInto&) @ 0x0052CCA0, CBaseClass::CBaseClass() @ 0x0099A2D0,
//   CWideString::CWideString() @ 0x0099AED0 (manifest label "CCharString" is a byte twin), CXMVPlaybackInfo::CXMVPlaybackInfo() @ 0x009BC7D0,
//   NUISystem::CMovie::GetDataFromDef() @ 0x00547C10. Vtables 0x0124881C / 0x012487F4 / 0x012487EC (CMovie at +0, +4, +0x18),
//   CEnginePrimitiveHandle 0x0122F598; the engine reference is kept in the global 0x013B8AB8.
// Retail layout: Ego_r.pdb minus 0x30 (the retail CDraggableInto is 0x15C bytes): Playing +0x15c, Handle +0x160, Width +0x168,
//   Height +0x16c, PositionIsCenter +0x170, Movie +0x174, Playback +0x178, Angle +0x190, AttempToPlay +0x194.
// Shape: three vtable stores, then the members in declaration order: the Playing flag, the handle (out-of-line CBaseClass ctor,
//   its own vtable and the null Data), the string and playback ctors, the AttempToPlay flag; the body only re-reads the def
//   (the user copy constructor copies none of the movie's own members and leaves the engine global alone).
struct CIEngine;

class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CEnginePersistentPrimitiveBase;
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    CEnginePrimitiveHandle() { }
};
struct CWideString {
    void* PStringData;
    CWideString();                                 // 0x0099AED0
};
struct CXMVPlaybackInfo {
    unsigned char _data[0x18];
    CXMVPlaybackInfo();                            // 0x009BC7D0
};

namespace NUISystem {
struct CComponentInterface {
    virtual void v000();
};
struct CDrawableInterface {
    virtual void v000();
    unsigned char _pad04[0x10];
};
struct CDraggableInterface {
    virtual void v000();
};
struct CDraggableInto : CComponentInterface, CDrawableInterface, CDraggableInterface {
    unsigned char _pad1c[0x15c - 0x1c];
    CDraggableInto(long id);                       // 0x0052CC50
    CDraggableInto(const CDraggableInto& other);   // 0x0052CCA0
};
struct CMovie : CDraggableInto {
    bool                   Playing;                // +0x15c
    CEnginePrimitiveHandle Handle;                 // +0x160
    unsigned long          Width;                  // +0x168
    unsigned long          Height;                 // +0x16c
    bool                   PositionIsCenter;       // +0x170
    CWideString            Movie;                  // +0x174
    CXMVPlaybackInfo       Playback;               // +0x178
    float                  Angle;                  // +0x190
    bool                   AttempToPlay;           // +0x194
    static CIEngine*       Engine;                 // 0x013B8AB8
    CMovie(CIEngine& engine, long id);
    CMovie(const CMovie& other);
    virtual void v000();
    void GetDataFromDef();                         // 0x00547C10
};
CMovie::CMovie(const CMovie& other)
    : CDraggableInto(other), Playing(false), AttempToPlay(false)
{
    GetDataFromDef();
}
}
