A="rebuild/src/compiled/00/4b/CQuestManager_ActivateTestQuestCard_004b4c50.cpp"
EDITS=[(A,"""                            RemoveAvailableQuestCardType(card->GetDefName().GetString());""","""                            {
                                // the def name is a named local (as the sibling ActivateQuestCard 0x004B4AA0)
                                CCharString defName = card->GetDefName().GetString();
                                RemoveAvailableQuestCardType(defName);
                            }""")]
