// CTCInventoryAbilities::CycleAssignedExpressions(bool forward) -- pages the quick-access expression buttons one row forward or back
//   through the prioritised expression list, skipping rows with no expression, then plays the menu click (SND_MENU_04).
// Leaf: ?CycleAssignedExpressions@CTCInventoryAbilities@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6793, 442 bytes, __fastcall, `ret 4`.
// Flags: /O1 /Oy-
// Identity: Ego_r.pdb (0x005856CD) and FableWin.pdb both list ?CycleAssignedExpressions@CTCInventoryAbilities@@QAEX_N@Z; the twin of
//   CycleAssignedAbilities 0x005F64DD (same TU, same flags).
// Callees: GetPrioritisedExpressionDefs(std::vector<long>&) const 0x005EED8C, GetNumAssignableSpecialMoveButtons() const 0x005EDCF0,
//   SetExpressionToButton(long, long) 0x005F66D9, CSoundInit::CSoundInit() 0x0041C930, CThing::GetSampleBank() const 0x004C7A80,
//   CCharString::CCharString(const char*, long) 0x0099EBF0, CASoundBank::GetSoundIndexFromSymbol(const CCharString&) const 0x00A01920,
//   CCharString::~CCharString() 0x0099EAE0, CASoundManager::Get2DEmitter() (vtable +0x24; DESCRIPTIVE name, none found
//   in any PDB/FSE/EgoCore, as marked in engine/CASoundManager.h) on PSoundManager [0x013B8394],
//   CASoundEmitter::PlaySample(CASoundBank&, long, bool, const CSoundInit&, bool) (vtable +0x0C), free 0x00BFEA14 (the vector's storage).
// Shape: the empty test comes BEFORE the button reset loop here (the abilities twin resets first), so ebx is pushed late; there is no
//   clamp to 0 and no `level += level % n` (both are in the abilities twin). original_level is std::max over two references, the
//   wrap-around is std::min(original_level, 0L).
// Visibility: retail hoists `expressions.size()` out of the do-loop (0x005F6805..0x005F6813, slot [ebp-10h]) although the loop calls
//   GetNumAssignableSpecialMoveButtons. VC7.1 only does that when it can see that nothing captured the local vector, i.e. when the
//   body of GetPrioritisedExpressionDefs (same class TU, 0x005EED8C) and of the STLport push_back it calls out of line (0x00485FF3,
//   FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE) are visible. So the REAL landed body of GetPrioritisedExpressionDefs sits in this
//   row (it is not a stand-in); a declaration only gives 444v442 with the size recomputed inside the loop.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CHILD
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CTCInventoryAbilities.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"

// Real landed body of 0x005EED8C (same class TU).
void CTCInventoryAbilities::GetPrioritisedExpressionDefs(std::vector<long>& defs) const
{
    for (unsigned long i = 0; i < ReceivedExpressionOrder.size(); ++i)
        defs.push_back(ReceivedExpressionOrder[i]);
}
void CTCInventoryAbilities::CycleAssignedExpressions(bool forward)
{
 std::vector<long> expressions;
 GetPrioritisedExpressionDefs(expressions);
 if(expressions.empty())return;
 for(long button_index=0;button_index<GetNumAssignableSpecialMoveButtons();++button_index)
  SetExpressionToButton(0,button_index);
 std::vector<long>::const_iterator it=expressions.begin();
 long original_level=std::max(CurrentAssignedExpressionLevel,-GetNumAssignableSpecialMoveButtons());
 bool valid_set=false;
 do {
  if(forward)CurrentAssignedExpressionLevel+=GetNumAssignableSpecialMoveButtons();
  else CurrentAssignedExpressionLevel-=GetNumAssignableSpecialMoveButtons();
  if(CurrentAssignedExpressionLevel>=(long)expressions.size())CurrentAssignedExpressionLevel=std::min(original_level,0L);
  else if(CurrentAssignedExpressionLevel<0)
   CurrentAssignedExpressionLevel=expressions.size()-GetNumAssignableSpecialMoveButtons();
  it=expressions.begin();
  if(CurrentAssignedExpressionLevel>0)it+=CurrentAssignedExpressionLevel;
  // Guarded do-while, not a plain for: retail 0x005F6862 `and [ebp-4],0 ; call 0x5EDCF0 ; test eax,eax ; jle 0x5F6897 ;
  //   mov eax,ebx ; jmp 0x5F6878` enters the body past a one-time guard (same shape as the landed twin 0x005F64DD); a plain
  //   for loop grades 441v442 (no `mov eax,ebx ; jmp`, iterator copy spilled to [ebp-0Ch] instead).
  long c1=0;
  if(c1<GetNumAssignableSpecialMoveButtons()) {
   std::vector<long>::const_iterator expression_it=it;
   do {
    if(expression_it!=expressions.end()) {
     if(*expression_it>0){valid_set=true;break;}
    }
    ++c1;++expression_it;
   }while(c1<GetNumAssignableSpecialMoveButtons());
  }
 }while(!valid_set&&CurrentAssignedExpressionLevel!=original_level);
 if(valid_set) {
  for(long button_index=0;it!=expressions.end();++it) {
   if(*it>0)SetExpressionToButton(*it,button_index);
   else SetExpressionToButton(0,button_index);
   if(++button_index==GetNumAssignableSpecialMoveButtons())break;
  }
  if(PSoundManager) {
   CSoundInit init;init.GainFaderBits=0x20;
   CASoundBank& bank=GetParentThing().GetSampleBank();
   long sample=bank.GetSoundIndexFromSymbol("SND_MENU_04");
   PSoundManager->Get2DEmitter().PlaySample(bank,sample,true,init,false);
  }
 }
}