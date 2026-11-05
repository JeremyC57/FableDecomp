// CTCQuestCompletionUI::OnSerialise(CPersistContext& context, bool in_game) -- save or restore the three per-quest start maps (StartQuestExperience / StartQuestGold / StartQuestRenown, keyed by quest-card def) as four parallel vectors "defs", "experience", "renown", "gold"
// Leaf: ?OnSerialise@CTCQuestCompletionUI@@MAEXAAVCPersistContext@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E8220, 1258 bytes, thiscall `ret 8`, no EH frame.
// Identity: CTCQuestCompletionUI vtable 0x0125589C slot 1 (rtti-vtables.tsv; the CTCBase OnSerialise slot, `ret 8`, the only
//   reference to 0x005E8220 is that vtable entry 0x012558A0); Ego_r.pdb + FableWin.pdb ?OnSerialise@CTCQuestCompletionUI@@MAEXAAVCPersistContext@@_N@Z
//   (protected virtual). The transfer names are the PDB members' contents: the three maps are Ego_r CTCQuestCompletionUI
//   StartQuestExperience / StartQuestGold / StartQuestRenown (map<long, unsigned long>), "gold" is the +0x90 map, "renown" the +0x9C one.
//   Label correction: the catalog's "_global::CTCInventoryMapSerialization_TransferCharactersAndStats" is a guess.
// Callees: CPersistContext::TransferVector<long> 0x005815D6 and <unsigned long> 0x005E9840 (landed), map<long, unsigned long>
//   ::operator[] 0x005E9760 (landed), vector<long>::_M_insert_overflow 0x00440990, vector<unsigned long>::_M_insert_overflow
//   0x00464ACE, malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14 (the one expanded _M_insert_overflow and the vector dtors).
// Layout (retail; Ego_r.pdb CTCQuestCompletionUI with STLport sizes: Components vector 12, Boasts list 4): StartQuestExperience
//   +0x84, StartQuestGold +0x90, StartQuestRenown +0x9C (12-byte STLport maps; node key +0x10, value +0x14); CPersistContext Mode
//   +0x18 (IsSaving: 1 or 3).
// Shape: `if (!in_game) return;` then the inline IsSaving() split; each branch declares its own four vectors (defs, experience,
//   gold, renown: the destruction order) and transfers them in the order defs, experience, renown, gold. The save branch walks the
//   maps with the inline STLport iterator (the expanded _Rb_global::_M_increment, end() re-read each pass) and push_back; the
//   load branch writes map[defs[i]] for the three maps. Retail expands _M_insert_overflow at the LAST push_back only (renown) and
//   calls it at the other three: VC7.1's per-site inline choice follows the first walk's loop form. The first walk written as
//   `iterator it = begin(); while (it != end()) {...; ++it;}` (the iterator initialised at its declaration, reused by the three
//   `for` walks) gives retail's one expansion; four `for` loops expand two, four `while` loops none.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CTCBase.h"
#include "engine/CPersistContext.h"

// No shared CTCQuestCompletionUI header exists (header debt): the class over the retail CTCBase with the three maps this row reads.
class CTCQuestCompletionUI : public CTCBase {
public:
    unsigned char _pad_0c[0x84 - 0x0c];      // Active .. GoldGainedDisplayed (Ego_r members; not read here)
    std::map<long,unsigned long,std::less<long>,std::allocator<std::pair<const long,unsigned long> > > StartQuestExperience; // +0x84
    std::map<long,unsigned long,std::less<long>,std::allocator<std::pair<const long,unsigned long> > > StartQuestGold;       // +0x90
    std::map<long,unsigned long,std::less<long>,std::allocator<std::pair<const long,unsigned long> > > StartQuestRenown;     // +0x9c
protected:
    virtual void OnSerialise(CPersistContext& context, bool in_game);
};

void CTCQuestCompletionUI::OnSerialise(CPersistContext& context, bool in_game)
{
    if (!in_game)
        return;
    if (context.IsSaving()) {
        std::vector<long> defs;
        std::vector<unsigned long> experience;
        std::vector<unsigned long> gold;
        std::vector<unsigned long> renown;
        std::map<long,unsigned long>::iterator it = StartQuestExperience.begin();
        while (it != StartQuestExperience.end()) { defs.push_back((*it).first); ++it; }
        for (it = StartQuestExperience.begin(); it != StartQuestExperience.end(); ++it)
            experience.push_back((*it).second);
        for (it = StartQuestGold.begin(); it != StartQuestGold.end(); ++it)
            gold.push_back((*it).second);
        for (it = StartQuestRenown.begin(); it != StartQuestRenown.end(); ++it)
            renown.push_back((*it).second);
        context.TransferVector("defs", defs);
        context.TransferVector("experience", experience);
        context.TransferVector("renown", renown);
        context.TransferVector("gold", gold);
    } else {
        std::vector<long> defs;
        std::vector<unsigned long> experience;
        std::vector<unsigned long> gold;
        std::vector<unsigned long> renown;
        context.TransferVector("defs", defs);
        context.TransferVector("experience", experience);
        context.TransferVector("renown", renown);
        context.TransferVector("gold", gold);
        for (unsigned long i = 0; i < defs.size(); ++i) {
            StartQuestExperience[defs[i]] = experience[i];
            StartQuestGold[defs[i]] = gold[i];
            StartQuestRenown[defs[i]] = renown[i];
        }
    }
}