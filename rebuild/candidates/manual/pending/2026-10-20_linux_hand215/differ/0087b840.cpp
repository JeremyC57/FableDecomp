// NCameraModes::CTrack::OnInitLeaf() -- when the target thing's shape-manager component (type 0x44) knows the mode's track, start at the track point nearest the target's camera point (type 0x3f component)
// Leaf: OnInitLeaf
// Grade: DIFFER(199v199) -- register swap only: retail keeps &m_target in edi and &PTrack in ebx, ours the reverse (pointer locals in either declaration order move other registers; `if (kind != 3) comp = fallback;` fixed the selection as in UpdateDesiredPos)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087B840, length 199 bytes (208 with padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0 (the inlined GetInterface for types 0x44 and 0x3f), the type-0x44 component's member @ 0x00761B50 (given
// &PTrack), the type-0x3f component's slot 28 (+0x70, kind), NShape::CTrack::GetTrackPosNearest @ 0x00760DA0.
// Layout: as UpdateDesiredPos 0x0087B910 (target +0xa4, PTrack +0xc0, CurrentFractionAlongTrack +0xc4).
// Shape: two landed out-parameter GetInterface lookups with uninitialised out pointers (the type-0x44 presence bit is a
// byte test at +0x28); the nearest position goes to a dead local, the fraction straight into the member.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
};
namespace NShape {
struct CTrack {
    void GetTrackPosNearest(const C3DVector* pos, C3DVector* nearest, float* fraction);   // 0x00760DA0
    C3DVector GetTrackPosAtLengthFraction(float fraction);                                // 0x00760C40
};
}

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetKind();                 // slot 28 (+0x70)
    CThing*       m_pThing;                 // +0x04
    unsigned char _pad08[0x48 - 0x08];
    C3DVector     m_position;               // +0x48 (descriptive: the camera-point position)
    bool IsShapeKnown_00761b50(NShape::CTrack** shapeSlot);   // 0x00761B50 (manifest "Map_FindByKey", descriptive)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

namespace NCameraModes {
struct CTrack {
    virtual void v00();
    unsigned char           _pad04[0xa0];
    CBaseIntelligentPointer m_target;       // +0xa4
    unsigned char           _padac[0x14];
    NShape::CTrack*         PTrack;                     // +0xc0
    float                   CurrentFractionAlongTrack;  // +0xc4
    bool                    ZTargeting;                 // +0xc8
    void Refresh_0084d080();                // 0x0084D080 (manifest "CVertexBufferWin32::DoSizeof" is propagated)
    void OnInitLeaf();
    void UpdateDesiredPos(C3DVector& desired);
};
}

void NCameraModes::CTrack::OnInitLeaf()
{
    CBaseIntelligentPointer* target = &m_target;
    CTCBase* pShapes;
    target->GetPItem()->GetInterface(&pShapes, 0x44);
    if (pShapes->IsShapeKnown_00761b50(&PTrack)) {
        CTCBase* pComponent;
        target->GetPItem()->GetInterface(&pComponent, 0x3f);
        CTCBase* pFallback;
        if (pComponent->GetKind() != 3)
            pComponent = pFallback;
        C3DVector nearest;
        PTrack->GetTrackPosNearest(&pComponent->m_position, &nearest, &CurrentFractionAlongTrack);
    }
}
