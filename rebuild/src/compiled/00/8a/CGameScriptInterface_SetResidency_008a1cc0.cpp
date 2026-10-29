// CGameScriptInterface::SetResidency(const CScriptThing& building, bool resident) -- mark a valid building as available (or not) for residency and set its automatic locking; when it is withdrawn, evict every villager living in it (re-scanning the village list after each kill) and return the script thing of the last evictee's other building, else the null script thing
// Leaf: ?SetResidency@CGameScriptInterface@@UAE?AVCScriptThing@@ABV2@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A1CC0, length 390 bytes (`ret 0Ch`, hidden CScriptThing result).
// Callees: the script thing's slots 75 (+0x12C IsValid) and 11 (+0x2C GetThing), own virtual slot 431
//   OverrideAutomaticHouseLocking(const CScriptThing&, bool) (0x0088E030), CThingBuilding::SetIsAvailableForResidency(bool)
//   @ 0x0082E090, the component-map LowerBound @ 0x0040F020, the TC 0x23 village accessor @ 0x007EE540 (manifest
//   "IS_THING_ALIVE<...>" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80,
//   CThing::GetScriptThing @ 0x004C7E50 (into the hidden result), CGameScriptThing::CreateNullThing() @ 0x008CFF40.
// Layout: CThing flags +0x6C (bit 6 = building), UsingComponents +0x20, component map +0x44, dead flags +0x91 bit 0,
//   home building pointer +0x168 and second residence pointer +0x170 (CIntelligentPointer, 8 bytes); the village's
//   resident list (STLport list of CIntelligentPointer<CThing>) at +0x2C.
// Shape: the member pointers are reached through inline accessors (`lea ecx,[eax+N]`); a null or dead resident `continue`s without advancing (as retail: the iterator is only advanced on the
//   non-matching path, and reset to begin() after each kill).
class CThing_Retail;
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
struct SResidentNode {
    SResidentNode*          _M_next;
    SResidentNode*          _M_prev;
    CIntelligentPointer<CThing_Retail> _M_data;                    // +0x08
};
struct CResidentList {
    SResidentNode* _M_node;
};
struct CVillage_Retail {
    unsigned char _pad00[0x2c];
    CResidentList Residents;                            // +0x2C
};
struct CTCVillageMember_Retail { CVillage_Retail* GetVillage(); };   // 0x007EE540
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
class CScriptThing;
class CThing_Retail { public:
    unsigned char           _pad00[0x20];
    unsigned long           m_presentBits[9];       // +0x20
    CInterfaceMap_Retail    m_interfaces;           // +0x44
    unsigned char           _pad4c[0x20];
    unsigned char           m_flags;                // +0x6C (bit 6 = building)
    unsigned char           _pad6d[0x24];
    unsigned char           m_dead : 1;             // +0x91 bit 0
    unsigned char           m_91_1 : 7;
    unsigned char           _pad92[0xd6];
    CIntelligentPointer<CThing_Retail> HomeBuilding;           // +0x168
    CIntelligentPointer<CThing_Retail> OtherResidence;         // +0x170
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return ((CTCBase_Retail*)it->second);
    }
    CIntelligentPointer<CThing_Retail>& GetHomeBuilding() { return HomeBuilding; }
    CIntelligentPointer<CThing_Retail>& GetOtherResidence() { return OtherResidence; }
    void SetIsAvailableForResidency(bool available);    // 0x0082E090
    void Kill(bool flag);                               // 0x004C9B80
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
    void* PImp_Data;
    void* PImp_Info;
};
class CGameScriptThing : public CScriptThing {      // Ego_r.pdb: CGameScriptThing derives from CScriptThing
public:
    static CScriptThing __fastcall CreateNullThing(); // 0x008CFF40 (?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ)
};

#define VPAD1(a) virtual void a();
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CGameScriptInterface { public:
    VPAD256(pa) VPAD128(pb) VPAD32(pc) VPAD8(pd) VPAD4(pe) VPAD2(pf) VPAD1(pg)   // slots 0..430
    virtual void OverrideAutomaticHouseLocking(const CScriptThing& building, bool override) const;   // slot 431 (+0x6BC)
    virtual CScriptThing SetResidency(const CScriptThing& building, bool resident);
};

CScriptThing CGameScriptInterface::SetResidency(const CScriptThing& building, bool resident)
{
    CThing_Retail* evictedTo = 0;
    if (building.IsValid()) {
        CThing_Retail* house = building.GetThing();
        if (house->m_flags & 0x40) {
            OverrideAutomaticHouseLocking(building, !resident);
            house->SetIsAvailableForResidency(resident);
            if (!resident && house->HasInterface(0x23)) {
                CResidentList& residents = ((CTCVillageMember_Retail*)house->GetInterface(0x23))->GetVillage()->Residents;
                SResidentNode* it = residents._M_node->_M_next;
                while (it != residents._M_node) {
                    CIntelligentPointer<CThing_Retail>& resident = it->_M_data;
                    if (!resident.GetPItem() || resident.GetPItem()->m_dead)
                        continue;
                    if (resident.GetPItem()->GetHomeBuilding().GetPItem() == house) {
                        if (resident.GetPItem()->GetOtherResidence().GetPItem()) {
                            resident.GetPItem()->GetOtherResidence().GetPItem()->SetIsAvailableForResidency(false);
                            evictedTo = resident.GetPItem()->GetOtherResidence().GetPItem();
                        }
                        resident.GetPItem()->Kill(true);
                        it = residents._M_node->_M_next;
                    } else
                        it = it->_M_next;
                }
            }
        }
    }
    if (evictedTo)
        return evictedTo->GetScriptThing();
    return CGameScriptThing::CreateNullThing();
}