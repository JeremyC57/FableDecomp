// CWorldMap::SetRegionsAsConnectedInGraph(const CCharString&, const CCharString&, bool) -- connect or disconnect two named regions in the region graph
// Leaf: ?SetRegionsAsConnectedInGraph@CWorldMap@@QAEXABVCCharString@@0_N@Z
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x004FC970, 77 bytes, `ret 0Ch`. PDB ?SetRegionsAsConnectedInGraph@CWorldMap@@QAEXABVCCharString@@0_N@Z
// Callees: CWorldMap::GetRegionNumberFromName(const CCharString&) const 0x004FC210 (x2),
// CRegionConnectionGraph::SetRegionsAsConnected(long, long, bool) 0x008286A0 (manifest label
// "Navigation_ConnectRegionsInGraph" is a guess; PDB ?SetRegionsAsConnected@CRegionConnectionGraph@@QAEXJJ_N@Z),
// CPlayerGui::UpdateMiniMapMarkers(bool) 0x004378C0 (forwards to the CDrawMiniMap at +0x160, whose 0x00643E9A forwards
// to UpdateMarkers; landed sources call it "Refresh(long)", the manifest "_Cons_val<...>") on (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790.
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x44 PRegionGraph (CCountedPointer<CRegionConnectionGraph>
// Data, PDB +0x54).
// Shape: the graph pointer is fetched into ebx before the lookups (a named local), both region numbers must be
// positive, the bool is forwarded as a dword push.
class CCharString;
class CRegionConnectionGraph {
public:
    void SetRegionsAsConnected(long region1, long region2, bool connected);    // 0x008286A0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui {
public:
    void UpdateMiniMapMarkers(bool force);      // 0x004378C0
};

class CWorldMap {
public:
    virtual void v00();
    unsigned char           _pad04[0x40];
    CRegionConnectionGraph* PRegionGraph;       // +0x44 CCountedPointer<CRegionConnectionGraph>::Data (PDB +0x54)
    void*                   PRegionGraphInfo;   // +0x48 CCountedPointer<CRegionConnectionGraph>::Info
    long GetRegionNumberFromName(const CCharString& name) const;  // 0x004FC210
    void SetRegionsAsConnectedInGraph(const CCharString& region1, const CCharString& region2, bool connected);
};

void CWorldMap::SetRegionsAsConnectedInGraph(const CCharString& region1, const CCharString& region2, bool connected)
{
    CRegionConnectionGraph* graph = PRegionGraph;
    if (graph)
    {
        long number1 = GetRegionNumberFromName(region1);
        long number2 = GetRegionNumberFromName(region2);
        if (number1 > 0 && number2 > 0)
        {
            graph->SetRegionsAsConnected(number1, number2, connected);
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
        }
    }
}