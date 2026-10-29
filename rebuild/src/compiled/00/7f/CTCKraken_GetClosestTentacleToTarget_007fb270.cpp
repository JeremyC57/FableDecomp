// CTCKraken::GetClosestTentacleToTarget(list<CIntelligentPointer<CThing> >::iterator& closest) -- picks the live tentacle whose facing (kraken heading plus the tentacle's angle offset) is nearest the bearing from the kraken to its target, within one tentacle's share of a turn; true when one was found and its scripted-hook component is not in mode 0x54, otherwise the iterator is set to the end
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FB270, 554 bytes, `ret 4`. Speed TU (no pragmas, no frame pointer).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, the physics TC slot 63 (+0xFC)
//   GetFacingAngleXY, _CIfmod 0x00BFEB28, AngleDifference 0x00A441E0 (`__stdcall`), LowerBound 0x0040F020,
//   HasMode 0x0073A8A0 on interface 0x31.
// Layout: local view -- CTCKraken: ParentThing +0x04, kraken body pointer +0x18, target pointer +0x20, tentacle list
//   +0x44 (one-pointer STLport list, CIntelligentPointer at node +8); CThing flags byte +0x91, physics TC +0x60;
//   the tentacle component (0xE3) angle offset +0x44.
#include <math.h>
#pragma intrinsic(fmod, fabs, atan2)
float __stdcall AngleDifference(float a, float b);   // 0x00A441E0

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
};
class CTCBase;
struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
// same-TU stand-in for the interface-map LowerBound (VC7.1 then knows the entry is not derived from the key)
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
struct CPhysicsTC {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();                 // slot 63 (+0xFC)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                     // +0x20 bitset of interface types
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsTC*   PhysicsTC;                          // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                            // +0x91
    const C3DVector& GetPos() const;             // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (HasTC(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* FindTC(long kind)
    {
        long key = kind;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || kind < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void*  __vftable;
    CThing* m_pItem;
    CBaseObject* GetPItem() const;                         // 0x00A01B50
};
class CTCKrakenTentacle { public:
    unsigned char _pad00[0x44];
    float AngleOffset;                                // +0x44
};
class CTCCreatureModeManager { public:
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};
struct STentacleNode {
    STentacleNode* _M_next;
    STentacleNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CTentacleIterator { STentacleNode* _M_node; };
struct CTentacleList {
    STentacleNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (STentacleNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
class CTCKraken { public:
    void*                   __vftable;                // +0x00
    CThing*                 ParentThing;              // +0x04
    unsigned char           _pad08[0x18 - 0x08];
    CBaseIntelligentPointer Body;                     // +0x18
    CBaseIntelligentPointer Target;                   // +0x20
    unsigned char           _pad28[0x44 - 0x28];
    CTentacleList           Tentacles;                // +0x44
    bool GetClosestTentacleToTarget(CTentacleIterator& closest);
};
bool CTCKraken::GetClosestTentacleToTarget(CTentacleIterator& closest)
{
    if (((CThing*)Target.GetPItem()) && !(((CThing*)Target.GetPItem())->Flags91 & 1) && Tentacles.size() > 0) {
        C3DVector targetPos(((CThing*)Target.GetPItem())->GetPos());
        C3DVector bodyPos(((CThing*)Body.GetPItem())->GetPos());
        float heading = ParentThing->PhysicsTC->GetFacingAngleXY();
        float bearing = (float)atan2(targetPos.X - bodyPos.X, targetPos.Y - bodyPos.Y) * 0.15915493667125702f;
        if (bearing < 0.0f || bearing >= 1.0f) {
            bearing = (float)fmod((double)bearing, 1.0);
            if (bearing < 0.0f)
                bearing += 1.0f;
        }
        bool found = false;
        float best = 1.0f / Tentacles.size();
        for (STentacleNode* node = Tentacles._M_node->_M_next; node != Tentacles._M_node; node = node->_M_next) {
            CThing* thing = ((CThing*)node->_M_data.GetPItem());
            CTCKrakenTentacle* tentacle;
            thing->GetInterface(&tentacle, 0xe3);
            {
                float facing = heading + tentacle->AngleOffset;
                float difference = (float)fabs(AngleDifference(facing, bearing));
                if (difference < best) {
                    best = difference;
                    closest._M_node = node;
                    found = true;
                }
            }
        }
        if (found) {
            CThing* thing = ((CThing*)closest._M_node->_M_data.GetPItem());
            if (thing->HasTC(0x31) && !thing->FindTC<CTCCreatureModeManager>(0x31)->IsModeActive(0x54))
                return true;
        }
    }
    closest._M_node = Tentacles._M_node;
    return false;
}