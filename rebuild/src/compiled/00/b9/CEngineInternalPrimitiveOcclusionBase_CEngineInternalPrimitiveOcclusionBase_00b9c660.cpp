// CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase() -- releases the two occlusion info counted pointers (OcclusionInfo[2] at +0x48, destroyed last to first), then the inlined base destructors leave CIVCountedPointeeBase's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9C660, length 55 bytes.
// Callees: CIVCountedPointeeBase virtual slot 1 (Delete, +4) at zero count. Vtable written last: 0x012354D0 = CIVCountedPointeeBase (rtti-vtables.tsv).
// Shape: the member-array destructor is the compiler's counted loop (end pointer `lea esi,[edi+50h]`, count 2 in ebx, `mov ecx,[esi-4] ; sub esi,4`) around
// the inlined CIVCountedPointer release; no vtable store of this class's own (an abstract class: `__declspec(novtable)`), then the empty inline
// CEngineInternalPrimitiveBase and CIVCountedPointeeBase destructors, whose stores collapse to the root's. The layout comes from
// engine/CEngineInternalPrimitiveOcclusionBase.h (OcclusionInfo +0x48) over the 0x48-byte CEngineInternalPrimitiveBase (engine header).
class CIVCountedPointeeBase { public:                     // vtable 0x012354D0
    virtual ~CIVCountedPointeeBase() { }           // slot 0 (deleting destructor)
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    ~CIVCountedPointer()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

class CEngineInternalPrimitiveBase : public CIVCountedPointeeBase { public:   // engine/CEngineInternalPrimitiveBase.h: 0x48 bytes
    unsigned char _pad08[0x40];                    // +0x08 .. +0x48
    ~CEngineInternalPrimitiveBase() { }
};

struct __declspec(novtable) CEngineInternalPrimitiveOcclusionBase : CEngineInternalPrimitiveBase {
    CIVCountedPointer OcclusionInfo[2];            // +0x48
    ~CEngineInternalPrimitiveOcclusionBase();
};

CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase()
{
}