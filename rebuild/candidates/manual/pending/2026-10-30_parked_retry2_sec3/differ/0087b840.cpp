// NCameraModes::CTrack::OnInitLeaf() -- when the target thing's shape-manager component (type 0x44) knows the mode's track, start at the track point nearest the target's camera point (type 0x3f component)
// Leaf: OnInitLeaf
// Grade: DIFFER(199v199) -- register swap only (edi/ebx for &m_target and &PTrack); tried (this sweep): landed UpdateDesiredPos model (CIntelligentPointer<CThing>, CVectorMap), PDB callee CTCShapeManager::GetBestTrack(const NShape::CTrack**) const + const PTrack + reference GetTrackPosNearest, hoisted target local
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087B840, length 199 bytes (208 with padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0 (the inlined GetInterface for types 0x44 and 0x3f), the type-0x44 component's member @ 0x00761B50 (given
// &PTrack), the type-0x3f component's slot 28 (+0x70, kind), NShape::CTrack::GetTrackPosNearest @ 0x00760DA0.
// Layout: as UpdateDesiredPos 0x0087B910 (target +0xa4, PTrack +0xc0, CurrentFractionAlongTrack +0xc4).
// Shape: two landed out-parameter GetInterface lookups with uninitialised out pointers (the type-0x44 presence bit is a
// byte test at +0x28); the nearest position goes to a dead local, the fraction straight into the member.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
};
namespace NShape {
class CTrack { public:
    void GetTrackPosNearest(const C3DVector& pos, C3DVector& nearest, float& fraction) const;   // 0x00760DA0
    C3DVector GetTrackPosAtLengthFraction(float fraction) const;                                // 0x00760C40
};
}

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetKind();                 // slot 28 (+0x70)
    CThing*       m_pThing;                 // +0x04
    unsigned char _pad08[0x48 - 0x08];
    C3DVector     m_position;               // +0x48 (descriptive: the camera-point position)
};
class CTCShapeManager : public CTCBase { public:
    bool GetBestTrack(const NShape::CTrack** track) const;   // 0x00761B50
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace NCameraModes {
struct CTrack {
    virtual void v00();
    unsigned char           _pad04[0xa0];
    CIntelligentPointer<CThing> m_target;       // +0xa4
    unsigned char           _padac[0x14];
    const NShape::CTrack*   PTrack;                     // +0xc0
    float                   CurrentFractionAlongTrack;  // +0xc4
    bool                    ZTargeting;                 // +0xc8
    void Refresh_0084d080();                // 0x0084D080 (manifest "CVertexBufferWin32::DoSizeof" is propagated)
    void OnInitLeaf();
    void UpdateDesiredPos(C3DVector& desired);
};
}

void NCameraModes::CTrack::OnInitLeaf()
{
    CTCShapeManager* pShapes;
    m_target.GetPItem()->GetInterface(&pShapes, 0x44);
    if (pShapes->GetBestTrack(&PTrack)) {
        CTCBase* pComponent;
        m_target.GetPItem()->GetInterface(&pComponent, 0x3f);
        CTCBase* pFallback;
        if (pComponent->GetKind() != 3)
            pComponent = pFallback;
        C3DVector nearest;
        PTrack->GetTrackPosNearest(pComponent->m_position, nearest, CurrentFractionAlongTrack);
    }
}
