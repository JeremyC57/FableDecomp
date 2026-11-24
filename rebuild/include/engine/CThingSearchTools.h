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
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG
            // NF127 nested opt-in (2026-11-20, row 0x00910A90 GetAllLeafThings<CThingObject,vector,CIsSeatWithSpaceOwnedBy>, the out-of-line
            // instance): the thing held as CThing* and cast to T* at the filter and as the push_back argument (the landed row's
            // ThingCast temporary), so the argument is homed just before the push_back (`mov [esp+18h],esi` at 0x00910AE2) and the
            // result vector stays in edi. Same behaviour.
            CThing* thing=static_cast<CThing*>(*it);
            if(thing->IsIncludedInThingScans() && filter(*static_cast<T*>(thing)))
                result.push_back(static_cast<T*>(thing));
#else
            T* thing=static_cast<T*>(*it);
            if(thing->IsIncludedInThingScans() && filter(*thing))
                result.push_back(thing);
#endif
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
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
    // Nested opt-in (hand4b w24, row 0x00834530 CombatStressTestDebug and its same-TU instance 0x0052B270
    // GetAllThings<CThingAICreature, vector<CThingAICreature*>, CThingFilter_None>): the original text. Ego_d.pdb's record of that
    // instance (debug 277): filter_func, things; flags, class_list; { it; { temp_thing; { tthing; { jonathans_lovely_safe_variable } } } },
    // and FableWin.pdb's /Od body (0x01D55090) for the statements: flags = T::StatGetTypeFlags() first, then class_list = PeekClassList();
    // temp_thing = *it for the scan and type-flag tests; tthing = static_cast<T*>(*it) (FableWin dereferences the iterator again) for the
    // filter; the pushed copy in its own local. Retail then stores the pushed pointer to its frame word only on the push path (0x0052B2BE).
    // Default path unchanged.
    template<class T,class Container,class Filter> __declspec(noinline) void GetAllThings(const Filter& filter_func,Container& things) const {
        unsigned long flags=T::StatGetTypeFlags();
        const std::list<CClass*>& class_list=PeekClassList();
        for(std::list<CClass*>::const_iterator it=class_list.begin();it!=class_list.end();++it) {
            CThing* temp_thing=static_cast<CThing*>(*it);
            if(temp_thing->IsIncludedInThingScans() && (temp_thing->GetTypeFlags()&flags)==flags) {
                T* tthing=static_cast<T*>(*it);
                if(filter_func(*tthing)) {
                    T* jonathans_lovely_safe_variable=tthing;
                    things.push_back(jonathans_lovely_safe_variable);
                }
            }
        }
    }
#else
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
#endif
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
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_DECL_PASS26) && !defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY)
    template<class Filter,class Compare,class T> bool GetPBestThing(const Filter&,const Compare&,T**)const;
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
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
            // NF115 nested opt-in (PRIORITY q4495, row 0x0065F744 GetPBestThing<CThingFilter_HasTC,CThingCompare_Nearest,CThingAICreature>):
            // retail tests the re-read element for null after the scan and type tests and before the filter (0x0065F776 `test edi,edi ; je`;
            // the CThing instance 0x004AC2C0 has the same `test esi,esi ; je` at 0x004AC304). FableWin.pdb's local record of another
            // GetPBestThing instance (binary_compose filter, CThing** out) lists pbest_thing, flags, class_list, { it; { temp_thing; { tthing } } }
            // (the ALLTHINGSBODY_EGOD shape): the element is read again as tthing for the filter. Inferred: the null test on tthing.
            if(thing->IsIncludedInThingScans() && (thing->GetTypeFlags()&mask)==mask) {
                T* tthing=static_cast<T*>(*it);
                if(tthing && filter(*tthing))
                    best=compare(best,tthing);
            }
#else
            if(thing->IsIncludedInThingScans() && (thing->GetTypeFlags()&mask)==mask && filter(*thing))
                best=compare(best,thing);
#endif
        }
        *out=best;
        return best!=0;
    }
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
    // Retail opt-in (Codex2026-11-10): both original PDBs and native00942390.
    template<class Filter,class Compare,class T> bool GetPBestThingInRoughArea(const C2DBoxI&,const Filter&,const Compare&,T**)const;
#endif
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_COUNTERS_PASS26) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
    // Original raw PDB static members; native00942390 uses13b89a8/ac/b0.
    static unsigned long NoLookups,NoThingsScanned,SearchID;
protected:
    void GetAllThingMapsInArea(const C2DBoxI&,std::vector<NThingMap::CThingMap*>&)const;
public:
#elif defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY)
    // NF103 (round 10, header claim): the ROUGHAREABODY / TCSINROUGHAREA_BODY copies below use the same PDB statics (they spelled
    // SearchCount_013b89a8 / SearchWork_013b89ac / SearchID_013b89b0); GetAllThingMapsInArea is declared by those blocks.
    static unsigned long NoLookups,NoThingsScanned,SearchID;
#endif
    CThingSearchTools(CThingManager&,CWorldMap&);
#ifndef FABLE_RETAIL_CTHINGSEARCHTOOLS_IMPLICIT_DTOR
    ~CThingSearchTools();
#else
    // Opt-in (chunk 2051, row 0x004A6E30 CWorld::Init): no user destructor -- retail expands the implicit one in place (0x004A72D7: the vector at +0xC freed,
    // then operator delete).
#endif
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
    // egod opt-in (e08, row 0x00878430, the <CThing, std::list<const CThing*>, CFilter_Bind2<CThingFilter_IsOfType,
    // CThingFilter_OverlappingSphere> > instance): the GetAllThingsInRoughArea<T,Container,Filter> body with Ego_d.pdb's own parameter
    // names, locals and scopes (cm_helper_functions.obj record: filter_func, things, the function-scope `flags`, it (a const_iterator),
    // thing_map, local_area, c1, clipped_area, cy, pcell, cx, ploop_node, pmapwho, loop_thing, tthing; no named map list), the empty
    // test through C2DBoxI::GetWidth / GetHeight (needs FABLE_RETAIL_C2DBOXI_GETWIDTH + FABLE_RETAIL_C2DBOXI_GETHEIGHT) and the
    // protected inline GetMapwhoArea (Ego_r.pdb / FableWin.pdb ?GetMapwhoArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@JABV2@@Z,
    // expanded in retail) with Ego_d.pdb's locals layer_info / clipped_area (Ego_d's third local, `const C2DExtentsI& extents`, is
    // used only by the debug build's checks: FableWin's /Od copy is 540 bytes, not recovered). The named result is returned through
    // C2DBoxI's user copy constructor (Ego_d method list 0xAE3E, FableWin ??0C2DBoxI@@QAE@ABV0@@Z 24i/62b = FABLE_RETAIL_C2DBOXI_COPY,
    // which needs FABLE_RETAIL_C2DBOXI): the same text reproduces the out-of-line copy 0x00436500 (RELOCATION_MATCH) and the row's
    // expansion. FITTED only: field stores rather than the four-coordinate constructor (both grade the same at both addresses).
    // The search statics, GetAllThingMapsInArea and the inline PeekCell (FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE) are
    // those of FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA, which must be on too.
protected:
    C2DBoxI GetMapwhoArea(const NThingMap::CThingMap& map,long layer,const C2DBoxI& area) const {
        const NThingMap::CLayerInfo& layer_info=NThingMap::LayerInfo[layer];
        C2DBoxI clipped_area;
        clipped_area.TLX=area.TLX>>layer_info.ScaleShift;
        clipped_area.TLY=area.TLY>>layer_info.ScaleShift;
        clipped_area.BRX=((area.BRX-1)>>layer_info.ScaleShift)+1;
        clipped_area.BRY=((area.BRY-1)>>layer_info.ScaleShift)+1;
        return clipped_area;
    }
public:
    template<class T,class Container,class Filter>
#if defined(_MSC_VER) && (defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_NOINLINE_PASS27) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_SEARCH_BUDGET_PASS27))
    // Inline-budget: both TUs call the original rough-area scan.
    __declspec(noinline)
#endif
    void GetAllThingsInRoughArea(const C2DBoxI& area,const Filter& filter_func,Container& things) const {
        unsigned long flags=T::StatGetTypeFlags();
        GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
        for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
            NThingMap::CThingMap& thing_map=**it;
            C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
            for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
                C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
                if(clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0)
                    continue;
                for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                    const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                    for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                        for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                            CTCMapwho* pmapwho=ploop_node->PMapwho;
                            if(pmapwho->GetSearchID()<SearchID) {
                                pmapwho->SetSearchID(SearchID);
                                CThing* loop_thing=&pmapwho->GetParentThing();
                                if((loop_thing->GetTypeFlags()&flags)==flags) {
                                    T* tthing=static_cast<T*>(loop_thing);
                                    if(filter_func(*tthing))
                                        things.push_back(tthing);
                                }
                            }
                        }
                    }
                }
            }
        }
        const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    }
#elif defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
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
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET)
                    // w24A opt-in (rows 0x00959380 / 0x0095C7C0, the <CThingObject, vector, CThingFilter_None> instances): FITTED form of the
                    // PeekCell expression -- the layer's level-info record addressed as bytes (layer * sizeof(CMapwhoLevelInfo) added to the
                    // vector start), the spelling the landed family K016 rows 0x006F3570 / 0x006F37B0 / 0x007197C0 carry in their local models.
                    // Retail's vector instances read the row width with `mov ecx,[ecx+eax+4]` (layer * 20 as base, 0x0095C8C3) while the list
                    // instances (0x008780D0, PEEKCELL_INLINE) read it with the start as base (`mov eax,[eax+ebp+4]`, 0x008781D3): the same
                    // inline PeekCell in both, so the order follows the instance's context, and no natural spelling of the access (PeekCell
                    // bodies with or without ASSERT dead arms, locals, operand orders, vector index forms: 30 probes) reproduces it here.
                    const NThingMap::CThingMapCell* cell=&map.Cells.begin()[(unsigned char)layer].begin()[y*(*(const long*)((unsigned char)layer*sizeof(NThingMap::CMapwhoLevelInfo)+(size_t)map.MapwhoLevelInfo.begin()+offsetof(NThingMap::CMapwhoLevelInfo,Extents)))+cells.TLX];
#elif defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE)
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
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL
                                // agentNF41 opt-in (row 0x00746560, the <const CThing, vector<const CThing*>, CFilter_Bind3<HasPhysicsMesh,
                                // RadiusIntersectsInfiniteLine, IsNot> > instance): the typed pointer is a named local that push_back takes by
                                // reference, stored before the filter calls (0x007466C0 mov [esp+38h],edi ahead of call 0x006A6F50), the
                                // spelling of the landed row's local model.
                                if((thing.GetTypeFlags()&mask)==mask) {
                                    T* typed=static_cast<T*>(&thing);
                                    if(filter(*typed))
                                        result.push_back(typed);
                                }
#else
                                if((thing.GetTypeFlags()&mask)==mask && filter(static_cast<const T&>(thing)))
                                    result.push_back(static_cast<T*>(&thing));
#endif
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
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
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
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
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
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
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
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
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
        ++NoLookups;
        NoThingsScanned=0;
        ++SearchID;
        for(std::vector<NThingMap::CThingMap*>::iterator it=maps.begin();it!=maps.end();++it) {
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
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
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
FABLE_STATIC_ASSERT_AT(462, sizeof(CThingSearchTools)==0x18);
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
template<class Filter,class Compare,class T>
bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area,const Filter& filter,const Compare& compare,T** out)const{
 T* best=0;
 std::vector<NThingMap::CThingMap*>& maps=const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps);
 GetAllThingMapsInArea(area,maps);
 ++NoLookups;NoThingsScanned=0;++SearchID;
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND
 // NF107 opt-in (round 10, rows 0x007F7340 / 0x007F70D0 / 0x00937420, the GetPBestThingInRoughArea instances): the loop's end is read from
 // the member, as ROUGHAREA_THISEND (0x007F7363 `cmp ebp,[esi+10h]` = this->AreaMaps._M_finish, with &AreaMaps kept in ebx / [esp+40h]).
 for(std::vector<NThingMap::CThingMap*>::const_iterator it=maps.begin();it!=AreaMaps.end();++it){
#else
 for(std::vector<NThingMap::CThingMap*>::const_iterator it=maps.begin();it!=maps.end();++it){
#endif
  const NThingMap::CThingMap& map=**it;
  C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
  for(long layer=0;layer<NThingMap::MaxLayers;++layer){
   long shift=NThingMap::LayerInfo[layer].ScaleShift;
   C2DBoxI cells(local.TLX>>shift,local.TLY>>shift,((local.BRX-1)>>shift)+1,((local.BRY-1)>>shift)+1);
   if(cells.BRX-cells.TLX==0||cells.BRY-cells.TLY==0)continue;
   for(long y=cells.TLY;y<cells.BRY;++y){
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    const NThingMap::CThingMapCell* cell=&map.PeekCell((unsigned char)layer,cells.TLX,y);
#elif defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_CELL_BYTEOFFSET)
    // NF107 opt-in (with BESTROUGH_THISEND): the FITTED byte-offset cell expression of ROUGHAREA_CELL_BYTEOFFSET above (retail 0x007F7425..
    // 0x007F7468: layer * 20 and layer * 12 hoisted out of the y loop, `mov ecx,[ecx+ebp+4]` for the row width).
    const NThingMap::CThingMapCell* cell=&map.Cells.begin()[(unsigned char)layer].begin()[y*(*(const long*)((unsigned char)layer*sizeof(NThingMap::CMapwhoLevelInfo)+(size_t)map.MapwhoLevelInfo.begin()+offsetof(NThingMap::CMapwhoLevelInfo,Extents)))+cells.TLX];
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
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_PASS22_FIRST_THISEND
 for(std::vector<NThingMap::CThingMap*>::const_iterator it=maps.begin();it!=AreaMaps.end();++it){
#else
 for(std::vector<NThingMap::CThingMap*>::const_iterator it=maps.begin();it!=maps.end();++it){
#endif
  const NThingMap::CThingMap& map=**it;
  C2DBoxI local=ClipAndConvertWorldMapAreaToLocalArea(map,area);
  for(long layer=0;layer<NThingMap::MaxLayers;++layer){
   long shift=NThingMap::LayerInfo[layer].ScaleShift;
   C2DBoxI cells(local.TLX>>shift,local.TLY>>shift,((local.BRX-1)>>shift)+1,((local.BRY-1)>>shift)+1);
   if(cells.BRX-cells.TLX==0||cells.BRY-cells.TLY==0)continue;
   for(long y=cells.TLY;y<cells.BRY;++y){
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_PASS22_FIRST_PEEKCELL
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
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_STATICS
    // NF103 opt-in (round 10): the rough-area search statics, Ego_r.pdb / FableWin.pdb ?NoLookups@CThingSearchTools@@2KA (0x013B89A8),
    // ?NoThingsScanned@CThingSearchTools@@2KA (0x013B89AC), ?SearchID@CThingSearchTools@@2KA (0x013B89B0), as the retail branch declares them.
    static unsigned long NoLookups,NoThingsScanned,SearchID;
#endif
#ifdef FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_DECL
    // NF378 add-only opt-in (flattened layout; rows 0x006CD3B0 / 0x00914B20 / 0x0093F8A0): the const member template Ego_r.pdb / FableWin.pdb
    // ??$GetPBestThing@...@CThingSearchTools@@QBE_N...@Z, the declaration the retail branch carries under BESTTHING_DECL_PASS26.
    // Declaration only: no layout change.
    template<class Filter,class Compare,class T> bool GetPBestThing(const Filter&,const Compare&,T**)const;
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(564, sizeof(CThingSearchTools) == 0x18);
FABLE_STATIC_ASSERT_AT(565, offsetof(CThingSearchTools, WorldMap) == 0x0);
FABLE_STATIC_ASSERT_AT(566, offsetof(CThingSearchTools, PThingMapManager) == 0x4);
FABLE_STATIC_ASSERT_AT(567, offsetof(CThingSearchTools, ThingManager) == 0x8);
FABLE_STATIC_ASSERT_AT(568, offsetof(CThingSearchTools, AreaMaps) == 0xc);

#endif // FABLE_RETAIL_CTHINGSEARCHTOOLS
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_EGOD) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA) && !defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD)
// egod opt-in (hand4c 2026-11-19, row 0x00518340, the <CTCCreatureGenerator, CThingFilter_IsInQuestOrNullQuest, std::vector<const
// CTCCreatureGenerator*> > instance): the GetAllTCsInRoughArea<T,Filter,Container> body defined out of class (the in-class declaration is
// TCSINROUGHAREA's; appended after the retail / non-retail branches so no assert line moves) with Ego_d.pdb's parameter names, locals and scopes (world_map.obj record:
// filter_func, tcs, it, thing_map, local_area, c1, clipped_area, cy, pcell, cx, ploop_node, pmapwho, loop_thing, tc, tc_type) in FableWin.pdb's
// statement order (debug 0x01CCB630, 1382 bytes: the ROUGHAREA_EGOD cell walk -- GetMapwhoArea, GetWidth / GetHeight, PeekCell --
// then GetParentThing, T::StatGetInterfaceType() into tc_type, `loop_thing && GetTC(tc_type, &tc) && filter_func(*loop_thing)` and
// push_back(tc); retail keeps the loop_thing null test, 0x0051849A). Needs ROUGHAREA_EGOD (statics, GetMapwhoArea), the CThing
// GetTC(id, ptc) member template and T::StatGetInterfaceType at the point of instantiation.
template<class T,class Filter,class Container>
void CThingSearchTools::GetAllTCsInRoughArea(const C2DBoxI& area,const Filter& filter_func,Container& tcs) const {
    GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
    ++NoLookups;
    NoThingsScanned=0;
    ++SearchID;
    for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
        NThingMap::CThingMap& thing_map=**it;
        C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
        for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
            C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
            if(clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0)
                continue;
            for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                    for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                        CTCMapwho* pmapwho=ploop_node->PMapwho;
                        if(pmapwho->GetSearchID()<SearchID) {
                            pmapwho->SetSearchID(SearchID);
                            CThing* loop_thing=&pmapwho->GetParentThing();
                            T* tc;
                            ETCInterfaceType tc_type=T::StatGetInterfaceType();
                            if(loop_thing && loop_thing->GetTC(tc_type,&tc) && filter_func(*loop_thing))
                                tcs.push_back(tc);
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
}
#endif
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC) && !defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_BODY) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD)
// egod opt-in (hand4c 2026-11-19, row 0x0066A1E0, the <CThingPhysical, CFilter_Bind3<CThingFilter_SolidToThing,
// CThingFilter_PhysicsFlagsSolidToThing, CThingFilter_OverlappingCircle>, `anonymous namespace'::CThingFindHeightZBelowAt> instance): the
// ForEachInRoughArea<T,Filter,Proc> body defined out of class (in-class declaration: FOREACHINROUGHAREA + FOREACHINROUGHAREA_CONSTPROC;
// appended after the retail / non-retail branches so no assert line moves) with Ego_d.pdb's parameter names, locals and scopes
// (thing_creature_base.obj record: filter_func, op_func, got_one, flags, it, map (a const reference), local_area, c1, clipped_area, cy,
// pcell, cx, ploop_node, pmapwho, loop_thing, tthing) in FableWin.pdb's statement order (debug 0x01DAAC80, 1606 bytes: got_one = false
// and flags before GetAllThingMapsInArea, the ROUGHAREA_EGOD walk with the empty test on the edges (FableWin 0x01DAB083 `cmp TLX,BRX ; je`,
// `cmp BRY,TLY ; je`, no GetWidth / GetHeight calls, unlike the GetAllThingsInRoughArea / GetAllTCsInRoughArea copies), then `filter_func(*tthing) && op_func(*tthing)` sets got_one).
// Needs ROUGHAREA_EGOD (statics, GetMapwhoArea) and T::StatGetTypeFlags at the point of instantiation.
template<class T,class Filter,class Proc>
bool CThingSearchTools::ForEachInRoughArea(const C2DBoxI& area,const Filter& filter_func,const Proc& op_func) const {
    bool got_one=false;
    unsigned long flags=T::StatGetTypeFlags();
    GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
    ++NoLookups;
    NoThingsScanned=0;
    ++SearchID;
    for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
        const NThingMap::CThingMap& map=**it;
        C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(map,area);
        for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
            C2DBoxI clipped_area=GetMapwhoArea(map,c1,local_area);
            if(clipped_area.TLX==clipped_area.BRX || clipped_area.BRY==clipped_area.TLY)
                continue;
            for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                const NThingMap::CThingMapCell* pcell=&map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                    for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                        CTCMapwho* pmapwho=ploop_node->PMapwho;
                        if(pmapwho->GetSearchID()<SearchID) {
                            pmapwho->SetSearchID(SearchID);
                            CThing* loop_thing=&pmapwho->GetParentThing();
                            if((loop_thing->GetTypeFlags()&flags)==flags) {
                                T* tthing=static_cast<T*>(loop_thing);
                                if(filter_func(*tthing) && op_func(*tthing))
                                    got_one=true;
                            }
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    return got_one;
}
#endif
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCDSINROUGHAREA_EGOD) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA) && !defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD)
// NF101 opt-in (round 9, q11280 / q11283, row 0x005185F0, the <CTCDNavigationSeed, CThingFilter_IsInQuestOrNullQuest, std::vector<const
// CThing*> > instance): the GetAllTCDsInRoughArea<T,Filter,Container> body defined out of class (in-class declaration: TCSINROUGHAREA;
// appended after the retail / non-retail branches so no assert line moves): the TCSINROUGHAREA_EGOD walk of GetAllTCsInRoughArea above
// (GetAllThingMapsInArea 0x0049C6F0, the NoLookups / NoThingsScanned / SearchID statics, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
// GetMapwhoArea, GetWidth / GetHeight, PeekCell) with the driver test of the TCSINROUGHAREA_BODY copy: the parent thing's HasTCDriver()
// (PTCDriver +0x5C), GetTCDriver().IsDriverType(T::StatGetDriverType()) (`push 15h ; call [edx+70h]`, 0x00518754) and the filter, then
// push_back(loop_thing) (a CThing* local: a CThing& one changes the register allocation, graded DIFFER 595v597); retail has no null test
// on the parent thing here (0x00518737..0x00518745). Local names follow the TCs EGOD body
// (same world_map.obj; descriptive where Ego_d's TCD record was not read). Needs ROUGHAREA_EGOD, FABLE_RETAIL_CTHING_TCDRIVER,
// FABLE_RETAIL_CTCDRIVERBASE_ISDRIVERTYPE and T::StatGetDriverType at the point of instantiation.
template<class T,class Filter,class Container>
void CThingSearchTools::GetAllTCDsInRoughArea(const C2DBoxI& area,const Filter& filter_func,Container& things) const {
    GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
    ++NoLookups;
    NoThingsScanned=0;
    ++SearchID;
    for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
        NThingMap::CThingMap& thing_map=**it;
        C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
        for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
            C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
            if(clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0)
                continue;
            for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                    for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                        CTCMapwho* pmapwho=ploop_node->PMapwho;
                        if(pmapwho->GetSearchID()<SearchID) {
                            pmapwho->SetSearchID(SearchID);
                            CThing* loop_thing=&pmapwho->GetParentThing();
                            if(loop_thing->HasTCDriver() && loop_thing->GetTCDriver().IsDriverType(T::StatGetDriverType()) && filter_func(*loop_thing))
                                things.push_back(loop_thing);
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
}
#endif
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_EGOD) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH) && !defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD) && defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
// NF110 opt-in (round 10, q11280 / q11283; the GetPFirstThingInRoughArea<Filter,T> instances 0x007FCEF0, 0x007512E0, ...): the body the landed
// row 0x009871C0 defines in its own source (verbatim), moved here so every instance shares it: Ego_d.pdb's own record of the
// GetPFirstThingInRoughArea instances (parameters area / filter_func / thing, function-scope `flags`, it, thing_map, local_area, c1,
// clipped_area, cy, pcell, cx, ploop_node, pmapwho, loop_thing, tthing), i.e. the ROUGHAREA_EGOD GetAllThingsInRoughArea text above (inline
// GetMapwhoArea, GetWidth() / GetHeight() empty test, inline PeekCell) with the first hit returned through `thing` (the scratch list left as
// is) and the list cleared on a miss. Declared by FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH; needs FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE,
// FABLE_RETAIL_C2DBOXI_COPY / _GETWIDTH / _GETHEIGHT, FABLE_RETAIL_CTCMAPWHO_SEARCHID and T::StatGetTypeFlags at the point of instantiation.
template<class Filter,class T>
bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& area,const Filter& filter_func,T** thing) const
{
    unsigned long flags=T::StatGetTypeFlags();
    GetAllThingMapsInArea(area,const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps));
    ++NoLookups;
    NoThingsScanned=0;
    ++SearchID;
    for(std::vector<NThingMap::CThingMap*>::const_iterator it=AreaMaps.begin();it!=AreaMaps.end();++it) {
        NThingMap::CThingMap& thing_map=**it;
        C2DBoxI local_area=ClipAndConvertWorldMapAreaToLocalArea(thing_map,area);
        for(long c1=0;c1<NThingMap::MaxLayers;++c1) {
            C2DBoxI clipped_area=GetMapwhoArea(thing_map,c1,local_area);
            if(clipped_area.GetWidth()==0 || clipped_area.GetHeight()==0)
                continue;
            for(long cy=clipped_area.TLY;cy<clipped_area.BRY;++cy) {
                const NThingMap::CThingMapCell* pcell=&thing_map.PeekCell((unsigned char)c1,clipped_area.TLX,cy);
                for(long cx=clipped_area.TLX;cx<clipped_area.BRX;++cx,++pcell) {
                    for(const NThingMap::CMapwhoNode* ploop_node=pcell->PFirstNode;ploop_node;ploop_node=ploop_node->PNext) {
                        CTCMapwho* pmapwho=ploop_node->PMapwho;
                        if(pmapwho->GetSearchID()<SearchID) {
                            pmapwho->SetSearchID(SearchID);
                            CThing* loop_thing=&pmapwho->GetParentThing();
                            if((loop_thing->GetTypeFlags()&flags)==flags) {
                                T* tthing=static_cast<T*>(loop_thing);
                                if(filter_func(*tthing)) {
                                    *thing=tthing;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    const_cast<std::vector<NThingMap::CThingMap*>&>(AreaMaps).clear();
    return false;
}
#endif
#endif // FABLE_ENGINE_CTHINGSEARCHTOOLS_H
