// CEngineWorldMap::CreateNewBankMap(const CCharString& name) -- add a new entry called `name` to the static-map bank file with a fresh CStaticMapBankStateBlock as its state block, and return the new entry's index
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3FF70, 142 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A (8 bytes, then the 12-byte count block), CBankFile::AddEntry(const CCharString&, unsigned long,
//   const CCharString&, CCountedPointer<CBankStateBlock>) @ 0x009CD620 and CBankFile::FindIndexBySymbol(const CCharString&) @ 0x009CCDC0,
//   operator delete @ 0x00BFE9BC, CCountedPointer<CStaticMapBankStateBlock>::DeleteData @ 0x00B430A0 (`if (p) p->vdtor(1)`).
// Layout: the retail StaticMapFile (CStaticMapBankFile : CBankFileAsync, 0x164) sits at +0x34; CStaticMapBankStateBlock (vtable 0x012A1F3C
//   per rtti-vtables.tsv) is 8 bytes: vptr + CStateBlock State (left uninitialised by its constructor).
// Leaf: ?CreateNewBankMap@CEngineWorldMap@@AAEKABVCCharString@@@Z
// NF65 (round 7): ported onto the shared headers: engine/CEngineWorldMap.h (overlay opt-in FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE: the
//   typed StaticMapFile member and the private CreateNewBankMap), engine/CStaticMapBankFile.h / CStaticMapBankStateBlock.h (overlay class
//   opt-ins), engine/CBankFile.h (FABLE_RETAIL_CBANKFILE_ENTRIES: AddEntry / FindIndexBySymbol), engine/CBankFileAsync.h, engine/CBankStateBlock.h,
//   engine/CCountedPointer.h. The local SCountBlock / CCountedPointer / bank-file / state-block / CEngineWorldMap models are gone. The counted
//   pointer is the FableWin.pdb pair: a CCountedPointer<CStaticMapBankStateBlock> local (its DeleteData = 0x00B430A0) converted to the
//   by-value CCountedPointer<CBankStateBlock> argument (??$?0VCStaticMapBankStateBlock@@@?$CCountedPointer@VCBankStateBlock@@@@...). Self is
//   private in Ego_r.pdb 0x009E0360 / FableWin.pdb 0x02D5FFE0 (AAE; the old local model emitted QAE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS
#define FABLE_RETAIL_CSTATICMAPBANKSTATEBLOCK_CLASS
#include "fable_compiler.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankStateBlock.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CStaticMapBankStateBlock.h"
#include "engine/CEngineWorldMap.h"

unsigned long CEngineWorldMap::CreateNewBankMap(const CCharString& name)
{
    CCountedPointer<CStaticMapBankStateBlock> state(new CStaticMapBankStateBlock);
    StaticMapFile.AddEntry(name, 0, name, state);
    return StaticMapFile.FindIndexBySymbol(name);
}
