#ifndef FABLE_ENGINE_CTHINGFILTER_HASINFORMATION_H
#define FABLE_ENGINE_CTHINGFILTER_HASINFORMATION_H
// New header (agentC14, row 0x00603000 CTCInventoryMap::CreateMiniMapMarkers). Ego_r.pdb
// ??RCThingFilter_HasInformation@@QBE_NABVCThingAICreature@@@Z (0x005A2B70; retail 0x006A7140, out of line) and the search instance
// ??$GetAllLeafThings@VCThingAICreature@@V?$list@PAVCThingAICreature@@...@@V?$CFilter_Bind2@VCThingFilter_HasInformation@@VCThingFilter_InMap@@@@@CThingSearchTools@@
// (Ego_r 0x006683D0; retail 0x00617A00, landed). An empty filter: no data members (no record in struct_layouts_egor.tsv).
class CThingAICreature;
class CThingFilter_HasInformation {
public:
    bool operator()(const CThingAICreature& creature) const;   // 0x006A7140
};
#endif
