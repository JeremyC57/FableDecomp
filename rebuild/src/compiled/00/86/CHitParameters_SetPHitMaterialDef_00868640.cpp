// CHitParameters::SetPHitMaterialDef(CIVCountedPointer def) -- assign the counted material-def pointer at +0x4c from the by-value argument (release the old pointee when different, take the new one and count it), then the argument's own release at the end of the call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868640, length 80 bytes.
// Shape: the argument in esi, `this` in edi, the compare-first assignment, the parameter destroyed last, `ret 4`.
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
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
    ~CIVCountedPointer() { Release(); }
};

class CHitParameters { public:
    unsigned char     _pad[0x4c];
    CIVCountedPointer m_hitMaterialDef;            // +0x4c
    void SetPHitMaterialDef(CIVCountedPointer def);
};

void CHitParameters::SetPHitMaterialDef(CIVCountedPointer def)
{
    m_hitMaterialDef = def;
}