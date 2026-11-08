#ifndef FABLE_ENGINE_CGRAPHICDATABANK_H
#define FABLE_ENGINE_CGRAPHICDATABANK_H
// New header (chunk 767, row 0x00B2AD80 CEngineSurfaceCompositionManager::ChangeSceneForTextureBank). Retail RTTI
// .?AVCGraphicDataBank@@ : CBankFileAsync (+0x0), CResourceBank (+0x164) (ghidra_out/rtti_bases.tsv); sizeof 0x30C (the surface
// manager's three banks sit at +0x8C / +0x398 / +0x6A4, engine/CEngineSurfaceCompositionManager.h). The bases and members are not
// modelled yet (one opaque block); only the members rows call are declared. Ego_r.pdb / FableWin.pdb ?ChangeScene@CGraphicDataBank@@QAEXXZ,
// retail 0x009FD0C0 (landed).
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
// Retail opt-in (chunk 882, row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC): the CBankFile base at +0 (RTTI
// CGraphicDataBank : CBankFileAsync : CBankFile, all at offset 0; CBankFileAsync's own members are folded into the opaque tail),
// so the sprite bank answers CBankFile::FindIndexBySymbol (0x009CCDC0) with `this` unadjusted. sizeof 0x30C kept.
#ifndef FABLE_RETAIL_CBANKFILE_FINDINDEX
#define FABLE_RETAIL_CBANKFILE_FINDINDEX
#endif
#include "engine/CBankFile.h"
class CGraphicDataBank : public CBankFile {
public:
    void ChangeScene();                                      // 0x009FD0C0
private:
    unsigned char Unmodeled_0x110[0x30c - 0x110];            // CBankFileAsync +0x110.., CResourceBank +0x164, own members (not modelled)
};
#else
class CGraphicDataBank {
public:
    void ChangeScene();                                      // 0x009FD0C0
#ifdef FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the two graphic-size virtuals the row
    // calls through the bank's vtable, FableWin.pdb ?GetGraphicMaxWidth@CGraphicDataBank@@UBEKK@Z / ?GetGraphicMaxHeight@..@@UBEKK@Z at
    // slots 23 (+0x5C, 0x00A014D0) and 24 (+0x60, 0x00A01540) of ??_7CGraphicDataBank@@6B@ (rebuild/manifest/rtti-vtables.tsv). The
    // bases' 23 earlier slots are not modelled (placeholders keep the numbering); the vptr takes the first dword of the opaque block.
    virtual void _vslot00(); virtual void _vslot01(); virtual void _vslot02(); virtual void _vslot03(); virtual void _vslot04();
    virtual void _vslot05(); virtual void _vslot06(); virtual void _vslot07(); virtual void _vslot08(); virtual void _vslot09();
    virtual void _vslot10(); virtual void _vslot11(); virtual void _vslot12(); virtual void _vslot13(); virtual void _vslot14();
    virtual void _vslot15(); virtual void _vslot16(); virtual void _vslot17(); virtual void _vslot18(); virtual void _vslot19();
    virtual void _vslot20(); virtual void _vslot21(); virtual void _vslot22();
    virtual unsigned long GetGraphicMaxWidth(unsigned long graphic) const;    // slot 23 (+0x5C) 0x00A014D0
    virtual unsigned long GetGraphicMaxHeight(unsigned long graphic) const;   // slot 24 (+0x60) 0x00A01540
private:
    unsigned char Unmodeled_0x004[0x308];                    // CBankFileAsync +0x4.., CResourceBank +0x164, own members (not modelled)
#else
private:
    unsigned char Unmodeled_0x000[0x30c];                    // CBankFileAsync +0x0, CResourceBank +0x164, own members (not modelled)
#endif
};
#endif
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank) == 0x30c);
#endif // FABLE_ENGINE_CGRAPHICDATABANK_H
