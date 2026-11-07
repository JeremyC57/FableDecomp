STANDIN = "struct CTCCreatureNavigation {                  // interface 7\n    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector& result, bool onMap) const;   // 0x006CB6E0\n};\n"
STANDIN_BODY = STANDIN + "bool NavSink_s8(const CTCCreatureNavigation* nav, const C3DVector* centre, float radius, C3DVector* result, bool onMap);\n#pragma auto_inline(off)\n__declspec(noinline) bool CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector& result, bool onMap) const   // same-TU stand-in (probe of the address-exposure lever)\n{\n    return NavSink_s8(this, &centre, radius, &result, onMap);\n}\n#pragma auto_inline(on)\n"
POS_OLD = """                    C3DVector position;
                    if (dummy->HasTC(7))
                        ((CTCCreatureNavigation*)dummy->GetTC(7))->GetNearbyWanderPosition(ParentThing->GetPos(), 50.0f, position, true);
                    else
                        position = ParentThing->GetPos();
                    villager->CreateVillager(*this, position, false);"""
AXES = [
 ("parent", [("", "", []),
             ("pl", "parent local in the branch", [(POS_OLD, POS_OLD.replace("                    C3DVector position;\n", "                    CThing* parent = ParentThing;\n                    C3DVector position;\n").replace("ParentThing->GetPos()", "parent->GetPos()"))]),
             ("pr", "parent reference in the branch", [(POS_OLD, POS_OLD.replace("                    C3DVector position;\n", "                    CThing& parent = *ParentThing;\n                    C3DVector position;\n").replace("ParentThing->GetPos()", "parent.GetPos()"))])]),
 ("nav", [("", "", []),
          ("nl", "nav local", [("                    if (dummy->HasTC(7))\n                        ((CTCCreatureNavigation*)dummy->GetTC(7))->GetNearbyWanderPosition(", "                    if (dummy->HasTC(7)) {\n                        CTCCreatureNavigation* nav = (CTCCreatureNavigation*)dummy->GetTC(7);\n                        nav->GetNearbyWanderPosition("),
                               ("50.0f, position, true);\n                    else", "50.0f, position, true);\n                    } else")])]),
 ("scope", [("", "", []),
            ("sl", "position at loop scope", [("                    C3DVector position;\n", ""), ("        CThing* dummy = it->_M_data.GetPItem();\n", "        C3DVector position;\n        CThing* dummy = it->_M_data.GetPItem();\n")]),
            ("sf", "position at function scope", [("                    C3DVector position;\n", ""), ("    long defIndex = ", "    C3DVector position;\n    long defIndex = ")])]),
 ("kill", [("", "", []),
           ("ks", "enable arm first", [("                if (!enable) {\n                    if (IS_THING_ALIVE(villager->PVillager.GetPItem()))\n                        villager->PVillager.GetPItem()->Kill(false);\n                } else if (!IS_THING_ALIVE(villager->PVillager.GetPItem())) {",
                                        "                if (enable) {\n                    if (IS_THING_ALIVE(villager->PVillager.GetPItem()))\n                        continue;"),
                                       ("                    villager->CreateVillager(*this, position, false);\n                }", "                    villager->CreateVillager(*this, position, false);\n                } else if (IS_THING_ALIVE(villager->PVillager.GetPItem()))\n                    villager->PVillager.GetPItem()->Kill(false);")])]),
 ("standin", [("", "", []),
              ("sb", "same-TU noinline stand-in body for GetNearbyWanderPosition", [(STANDIN, STANDIN_BODY)])]),
 ("gettc", [("", "", []),
            ("gt", "template GetTC helper", [("    __forceinline CTCBase* GetTC(long interfaceType)\n", "    template <class T> __forceinline T* GetTCAs(long interfaceType) { return (T*)GetTC(interfaceType); }\n    __forceinline CTCBase* GetTC(long interfaceType)\n"),
                                            ("(CTCDummyVillager*)dummy->GetTC(0xd6)", "dummy->GetTCAs<CTCDummyVillager>(0xd6)"), ("(CTCCreatureNavigation*)dummy->GetTC(7)", "dummy->GetTCAs<CTCCreatureNavigation>(7)")])]),
]
