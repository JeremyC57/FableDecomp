// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#define FABLE_ENGINE_CTHINGSEARCHTOOLS_H
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS
// Retail opt-in (Codex pass 11 consolidation 2): the retail CThingSearchTools (reference members, STLport vector); see below.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CWorldMap;class CThingManager;class C2DBoxI;class CClass;
namespace NThingMap { class CThingMap;class CThingMapManager; }
// Original Ego_r TPI0x24622, native size18 after one vector reduction.
class CThingSearchTools {
protected:
    const std::list<CClass*>& PeekTypeList(long)const;
#if (defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) && (defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY))) || (!defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) && (defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY)))
    // Chunk 756 (row 0x0049C700): Ego_r.pdb ?ClipAndConvertWorldMapAreaToLocalArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@ABV2@@Z
    // (protected const, out of line in retail at 0x0049C700).
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map,const C2DBoxI& area)const;
#endif
    CWorldMap& WorldMap;
    NThingMap::CThingMapManager* PThingMapManager;
    CThingManager& ThingManager;
    std::vector<NThingMap::CThingMap*> AreaMaps;
public:
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF_DECL
    // Opt-in (wf11, row 0x009810B0): GetPBestLeafThing<Filter,Compare,T> declared only -- the caller calls the template COMDAT
    // (0x009823D0 for <CThingFilter_AICreatureHasProperty,CThingCompare_Nearest,CThingAICreature>).
    template<class Filter,class Compare,class T> bool GetPBestLeafThing(const Filter& filter,const Compare& compare,T** out) const;
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF
    // Both PDB instantiations, native0048cb50 (T village) /0048cbd0 (T const village).
    template<class Filter,class Compare,class T>
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    bool GetPBestLeafThing(const Filter& filter,const Compare& compare,T** out) const {
        T* best=0;
        const std::list<CClass*>& things=PeekTypeList(T::StatGetType());
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
            if(thing->IsIncludedInThingScans() && filter(*thing))
                best=compare(best,thing);
        }
        *out=best;
        return best!=0;
    }
#endif
    // GetPFirstLeafThing<Filter,T>: out of line in retail (Ego_r 0x005457E0 ??$GetPFirstLeafThing@V?$CFilter_Bind2@VCThingFilter_OverlappingSphere@@
    // VCThingFilter_PosInContainmentVolume@@@@VCThingBuilding@@...; 0x00666EF0 calls it). A same-TU noinline body (EXTRA_RULES chunk 422):
    // AutoPlaceNearBuilding 0x00666EF0 matches only with the body visible (the filter temporaries are then known uncaptured).
    template<class Filter,class T>
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    bool GetPFirstLeafThing(const Filter& filter,T** out) const {
        const std::list<CClass*>& things=PeekTypeList(T::StatGetType());
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
            if(thing->IsIncludedInThingScans() && filter(*thing)) {
                *out=thing;
                return true;
            }
        }
        return false;
    }
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
    // Chunk 756 (row 0x009104D0 GetSeat): the GetAllLeafThings<T,Container,Filter> body (landed 0x00910A90 shape: the scan test, the
    // filter on the thing, push_back), a same-TU noinline body like GetPFirstLeafThing above: the caller then knows the result
    // vector is not captured and keeps its start in a register across later calls.
    template<class T,class Container,class Filter> __declspec(noinline) void GetAllLeafThings(const Filter& filter,Container& result) const {
        const std::list<CClass*>& things=PeekTypeList(T::StatGetType());
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
            if(thing->IsIncludedInThingScans() && filter(*thing))
                result.push_back(thing);
        }
    }
#else
    template<class T,class Container,class Filter>
    void GetAllLeafThings(const Filter&,Container&)const;
#endif
    // Chunk 728 (row 0x0074B770): GetAllThings<T,Container,Filter>, out of line in retail (Ego_r.pdb
    // ??$GetAllThings@VCThingMarker@@V?$vector@PAVCThingMarker@@...@std@@VCThingFilter_HasDefGlobalIndex@@@CThingSearchTools@@QBEXAB...@AAV...@Z;
    // retail instance 0x0074D940). A declaration only: no layout change.
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
    // Chunk 768 (row 0x005EBFA0 CTCTavernGame::CameraUseCameraPoint): the GetAllThings<T,Container,Filter> body (retail instance
    // 0x005ED860 <CThing, vector<CThing*>, CThingFilter_HasScriptName>, landed: PeekClassList 0x0049C770, the scan test, the filter on
    // the thing, push_back; the CThing type-mask test folds), a same-TU noinline body like GetAllLeafThings above: the caller then knows
    // the result vector is not captured and frees its storage without a second null test.
    template<class T,class Container,class Filter> __declspec(noinline) void GetAllThings(const Filter& filter,Container& result) const {
        const std::list<CClass*>& things=PeekClassList();
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_TYPEMASK
            // Nested opt-in (wf19 repair, row 0x007D46D0 CTCTavernGameCoinGolf::FadeOutTableOccupants): the THING_TYPE mask test of the
            // landed instance 0x007D4CE0 GetAllThings<CThingCreatureBase,list<CThingCreatureBase*>,CThingFilter_IsHuman> (scan test,
            // then `and edx,0Fh / cmp dl,0Fh` at 0x007D4D1E, then the filter), as GetPBestThing below. Needs T::StatGetTypeFlags
            // (FABLE_RETAIL_CTHING_STATTYPEFLAGS / FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS). The default path is unchanged.
            unsigned long mask=T::StatGetTypeFlags();
            if(thing->IsIncludedInThingScans() && (thing->GetTypeFlags()&mask)==mask && filter(*thing))
#else
            if(thing->IsIncludedInThingScans() && filter(*thing))
#endif
                result.push_back(thing);
        }
    }
protected:
    const std::list<CClass*>& PeekClassList() const;    // 0x0049C770 ?PeekClassList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@XZ
public:
#else
    template<class T,class Container,class Filter>
    void GetAllThings(const Filter&,Container&)const;
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
    // Chunk 1010 (row 0x008930F0 CGameScriptInterface::GetThingWithScriptName): GetPFirstThing<Filter,T>, out of line in retail
    // (Ego_r.pdb / FableWin.pdb ??$GetPFirstThing@V?$CFilter_Bind2@VCThingFilter_HasScriptName@@VCThingFilter_InMap@@@@VCThingGameObject@@
    // @CThingSearchTools@@QBE_NABV?$CFilter_Bind2@...@@PAPAVCThingGameObject@@@Z = retail 0x008ABDF0). Declaration only.
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING_BODY
    // The GetPFirstThing body (landed 0x008ABDF0 shape: PeekClassList 0x0049C770, the THING_TYPE mask test with T::StatGetTypeFlags,
    // the scan test, the filter on the thing), a same-TU noinline body like GetAllThings below: the caller then knows the filter
    // temporaries and the out pointer are not captured. Needs FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY (PeekClassList) and
    // T::StatGetTypeFlags at the point of instantiation.
    template<class Filter,class T> __declspec(noinline) bool GetPFirstThing(const Filter& filter,T** out) const {
        const std::list<CClass*>& things=PeekClassList();
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
            unsigned long mask=T::StatGetTypeFlags();
            if((thing->GetTypeFlags()&mask)==mask && thing->IsIncludedInThingScans() && filter(*thing)) {
                *out=thing;
                return true;
            }
        }
        return false;
    }
#else
    template<class Filter,class T> bool GetPFirstThing(const Filter& filter,T** out) const;
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
    // Chunk 1012 (row 0x0049FFD0 CWorld::SetKickPos): GetPBestThing<Filter,Compare,T>, out of line in retail (Ego_r.pdb
    // ??$GetPBestThing@V?$CFilter_Bind2@V?$binary_compose@U?$logical_or@_N@std@@VCThingFilter_HasTC@@V3@@std@@VCThingFilter_In2DRange@@@@
    // VCThingCompare_Nearest@@VCThing@@@CThingSearchTools@@QBE_N...@Z = retail 0x004AC2C0, landed, in the CWorld TU). The landed
    // body's shape (PeekClassList 0x0049C770, the scan test, the THING_TYPE mask test with T::StatGetTypeFlags, the filter on the thing,
    // best = compare(best, thing), the result through the out pointer), a same-TU noinline body like GetPFirstThing above: the caller
    // then knows the filter / comparator temporaries and the out pointer are only read or written, not captured. Needs
    // FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY (PeekClassList) and T::StatGetTypeFlags at the point of instantiation.
    template<class Filter,class Compare,class T> __declspec(noinline) bool GetPBestThing(const Filter& filter,const Compare& compare,T** out) const {
        T* best=0;
        const std::list<CClass*>& things=PeekClassList();
        for(std::list<CClass*>::const_iterator it=things.begin();it!=things.end();++it) {
            T* thing=static_cast<T*>(*it);
            unsigned long mask=T::StatGetTypeFlags();
            if(thing->IsIncludedInThingScans() && (thing->GetTypeFlags()&mask)==mask && filter(*thing))
                best=compare(best,thing);
        }
        *out=best;
        return best!=0;
    }
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
    // Retail opt-in (Codex2026-11-10): both original PDBs and native00942390.
    template<class Filter,class Compare,class T> bool GetPBestThingInRoughArea(const C2DBoxI&,const Filter&,const Compare&,T**)const;
#endif
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
    // Original raw PDB static members; native00942390 uses13b89a8/ac/b0.
    static unsigned long NoLookups,NoThingsScanned,SearchID;
protected:
    void GetAllThingMapsInArea(const C2DBoxI&,std::vector<NThingMap::CThingMap*>&)const;
public:
#endif
    CThingSearchTools(CThingManager&,CWorldMap&);
    ~CThingSearchTools();
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
    template<class T,class Container,class Filter> void GetAllThingsInRoughArea(const C2DBoxI& area,const Filter& filter,Container& result) const {
        std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
        GetAllThingMapsInArea(area,maps);
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_THISEND
        // Q18 opt-in (rows 0x006FE7C0 / 0x008780D0, the std::list instances): the loop's end is read from the member
        //   (0x00878115 `cmp esi,[ebp+10h]` = this->AreaMaps._M_finish, with `this` and &AreaMaps both kept: ebp / ebx).
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=AreaMaps.end();++it) {
#else
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
#endif
            const NThingMap::CThingMap& map=**it;
            C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
            for(long layer=0;layer<NThingMap::MaxLayers;++layer) {
                long shift=NThingMap::LayerInfo[layer].ScaleShift;
                C2DBoxI cells;
                cells.TLX=local.TLX>>shift;
                cells.TLY=local.TLY>>shift;
                cells.BRX=((local.BRX-1)>>shift)+1;
                cells.BRY=((local.BRY-1)>>shift)+1;
                if(cells.BRX-cells.TLX==0 || cells.BRY-cells.TLY==0)
                    continue;
                for(long y=cells.TLY;y<cells.BRY;++y) {
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE)
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
#else
                    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
#endif
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {
                        for(NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
                            CTCMapwho* who=node->PMapwho;
                            if(who->GetSearchID()<SearchID) {
                                who->SetSearchID(SearchID);
                                CThing& thing=who->GetParentThing();
                                unsigned long mask=T::StatGetTypeFlags();
                                if((thing.GetTypeFlags()&mask)==mask && filter(static_cast<const T&>(thing)))
                                    result.push_back(static_cast<T*>(&thing));
                            }
                        }
                    }
                }
            }
        }
        maps.clear();
    }
#elif defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY)
    // Chunk 768 (row 0x00969E70 CAIStateGroup_SleepOnFloor::IsRunnable): the GetAllThingsInRoughArea<T,Container,Filter> body (the
    // landed instances 0x0075BC40 / 0x00969FB0 shape: the maps under the area 0x0049C6F0, the search stamp bump, per map the local
    // area 0x0049C700, per mapwho layer the area scaled down by the layer's shift, then every cell's node list: a component not yet
    // stamped this search is stamped and its thing, when of type T, goes through the filter into the result; AreaMaps is cleared),
    // a same-TU noinline body like GetAllLeafThings above (retail compiles 0x00969FB0 straight after its caller 0x00969E70): the
    // caller then knows the result vector is not captured and keeps its start in a register across the later calls.
    // Needs engine/NThingMap_CThingMap.h, FABLE_RETAIL_CTCMAPWHO + FABLE_RETAIL_CTCMAPWHO_SEARCHID (engine/CTCMapwho.h) and
    // FABLE_RETAIL_CTHING_STATTYPEFLAGS (T::StatGetTypeFlags) at the point of instantiation.
    template<class T,class Container,class Filter> __declspec(noinline) void GetAllThingsInRoughArea(const C2DBoxI& area,const Filter& filter,Container& result) const {
        std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
        GetAllThingMapsInArea(area,maps);
        ++SearchCount_013b89a8;
        SearchWork_013b89ac=0;
        ++SearchID_013b89b0;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
            const NThingMap::CThingMap& map=**it;
            C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
            for(long layer=0;layer<NThingMap::g_NoMapwhoLevels;++layer) {
                long shift=LayerInfo_0137dad8[layer].ScaleShift;
                C2DBoxI cells;
                cells.TLX=local.TLX>>shift;
                cells.TLY=local.TLY>>shift;
                cells.BRX=((local.BRX-1)>>shift)+1;
                cells.BRY=((local.BRY-1)>>shift)+1;
                if(cells.BRX-cells.TLX==0 || cells.BRY-cells.TLY==0)
                    continue;
                for(long y=cells.TLY;y<cells.BRY;++y) {
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
#else
                    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
#endif
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {
                        for(NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
                            CTCMapwho* who=node->PMapwho;
                            if(who->GetSearchID()<SearchID_013b89b0) {
                                who->SetSearchID(SearchID_013b89b0);
                                CThing& thing=who->GetParentThing();
                                unsigned long mask=T::StatGetTypeFlags();
                                if((thing.GetTypeFlags()&mask)==mask && filter(static_cast<const T&>(thing)))
                                    result.push_back(static_cast<T*>(&thing));
                            }
                        }
                    }
                }
            }
        }
        maps.clear();
    }
protected:
    void GetAllThingMapsInArea(const C2DBoxI& area,std::vector<NThingMap::CThingMap*>& maps) const;   // 0x0049C6F0 (Ego_r.pdb, protected const)
public:
#else
    template<class T,class Container,class Filter>
    void GetAllThingsInRoughArea(const C2DBoxI&,const Filter&,Container&) const;
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
    // Opt-in (wf20, row 0x00972990): ??$GetPFirstThingInRoughArea@VCThingFilter_HasDefGlobalIndex@@VCThing@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV...@PAPAV...@@Z
    // (retail COMDAT 0x007FCEF0), declaration only.
    template<class Filter,class T> bool GetPFirstThingInRoughArea(const C2DBoxI&,const Filter&,T**) const;
#endif
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_BODY) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
    // Retail opt-in (chunk Q23, row 0x0063F7C0, the CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation,
    // CThingFilter_OverlappingBoundingBox> / CThingFindNavigationLayerAt instance): the ForEachInRoughArea<T,Filter,Proc> body with the
    // PDB signature -- every Ego_r.pdb / FableWin.pdb instance (the Bind2 / Bind3 navigation-layer pair, the FrameUpdateThing and
    // CThingFindHeightZBelowAt ones) is QBE and takes the functor as `const Proc&` (ABV), unlike the declaration below. The scan is the
    // GetAllThingsInRoughArea body of FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA (needed: the search statics and
    // GetAllThingMapsInArea; and CLIPAREA, CTCMAPWHO_SEARCHID, T::StatGetTypeFlags at the point of instantiation), with the map loop's end
    // read from the member (0x0063F811 `cmp esi,[ebp+10h]`, as ROUGHAREA_THISEND) and the inline PeekCell (FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE),
    // calling proc on each filtered thing and returning whether any call returned true (retail: `mov byte ptr [esp+13h],1`). The empty
    // cell-box test compares the edges (`cmp edx,esi ; je`, 0x0063F88A): the difference form keeps the width live for the x loop.
    template<class T,class Filter,class Proc> bool ForEachInRoughArea(const C2DBoxI& area,const Filter& filter,const Proc& proc) const {
        bool result=false;
        std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
        GetAllThingMapsInArea(area,maps);
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=AreaMaps.end();++it) {
            const NThingMap::CThingMap& map=**it;
            C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
            for(long layer=0;layer<NThingMap::MaxLayers;++layer) {
                long shift=NThingMap::LayerInfo[layer].ScaleShift;
                C2DBoxI cells;
                cells.TLX=local.TLX>>shift;
                cells.TLY=local.TLY>>shift;
                cells.BRX=((local.BRX-1)>>shift)+1;
                cells.BRY=((local.BRY-1)>>shift)+1;
                if(cells.TLX==cells.BRX || cells.TLY==cells.BRY)
                    continue;
                for(long y=cells.TLY;y<cells.BRY;++y) {
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {
                        for(NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
                            CTCMapwho* who=node->PMapwho;
                            if(who->GetSearchID()<SearchID) {
                                who->SetSearchID(SearchID);
                                CThing& thing=who->GetParentThing();
                                unsigned long mask=T::StatGetTypeFlags();
                                if((thing.GetTypeFlags()&mask)==mask && filter(static_cast<const T&>(thing)) && proc(thing))
                                    result=true;
                            }
                        }
                    }
                }
            }
        }
        maps.clear();
        return result;
    }
#elif defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA)
    // Chunk 847 (row 0x0063E590 CWorldMap::GetNavigationLayerAt): Ego_r.pdb ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind2@
    // VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@
    // QBE_NABVC2DBoxI@@ABV?$CFilter_Bind2@...@@AAVCThingFindNavigationLayerAt@@@Z and the CFilter_Bind3<CThingFilter_IsNot, ...> twin
    // (out of line in retail: 0x0063F4B0 the Bind2, 0x0063F7C0 the Bind3 instantiation). Declaration only: no layout change.
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
    // Nested opt-in (agentFAM11, 2026-11-12, queue land1011dp): the declaration with the PDB signature -- every Ego_r.pdb / FableWin.pdb
    // ForEachInRoughArea instance is QBE and takes the functor as `const Proc&` (ABV), e.g. the Bind2 navigation-layer instance
    // 0x0063F4B0 and its Bind3 twin 0x0063F7C0 (whose landed body, FOREACHINROUGHAREA_BODY, already has `const Proc&`).
    template<class T,class Filter,class Proc> bool ForEachInRoughArea(const C2DBoxI& area,const Filter& filter,const Proc& proc) const;
#else
    template<class T,class Filter,class Proc> bool ForEachInRoughArea(const C2DBoxI& area,const Filter& filter,Proc& proc) const;
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA
    // Nested opt-in (chunk 853, row 0x0050A650 CWorldMap::GetMapNavigationAreaInit): Ego_r.pdb ??$GetAllTCsInRoughArea@VCTCCreatureGenerator@@
    // VCThingFilter_IsInQuestOrNullQuest@@V?$vector@PBVCTCCreatureGenerator@@...@std@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV..@AAV..@Z and
    // ??$GetAllTCDsInRoughArea@VCTCDNavigationSeed@@ / @VCTCDRegionExit@@ (template order T, Filter, Container), out of line in retail
    // (0x00518340, 0x005185F0 / 0x00518900). Declarations only: no layout change.
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY
    // Nested opt-in (chunk 864, row 0x0050A650 CWorldMap::GetMapNavigationAreaInit): the GetAllTCsInRoughArea / GetAllTCDsInRoughArea
    // bodies, retail's own algorithm as the landed instances show it (0x005185F0 / 0x00518900 GetAllTCDsInRoughArea<CTCDNavigationSeed /
    // CTCDRegionExit>, and 0x00518340 GetAllTCsInRoughArea<CTCCreatureGenerator>, all out of line in retail): the same rough-area cell
    // walk as GetAllThingsInRoughArea above (maps under the area 0x0049C6F0, the search stamp bump, the local area 0x0049C700, per
    // mapwho layer the area scaled by the layer's shift, every cell's node list, each component stamped once per search), then per
    // thing: the TC scan takes the component of type T (CThing::GetTC<T>: the interface bit 0x00518 4A5 `test [edi+24h],20000000h`,
    // then Components.Find) and pushes the component through the filter; the TCD scan tests `HasTCDriver()` (PTCDriver +0x5C), asks `GetTCDriver()`'s
    // driver's slot-28 virtual for T's driver type (`push 19h ; call [edx+70h]`) and pushes the thing through the filter. Same-TU
    // noinline bodies like GetAllLeafThings above (retail compiles them out of line next to the caller): the caller then knows its
    // result vectors are not captured, so c2 keeps each vector's _M_start web as one promoted local that the allocator splits and
    // the frees reload into eax as retail (with declarations only the vectors are address-exposed and the last free's reload
    // coalesces with the loop iterator: the 0x0050A650 residue). Needs engine/CTCMapwho.h (FABLE_RETAIL_CTCMAPWHO +
    // FABLE_RETAIL_CTCMAPWHO_SEARCHID), engine/NThingMap_CThingMap.h, FABLE_RETAIL_CTHING_TCDRIVER (engine/CThing.h: HasTCDriver / GetTCDriver),
    // FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE and the TCD classes' FABLE_RETAIL_CTCD_STATGETDRIVERTYPE at the point of instantiation.
    template<class T,class Filter,class Container> __declspec(noinline) void GetAllTCsInRoughArea(const C2DBoxI& area,const Filter& filter,Container& result) const {
        std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
        GetAllThingMapsInArea(area,maps);
        ++SearchCount_013b89a8;
        SearchWork_013b89ac=0;
        ++SearchID_013b89b0;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
            const NThingMap::CThingMap& map=**it;
            C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
            for(long layer=0;layer<NThingMap::g_NoMapwhoLevels;++layer) {
                long shift=LayerInfo_0137dad8[layer].ScaleShift;
                C2DBoxI cells;
                cells.TLX=local.TLX>>shift;
                cells.TLY=local.TLY>>shift;
                cells.BRX=((local.BRX-1)>>shift)+1;
                cells.BRY=((local.BRY-1)>>shift)+1;
                if(cells.BRX-cells.TLX==0 || cells.BRY-cells.TLY==0)
                    continue;
                for(long y=cells.TLY;y<cells.BRY;++y) {
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
#else
                    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
#endif
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {
                        for(NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
                            CTCMapwho* who=node->PMapwho;
                            if(who->GetSearchID()<SearchID_013b89b0) {
                                who->SetSearchID(SearchID_013b89b0);
                                CThing& thing=who->GetParentThing();
                                T* tc=0;
                                if(thing.GetTC(&tc) && filter(thing))
                                    result.push_back(tc);
                            }
                        }
                    }
                }
            }
        }
        maps.clear();
    }
    template<class T,class Filter,class Container> __declspec(noinline) void GetAllTCDsInRoughArea(const C2DBoxI& area,const Filter& filter,Container& result) const {
        std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
        GetAllThingMapsInArea(area,maps);
        ++SearchCount_013b89a8;
        SearchWork_013b89ac=0;
        ++SearchID_013b89b0;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
            const NThingMap::CThingMap& map=**it;
            C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
            for(long layer=0;layer<NThingMap::g_NoMapwhoLevels;++layer) {
                long shift=LayerInfo_0137dad8[layer].ScaleShift;
                C2DBoxI cells;
                cells.TLX=local.TLX>>shift;
                cells.TLY=local.TLY>>shift;
                cells.BRX=((local.BRX-1)>>shift)+1;
                cells.BRY=((local.BRY-1)>>shift)+1;
                if(cells.BRX-cells.TLX==0 || cells.BRY-cells.TLY==0)
                    continue;
                for(long y=cells.TLY;y<cells.BRY;++y) {
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
                    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
#else
                    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
#endif
                    for(long x=cells.TLX;x<cells.BRX;++x,++cell) {
                        for(NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext) {
                            CTCMapwho* who=node->PMapwho;
                            if(who->GetSearchID()<SearchID_013b89b0) {
                                who->SetSearchID(SearchID_013b89b0);
                                CThing& thing=who->GetParentThing();
                                if(thing.HasTCDriver() && thing.GetTCDriver().IsDriverType(T::StatGetDriverType()) && filter(thing))
                                    result.push_back(&thing);
                            }
                        }
                    }
                }
            }
        }
        maps.clear();
    }
#ifndef FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
protected:
    void GetAllThingMapsInArea(const C2DBoxI& area,std::vector<NThingMap::CThingMap*>& maps) const;   // 0x0049C6F0 (Ego_r.pdb, protected const)
public:
#endif
#else
    template<class T,class Filter,class Container> void GetAllTCsInRoughArea(const C2DBoxI&,const Filter&,Container&) const;
    template<class T,class Filter,class Container> void GetAllTCDsInRoughArea(const C2DBoxI&,const Filter&,Container&) const;
#endif
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingSearchTools)==0x18);
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
template<class Filter,class Compare,class T>
bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area,const Filter& filter,const Compare& compare,T** out)const{
 T* best=0;
 std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
 GetAllThingMapsInArea(area,maps);
 ++NoLookups;NoThingsScanned=0;++SearchID;
 for(std::vector<NThingMap::CThingMap*>::const_iterator it=maps.begin();it!=maps.end();++it){
  const NThingMap::CThingMap& map=**it;
  C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
  for(long layer=0;layer<NThingMap::MaxLayers;++layer){
   long shift=NThingMap::LayerInfo[layer].ScaleShift;
   C2DBoxI cells(local.TLX>>shift,local.TLY>>shift,((local.BRX-1)>>shift)+1,((local.BRY-1)>>shift)+1);
   if(cells.BRX-cells.TLX==0||cells.BRY-cells.TLY==0)continue;
   for(long y=cells.TLY;y<cells.BRY;++y){
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
#else
    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
#endif
    for(long x=cells.TLX;x<cells.BRX;++x,++cell){
     for(const NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext){
      CTCMapwho* who=node->PMapwho;
      if(who->GetSearchID()<SearchID){
       who->SetSearchID(SearchID);
       CThing& thing=who->GetParentThing();
       unsigned long mask=T::StatGetTypeFlags();
       if((thing.GetTypeFlags()&mask)==mask&&filter(static_cast<const T&>(thing)))best=compare(best,static_cast<T*>(&thing));
      }
     }
    }
   }
  }
 }
 maps.clear();*out=best;return best!=0;
}
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY
// Opt-in (wf20, row 0x00972990 CAIStateGroup_JackOfBladesStage1::OnActivate): the GetPFirstThingInRoughArea<Filter,T> body (retail
// instance 0x007FCEF0 <CThingFilter_HasDefGlobalIndex,CThing>, landed: the maps under the area 0x0049C6F0, the search stamp bump, per
// map the local area 0x0049C700, per mapwho layer the scaled cells, every cell's node list; the first stamped thing of type T that
// passes the filter goes through the out pointer and returns true with the scratch list left as is; else AreaMaps is cleared and
// false), a same-TU noinline template like GetPBestThingInRoughArea above: retail's caller then knows the out pointer's target and
// the filter do not escape (the shield local is re-read after the intelligent pointer's base ctor; the filter temporary shares the
// string temporaries' frame word). Needs FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY (the statics and protected helpers).
template<class Filter,class T>
__declspec(noinline) bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& area,const Filter& filter,T** out)const{
 std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
 GetAllThingMapsInArea(area,maps);
 ++NoLookups;NoThingsScanned=0;++SearchID;
 for(std::vector<NThingMap::CThingMap*>::const_iterator it=maps.begin();it!=maps.end();++it){
  const NThingMap::CThingMap& map=**it;
  C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
  for(long layer=0;layer<NThingMap::MaxLayers;++layer){
   long shift=NThingMap::LayerInfo[layer].ScaleShift;
   C2DBoxI cells(local.TLX>>shift,local.TLY>>shift,((local.BRX-1)>>shift)+1,((local.BRY-1)>>shift)+1);
   if(cells.BRX-cells.TLX==0||cells.BRY-cells.TLY==0)continue;
   for(long y=cells.TLY;y<cells.BRY;++y){
    const NThingMap::CThingMapCell* cell=&map.Cells[layer][y*map.MapwhoLevelInfo[layer].Extents.X+cells.TLX];
    for(long x=cells.TLX;x<cells.BRX;++x,++cell){
     for(const NThingMap::CMapwhoNode* node=cell->PFirstNode;node;node=node->PNext){
      CTCMapwho* who=node->PMapwho;
      if(who->GetSearchID()<SearchID){
       who->SetSearchID(SearchID);
       CThing& thing=who->GetParentThing();
       unsigned long mask=T::StatGetTypeFlags();
       if((thing.GetTypeFlags()&mask)==mask&&filter(static_cast<const T&>(thing))){*out=static_cast<T*>(&thing);return true;}
      }
     }
    }
   }
  }
 }
 maps.clear();return false;
}
#endif

#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingSearchTools = 0x1c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingSearchTools = 0x18.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CThingManager;
struct CThingMapManager;
class CWorldMap;

#pragma pack(push, 1)
class CThingSearchTools {
public:
    CWorldMap*        WorldMap;                              // +0x00
    CThingMapManager* PThingMapManager;                      // +0x04
    CThingManager*    ThingManager;                          // +0x08
    unsigned char     AreaMaps[0xc];                         // +0x0c vector<NThingMap::CThingMap_*,std::allocator<NThingMap::CThingMap_*>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingSearchTools) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, WorldMap) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, PThingMapManager) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, ThingManager) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CThingSearchTools, AreaMaps) == 0xc);

#endif // FABLE_RETAIL_CTHINGSEARCHTOOLS
#endif // FABLE_ENGINE_CTHINGSEARCHTOOLS_H
