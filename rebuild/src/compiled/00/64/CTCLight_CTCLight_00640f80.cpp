// CTCLight::~CTCLight() -- unregister the light from the owner's graphic appearance (unless the owner is dead), free the two primitive vectors, release the light def and the shared light info, then ~CTCBase
// Leaf: ??1CTCLight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640F80, length 160 bytes (tail `jmp` into ~CTCBase).
// Label correction: the catalog row says "~CTCGraphicAppearanceMultipleStaticMeshes", but the two vtable stores are CTCLight's
// (primary 0x0125A194, appearance-component side 0x0125A208 at +0xc; rtti-vtables.tsv), and the secondary base's own vtable
// 0x0125A170 (NGraphicAppearance::CAGraphicAppearanceComponent) is restored before ~CTCBase.
// Callees: NGraphicAppearance::CTCGraphicAppearance::UnregisterComponent @ 0x004C0000 (takes the +0xc subobject), free @ 0x00BFEA14
// (STLport malloc-allocator vector storage), operator delete @ 0x00BFE9BC (the counted pointer's info block), CTCBase::~CTCBase
// @ 0x00686830.
// Layout: PDB CTCLight (0x4c) with retail 12-byte STLport vectors (0x44; see the landed OnAppearanceClear 0x00641210):
// PLightInfo +0x10 (CCountedPointer: Data, Info), PLightDef +0x18 (CDefPointer), Active/Overridden +0x1c/+0x1d, the two flicker
// floats +0x20/+0x24, Initialised +0x28, FlareSprites +0x2c, Lights +0x38; the owner's dead bit is bit 0 of CThing +0x91 and its
// CTCGraphicAppearance pointer CThing +0x64.
// Shape: members destroyed in reverse order: the vectors free `_M_start` when non-null, the CDefPointer releases through slot 1
// and clears itself inside its null test, the CCountedPointer re-reads Info after the decrement, calls the block's __fastcall
// DeleteFunc on Data, deletes the block and then zeroes Data and Info.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CThing;
namespace NGraphicAppearance {
class CPrimitive;
class CAGraphicAppearanceComponent;
class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent* component);   // 0x004C0000
};
class CAGraphicAppearanceComponent { public:                       // vtable 0x0125A170
    virtual void OnAppearanceDraw();
    ~CAGraphicAppearanceComponent() { }
};
}

class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
    unsigned char _pad68[0x29];
    unsigned char Dead : 1;                                 // +0x91 bit 0
};

class CTCBase { public:
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};

class CCPPointerInfo { public:                                     // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
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
class CDefPointeeBase { public:
    virtual void Destroy();
    virtual void Delete();                                  // slot 1
    long m_refCount;                                        // +0x04
};
template<class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};
template<class T> struct CPtrVector {                       // STLport vector<T*> (12 bytes, malloc allocator)
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    ~CPtrVector() { if (_M_start) free(_M_start); }
};

struct CLightInfo;
class CLightDef;

class CTCLight : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent { public:   // vtables 0x0125A194 / 0x0125A208
    CCountedPointer<CLightInfo>     PLightInfo;             // +0x10
    CDefPointer<const CLightDef>    PLightDef;              // +0x18
    bool  Active;                                           // +0x1c
    bool  Overridden;                                       // +0x1d
    float AccumilativeFlickerFactor;                        // +0x20
    float OldAccumilativeFlickerFactor;                     // +0x24
    bool  Initialised;                                      // +0x28
    CPtrVector<NGraphicAppearance::CPrimitive> FlareSprites;   // +0x2c
    CPtrVector<NGraphicAppearance::CPrimitive> Lights;         // +0x38
    virtual ~CTCLight();
    virtual void OnAppearanceDraw();
};

CTCLight::~CTCLight()
{
    if (!ParentThing->Dead)
        ParentThing->PTCGraphicAppearance->UnregisterComponent(this);
}