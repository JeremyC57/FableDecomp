cardold = "struct Card { bool IsCore() const;"
V = [
 ("ptrnull", [(cardold, "struct Card { void* Ptr() const; bool IsCore() const;"), ("            if (card->IsCore()) {\n", "            if (card->Ptr() != 0) {\n")]),
 ("ptrnull2", [(cardold, "struct Card { void* Ptr() const; bool IsCore() const;"), ("            if (card->IsCore()) {\n", "            if (card->Ptr()) {\n")]),
]
