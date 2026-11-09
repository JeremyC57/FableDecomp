fbui = """                    container = manager->Create("UI_CORE", false);
                    text = container->Find("UI_QN");
                }
            }
            if (text)"""
V = [
 ("fbret", [(fbui, fbui.replace('text = container->Find("UI_QN");\n', 'text = container->Find("UI_QN");\n                    return;\n', 1))]),
 ("fbcont", [(fbui, fbui.replace('text = container->Find("UI_QN");\n', 'text = container->Find("UI_QN");\n                    continue;\n', 1))]),
 ("forever", [("    for (Node* it = head->next; it != head; it = it->next) {\n", "    for (Node* it = head->next;; it = it->next) {\n        if (it == head) break;\n")]),
]
