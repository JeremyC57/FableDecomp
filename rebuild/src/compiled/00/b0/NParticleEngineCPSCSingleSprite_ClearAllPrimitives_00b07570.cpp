// NParticleEngine::CPSCSingleSprite::ClearAllPrimitives(CEngineInterface*, const CPrimitiveGroupHandle&) -- when the group handle is live, removes every sprite primitive id in PrimIDList from it through the engine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B07570, length 93 bytes.
// Callees: the engine interface's virtual slot 30 (+0x78), called as (group handle, primitive id).
// Identity: slot 29 of NParticleEngine::CPSCSingleSprite's vtable 0x0129FB84 (rtti-vtables.tsv); the CPSCBase
// default for slot 29 (0x00AF2AC0) is an empty `ret 8`, and the CPSCRenderSprite override (0x00AFADC0)
// uses the same first argument as an engine object (its slot 31 releases a handle).
// Layout: Ego_r.pdb CPSCSingleSprite PrimIDList (CArray<long>) at +0x138; the retail object is 0xc
// bytes smaller before it (12-byte CArray in the spline/trail members), so the array is at +0x12c.
// The handle's second word is the referenced group (tested for null before anything else).
// Shape: /O2 loop (aligned head `lea ebx,[ebx]`), the engine argument loaded into ebx only after the
// entry test, the element count re-read every iteration (the virtual call may change the array),
// unsigned index compare (`jb`).
struct CPrimitiveGroupHandle {              // intelligent handle: {link, group}
    void* m_pLink;                          // +0x00
    void* m_pGroup;                         // +0x04
};

struct CEngineInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void RemovePrimitive(const CPrimitiveGroupHandle& group, long primitiveId);   // slot 30 (+0x78)
};

template<class T> struct CArray {           // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
    T& operator[](unsigned long index) { return m_pData[index]; }
};

struct CBase {
    virtual ~CBase();
};

namespace NParticleEngine {
struct CPSCSingleSprite : CBase {           // vtable 0x0129FB84
    unsigned char _body[0x128];             // +0x04 .. +0x12c (CPSCBase part, sprite settings, trail primitive)
    CArray<long>  PrimIDList;               // +0x12c
    void ClearAllPrimitives(CEngineInterface* engine, const CPrimitiveGroupHandle& group);
};

void CPSCSingleSprite::ClearAllPrimitives(CEngineInterface* engine, const CPrimitiveGroupHandle& group)
{
    if (group.m_pGroup) {
        for (unsigned long i = 0; i < PrimIDList.Size(); ++i)
            engine->RemovePrimitive(group, PrimIDList[i]);
    }
}
}