// CThingTrackNode::`scalar deleting destructor'(unsigned int) -- stores the CThingTrackNode vtable, destroys the string member at +0xe8, runs CThingGameObject::~CThingGameObject and, on bit 0, frees through the thing class's own __fastcall operator delete
// Leaf: ??_GCThingTrackNode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00838500, length 47 bytes (vtable 0x01272CE4 slot 0).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>", propagated),
// CThingGameObject::~CThingGameObject @ 0x00838A00, the thing class's operator delete @ 0x004C73B0
// (`__fastcall`, object in ecx; the manifest's "operator_new" is propagated).
// Shape: own vtable store, one member destructor, the base destructor; the free has no `push`/`add esp`
// because CThing-derived objects are released through a class-scope `static void __fastcall
// operator delete(void*)` (the pointer is handed over in ecx). Layout as the landed ??1 0x00836D00:
// PDB CThingTrackNode is 264 bytes, retail 0x18 smaller (the CThingGameObject base is 0xd0 bytes): the
// two track words +0xd0/+0xd4, NextNodeUID +0xd8, PreviousNodeUID +0xe0, TrackName +0xe8, IsStart +0xec.
class CThingTrackNode;

class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

class CThingGameObject { public:                          // the polymorphic thing base (vptr at +0)
    virtual void v00();
    unsigned char _pad04[0xcc];                    // +0x04
    CThingGameObject();
    ~CThingGameObject();                           // 0x00838A00
    static void __fastcall operator delete(void* p);   // 0x004C73B0
};

#pragma pack(push, 4)
class CThingTrackNode : public CThingGameObject { public:        // vtable 0x01272CE4
    long             m_trackWordD0;                // +0xd0 (CThingTrackNode_u_232)
    CThingTrackNode* PreviousNode;                 // +0xd4 (CThingTrackNode_u_236)
    unsigned __int64 NextNodeUID;                  // +0xd8
    unsigned __int64 PreviousNodeUID;              // +0xe0
    CCharString      TrackName;                    // +0xe8
    bool             IsStart;                      // +0xec
    bool             IsEnd;                        // +0xed
    CThingTrackNode() { }
    __forceinline virtual ~CThingTrackNode() { }
};
#pragma pack(pop)

CThingTrackNode* Create_CThingTrackNode()
{
    return new CThingTrackNode;
}