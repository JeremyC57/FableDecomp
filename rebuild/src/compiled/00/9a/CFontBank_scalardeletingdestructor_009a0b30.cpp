// CFontBank::`scalar deleting destructor'(unsigned int) -- stores the CFontBank vtable, destroys the bank name string (+8), runs CBaseClassNonCopyable::~CBaseClassNonCopyable and frees on bit 0
// Leaf: ??_GCFontBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A0B30, length 46 bytes (vtable 0x0129AB38 slot 0).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>", propagated), CBaseClassNonCopyable::~CBaseClassNonCopyable
// @ 0x0099A300, operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CFontBank vtable 0x0129AB38.
// Layout: engine/CFontBank.h (vptr, FontHeight +4, Name +8 as its string-data word, PManager +0x0c); the
// class is modelled locally as a polymorphic CBase-derived class over the same offsets, with the name
// as a real CCharString member. Shape: own vtable store, one member destructor, the base destructor.
class CFontManager;

class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

class CBaseClassNonCopyable {
protected:                                      // root polymorphic base
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                              // 0x0099A300
};

class CFontBank : public CBaseClassNonCopyable { public:                         // vtable 0x0129AB38
    unsigned long FontHeight;                      // +0x04
    CCharString   Name;                            // +0x08
    CFontManager* PManager;                        // +0x0c
    CFontBank() : FontHeight(0), PManager(0) { }
    __forceinline virtual ~CFontBank() { }
};

CFontBank* Create_CFontBank()
{
    return new CFontBank;
}