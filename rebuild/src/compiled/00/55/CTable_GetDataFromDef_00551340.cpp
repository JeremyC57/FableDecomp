// NUISystem::CTable::GetDataFromDef() -- copy the table def's size and separation vectors, then create and initialise one component per def sprite
// Retail: 0x00551340, 431 bytes (catalog length corrected 2026-11-05 from 428, which cut the final "add esp,10h ; ret" at 0x005514EB-0x005514EE).
// Leaf: ?GetDataFromDef@CTable@NUISystem@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00551340, 431 bytes (0x00551340..0x005514EE, int3 at 0x005514EF, next function 0x005514F0); called by CTable(long) 0x005517E0.
// Callees: CComponent::LoadAndReturnDef() const (slot 108, +0x1B0, hidden CDefPointer<const CUIDef> result), operator new 0x00BFEA1A,
//   std::vector<unsigned long>::vector(const vector&) 0x00535470, NUISystem::CManager::GetInstance 0x0041E5F2,
//   NUISystem::CManager::CreateComponent(long) 0x0041D21B, std::map<ETableSprites,CCountedPointer<CComponent> >::operator[] 0x00554030
//   (FableWin/Ego_r ??A?$map@W4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@..., out of line: FABLE_RETAIL_STLP_MAP
//   stays off so the shared map declares it), CCountedPointer<CComponent>::DeleteData 0x00428B10, operator delete 0x00BFE9BC, CComponent::Initialise (slot 43, +0xAC).
// Shape: the def release is inline with no null test (VC7.1 drops it once the def is dereferenced, as in 0x00546CD0 / 0x00547C10);
//   FABLE_RETAIL_CCOUNTEDPOINTER (the (T*) ctor tests the stored member) gives retail's 0x10 frame; the plain ctor packs it to 0xC.
// Header: engine/NUISystem_CTable.h (Sprites +0x15C, PHorizontalSeparations +0x168, PVerticalSeparations +0x16C), engine/NUISystem_CUIDef.h
//   retail branch (Height +0x58, Width +0x5C, Sprites +0x64, HorizontalSeparations +0x7C, VerticalSeparations +0x88), CComponent::Size +0xCC.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "engine/NUISystem_CTable.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CDefPointer.h"
namespace NUISystem {
void CTable::GetDataFromDef()
{
    CDefPointer<const CUIDef> def = LoadAndReturnDef();
    Size.X = def->Width;
    Size.Y = def->Height;
    PHorizontalSeparations = new std::vector<unsigned long>(def->HorizontalSeparations);
    PVerticalSeparations = new std::vector<unsigned long>(def->VerticalSeparations);
    for (std::map<ETableSprites, long>::const_iterator it = def->Sprites.begin(); it != def->Sprites.end(); ++it) {
        Sprites[(*it).first] = CCountedPointer<CComponent>(CManager::GetInstance()->CreateComponent((*it).second));
        Sprites[(*it).first]->Initialise();
    }
}
}
