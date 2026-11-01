// CTCBase::PeekThingSearchTools() const -- the world's thing-search tools
// Leaf: ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFD40, 9 bytes: `call 0x00686A80 ; mov eax,[eax+20h] ; ret`.
// Identity (naming wave 25): Ego_r.pdb ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ (0x006B2180, after PeekWorld 0x006B1E70)
//   and the catalog name; the body is `*PeekWorld().PThingSearchTools` (CWorld +0x20 scoped_ptr<CThingSearchTools>, Ego_r.pdb and the
//   landed engine/CWorld.h). Callee: ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (DECISIONS_W24_RESOLVED item 7). Was a free
//   GetCombatManager(CThingCreatureBase*) over a helper_686a80 stand-in.
class CThingSearchTools;
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;                // +0x20 scoped_ptr<CThingSearchTools> (its one pointer, as the landed CWorld models)
};
class CTCBase {
protected:
    const CWorld& PeekWorld() const;                     // 0x00686A80
    const CThingSearchTools& PeekThingSearchTools() const;
};
const CThingSearchTools& CTCBase::PeekThingSearchTools() const
{
    return *PeekWorld().PThingSearchTools;
}
