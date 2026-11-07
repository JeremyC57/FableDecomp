PB_DECL = "    void push_back(const T& x);                      // 0x00486024\n"
PB_BODY = "    void push_back(const T& x);                      // 0x00486024 (same-TU body below)\n    void _M_insert_overflow(const T& x);\n};\ntemplate <class T> __declspec(noinline) void CRetailVector<T>::push_back(const T& x)\n{\n    if (_M_finish != _M_end_of_storage) {\n        *_M_finish = x;\n        ++_M_finish;\n    } else\n        _M_insert_overflow(x);\n}\nstruct CRetailVectorTail_unused {\n"
AXES = [
 ("pos", [("", "", []),
          ("tmp", "position temp in the call", [("    C2DVector position(GetScreenWidth()", "    PSaveInProgress->ChangePosition(C2DVector(GetScreenWidth()"),
                                                ("52.0f);\n    PSaveInProgress->ChangePosition(position, 0.0f, false);", "52.0f), 0.0f, false);")])]),
 ("half", [("", "", []),
           ("hl", "0.5f on the left", [("GetScreenWidth() * 0.5f", "0.5f * GetScreenWidth()"), ("GetScreenHeight() * 0.5f", "0.5f * GetScreenHeight()")]),
           ("dv", "/ 2.0f", [("GetScreenWidth() * 0.5f", "GetScreenWidth() / 2.0f"), ("GetScreenHeight() * 0.5f", "GetScreenHeight() / 2.0f")])]),
 ("sc", [("", "", []),
         ("sc", "constant first in the scale products", [("CManager::GetInstance()->GetUIScale().X * 204.0f", "204.0f * CManager::GetInstance()->GetUIScale().X"),
                                                         ("CManager::GetInstance()->GetUIScale().Y * 52.0f", "52.0f * CManager::GetInstance()->GetUIScale().Y")])]),
 ("pb", [("", "", []),
         ("pb", "same-TU noinline push_back body", [(PB_DECL, PB_BODY)])]),
 ("bk", [("", "", []),
         ("bk", "back() accessor", [("    T& operator[](unsigned long n) const { return *(begin() + n); }", "    T& operator[](unsigned long n) const { return *(begin() + n); }\n    T& back() const { return *(_M_finish - 1); }"),
                                    (">Components[Components.size() - 1]", "Components.back()")])]),
 ("nm", [("", "", []),
         ("nt", "dialog name as a temporary in the call", [("    {\n        CCharString name(\"PC_SAVE_IN_PROGRESS_DIALOG\", -1);\n        Components.push_back(CManager::GetInstance()->CreateComponent(name, false));\n    }",
                                                            "    Components.push_back(CManager::GetInstance()->CreateComponent(CCharString(\"PC_SAVE_IN_PROGRESS_DIALOG\", -1), false));")])]),
]
