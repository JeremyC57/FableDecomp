// CTCQuestCompletionUI::OnSerialise(CPersistContext& context, bool in_game) -- saves or restores quest-start reward values
// Leaf: ?OnSerialise@CTCQuestCompletionUI@@MAEXAAVCPersistContext@@_N@Z
// Grade: DIFFER(1204v1258) -- call sequence now matches (defs/experience/gold overflow calls, renown growth inline) but this file merges the defs and experience map walks; with four walks (retail size) the gold growth is also inlined (1428)
// Context: grades against the Codex row context $S/codex11_dl/x/rows/005e8220/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p01; 005e8220 probes in probe/005e8220.log
#define FABLE_RETAIL_QUEST_ALLOCATOR_LIFETIME
#define FABLE_RETAIL_QUEST_COMPLETION
#define FABLE_RETAIL_CTCBASE
#define FABLE_STLPORT_HTTP
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CPersistContext.h"
void CTCQuestCompletionUI::OnSerialise(CPersistContext& context,bool in_game)
{
    if(in_game) {
        if(context.IsSaving()) {
            std::vector<long> defs;
            std::vector<unsigned long> experience;
            std::vector<unsigned long> gold;
            std::vector<unsigned long> renown;
            for(std::map<long,unsigned long>::const_iterator it=StartQuestExperience.begin();it!=StartQuestExperience.end();++it) {
                defs.push_back(it->first);
                experience.push_back(it->second);
            }
            for(std::map<long,unsigned long>::const_iterator it=StartQuestGold.begin();it!=StartQuestGold.end();++it)
                gold.push_back(it->second);
            for(std::map<long,unsigned long>::const_iterator it=StartQuestRenown.begin();it!=StartQuestRenown.end();++it)
                renown.push_back(it->second);
            context.TransferVector("defs",defs);
            context.TransferVector("experience",experience);
            context.TransferVector("renown",renown);
            context.TransferVector("gold",gold);
        } else {
            std::vector<long> defs;
            std::vector<unsigned long> experience;
            std::vector<unsigned long> gold;
            std::vector<unsigned long> renown;
            context.TransferVector("defs",defs);
            context.TransferVector("experience",experience);
            context.TransferVector("renown",renown);
            context.TransferVector("gold",gold);
            for(unsigned long count=0;count<defs.size();++count) {
                StartQuestExperience[defs[count]]=experience[count];
                StartQuestGold[defs[count]]=gold[count];
                StartQuestRenown[defs[count]]=renown[count];
            }
        }
    }
}
