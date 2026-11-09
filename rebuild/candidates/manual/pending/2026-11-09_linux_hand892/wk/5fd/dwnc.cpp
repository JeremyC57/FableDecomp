struct Str { Str(const char* s, long n = -1); Str(const Str&); ~Str(); void* p; };
struct C {
    virtual C* Find(Str name);
    virtual void Add(C*& child);
    virtual void Set(int a, bool b, bool c);
};
struct M { C* Create(const Str& name, bool b); };
M* GetM();
struct Card { bool IsCore() const; bool IsVig() const; bool IsOpt() const; int Q() const; };
struct Node { Node* next; Card* card; };
bool Match(Card*);
void F(Node* head, C* parent, bool pc)
{
    M* manager = GetM();
    C* container = 0;
    for (Node* it = head->next; it != head; it = it->next) {
        Card* card = it->card;
        if (Match(card)) {
            C* text;
            if (card->IsCore()) {
                if (pc) {
                    container = manager->Create("PC_CORE", false);
                    text = container->Find("PC_QN");
                } else {
                    container = manager->Create("UI_CORE", false);
                    text = container->Find("UI_QN");
                }
            } else {
                do {
                    if (card->IsVig()) {
                        if (pc) {
                            container = manager->Create("PC_VIG", false);
                            text = container->Find("PC_QN");
                        } else {
                            container = manager->Create("UI_VIG", false);
                            text = container->Find("UI_QN");
                        }
                    } else if (card->IsOpt()) {
                        if (pc) {
                            container = manager->Create("PC_OPT", false);
                            text = container->Find("PC_QN");
                        } else {
                            container = manager->Create("UI_OPT", false);
                            text = container->Find("UI_QN");
                        }
                    } else {
                        if (pc) {
                            container = manager->Create("PC_CORE", false);
                            text = container->Find("PC_QN");
                        } else {
                            container = manager->Create("UI_CORE", false);
                            text = container->Find("UI_QN");
                        }
                    }
                } while (0);
            }
            if (text)
                text->Set(card->Q(), true, true);
            parent->Add(container);
        }
    }
}
