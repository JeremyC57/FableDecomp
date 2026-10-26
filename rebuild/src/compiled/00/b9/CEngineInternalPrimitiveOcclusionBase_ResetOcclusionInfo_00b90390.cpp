// CEngineInternalPrimitiveOcclusionBase::ResetOcclusionInfo(EEngineOcclusionLayer layer) -- drops the occlusion info counted pointer of `layer` (OcclusionInfo[layer] at +0x48, two layers); a layer of -1 is ignored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B90390, length 52 bytes (64 with padding).
// Callees: CIVCountedPointeeBase virtual slot 1 (Delete, +4) when the released count reaches zero. Shape: the inlined `CIVCountedPointer::operator=(0)`:
// the release through a local copy of the pointee (`dec [ecx+4] ; jne ; call [eax+4]`) with its own zero store inside the null test, then the unconditional
// zero store of the assignment (the add-reference of a null pointee folds away). The element address is `[esi+edi*4+48h]`.
// engine/CEngineInternalPrimitiveOcclusionBase.h (PDB) has OcclusionInfo (CIVCountedPointer<CEnginePrimitiveOcclusionInfo>[2]) at +0x48.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(CIVCountedPointeeBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
        return *this;
    }
};

class CEngineInternalPrimitiveOcclusionBase { public:
    unsigned char     _pad[0x48];
    CIVCountedPointer OcclusionInfo[2];            // +0x48
    void ResetOcclusionInfo(long layer);
};

void CEngineInternalPrimitiveOcclusionBase::ResetOcclusionInfo(long layer)
{
    if (layer != -1)
        OcclusionInfo[layer] = 0;
}