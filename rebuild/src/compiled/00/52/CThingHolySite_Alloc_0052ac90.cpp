// CThingHolySite::Alloc() -- the class-registry factory for CThingHolySite: allocate and construct a holy site (thing type 7): the physical/game-object base chain and a null def pointer; null when the thing manager refuses the allocation
// Leaf: ?Alloc@CThingHolySite
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052AC90, 79 bytes, `ret` (Ego_r.pdb `?Alloc@CThingHolySite@@SIPAVCClass@@XZ`: static __fastcall,
//   no arguments, returns CClass*).
// Callees: CThing::operator new(unsigned int, bool) 0x004C7380 (landed; size 0xd8, flag false), CThingPhysical::
//   CThingPhysical(long type) 0x005296B0 (type 7), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10.
// Vtables: CThingGameObject 0x01244F7C, CIntelligentPointer<CThing> 0x01238C6C, CThingHolySite 0x012452DC (rtti-vtables.tsv).
// Callers: a call at 0x0083BBFB (the CThingHolySite TU) and the class-registry reference at 0x00522E99 (CThingManager TU).
// Layout: Ego_r.pdb CThingHolySite (0xf0) has Def (CDefPointer<const CThingHolySiteDef>) +0xe8, PlayerStart +0xec;
//   retail is 0x18 bytes narrower: +0xd0 / +0xd4, 0xd8 bytes. Local retail model (engine/CThingGameObject.h is the PDB
//   layout).
// Shape: `new (false) T` through CThing's protected static __fastcall operator new(size, bool); the CDefPointer member's
//   inline default ctor stores the zero after the derived vtable.
class CThingHolySiteDef;
template <class T> class CDefPointer { public:         // 4 bytes: the counted def pointee
    T* Object;
    CDefPointer() : Object(0) { }
};

class CClass {                                  // the reflection root every thing class derives from (vptr owner)
public:
    virtual void v00();
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {            // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThing : public CClass {
protected:
    static void* __fastcall operator new(unsigned int size, bool flag);   // 0x004C7380 (null when the manager refuses)
};

class CThingPhysical : public CThing {
public:
    unsigned char _pad04[0xbc];                 // +0x04 .. +0xbf (CThing + CThingPhysical members, not touched here)
    CThingPhysical(long type);                  // 0x005296B0
};

class CThingGameObject : public CThingPhysical {           // vtable 0x01244F7C
public:
    CIntelligentPointer<CThing> PThingStandingOn;             // +0xc0
    unsigned char m_gameObjectFlags;            // +0xc8 (bitfield group, not initialised here)
    unsigned char _padc9[7];                    // +0xc9 tail padding (the class is 8-aligned through its 64-bit UID)
    virtual void v00();
    CThingGameObject(long type) : CThingPhysical(type) { }
};

class CThingHolySite : public CThingGameObject {          // vtable 0x012452DC
public:
    CDefPointer<const CThingHolySiteDef> Def;   // +0xd0
    unsigned char PlayerStart;                  // +0xd4 (not initialised)
    virtual void v00();
    CThingHolySite() : CThingGameObject(7) { }
    static CClass* __fastcall Alloc();
};

CClass* __fastcall CThingHolySite::Alloc()
{
    return new (false) CThingHolySite;
}