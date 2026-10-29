// CThingDeadCreature::~CThingDeadCreature() -- dead-creature thing dtor: empty user body, then the member dtors in reverse order (delete the body reorienter, release the killer intelligent pointer, release the anim-filter counted pointer) and the tail jump to ~CThingGameObject
// Leaf: ??1CThingDeadCreature@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00835B10, 120 bytes, tail jump to the base destructor.
// Identity: the one vptr store 0x01272B04 carries RTTI `.?AVCThingDeadCreature@@`; Ego_r.pdb has
//   `??1CThingDeadCreature@@UAE@XZ` (virtual). The catalog label is right.
// Callees: CThingBodyReorienter::~CThingBodyReorienter 0x00863790 (Ego_r.pdb `??1CThingBodyReorienter@@QAE@XZ`, non-virtual,
//   so boost::checked_delete calls it directly; the manifest label "Catch@0047e8cc" is wrong), operator delete 0x00BFE9BC
//   (twice: the reorienter and the counted pointer's info block), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10
//   (PKiller: CIntelligentPointer<T> has no destructor of its own in either PDB, so its implicit one only calls the base),
//   CThingGameObject::~CThingGameObject 0x00838A00 (Ego_r.pdb `??1CThingGameObject@@UAE@XZ`, tail jump); the info block's
//   DeleteFunc is a __fastcall pointer (`mov ecx,[info+8] ; call [info+4]`).
// Layout: Ego_r.pdb CThingDeadCreature (0x108) PAnimFilter +0xe8, PKiller +0xf0, BodyDieTimer +0xf8, BodyReorienter +0xfc,
//   StartedDecay +0x100; retail is 0x18 narrower (STLport containers in the bases): +0xd0 / +0xd8 / +0xe0 / +0xe4 / +0xe8, as
//   the landed ctor 0x00835C20 has it. engine/CThingGameObject.h models the vptr as a data member, so a local polymorphic
//   model of the 0xd0-byte base is used (NOTES.md Header debt).
// Shape: boost::scoped_ptr's dtor is `checked_delete(ptr)` (a null-tested direct dtor call + operator delete); CCountedPointer's
//   inline release (`ReleaseData(); Data = 0; Info = 0;`) re-reads Info after each step, as in the landed CTCLook 0x00753290.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

namespace boost {
template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void) sizeof(type_must_be_complete);
    delete x;
}
template <class T> class scoped_ptr { public:
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
private:
    T* ptr;
};
} // namespace boost

class CBaseObjectPointer;
class CBaseIntelligentPointer { public:                     // 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

class CThingBodyReorienter { public:
    unsigned char _pad_0x00[0x04];
    ~CThingBodyReorienter();                                // 0x00863790
};
class CAnimFilterBase;
class CThingCreatureBase;

class CThingGameObject { public:                            // retail 0xd0 bytes (Ego_r 0xe8)
    virtual ~CThingGameObject();                            // 0x00838A00
    unsigned char _pad_0x04[0xcc];
};

class CThingDeadCreature : public CThingGameObject { public: // vtable 0x01272B04
    CCountedPointer<CAnimFilterBase>                PAnimFilter;        // +0xd0
    CIntelligentPointer<const CThingCreatureBase>   PKiller;            // +0xd8
    long                                            BodyDieTimer;       // +0xe0
    boost::scoped_ptr<CThingBodyReorienter>         BodyReorienter;     // +0xe4
    bool                                            StartedDecay;       // +0xe8
    virtual ~CThingDeadCreature();
};

CThingDeadCreature::~CThingDeadCreature()
{
}