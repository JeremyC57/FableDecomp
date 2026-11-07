LB_OLD = "    const SEntry* LowerBound(const long* type) const;   // 0x004365B0\n};"
LB_BODY = LB_OLD + """
#pragma auto_inline(off)
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const   // same-TU body (STLport lower_bound)
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)"""
SIDE_OLD = "        SideVec = C3DVector(pf[1] * up.Z - pf[2] * up.Y, pf[2] * up.X - pf[0] * up.Z, pf[0] * up.Y - pf[1] * up.X);"
AXES = [
 ("ux", [("", "", []),
         ("u0", "up.X as pu[0]", [(SIDE_OLD, SIDE_OLD.replace("up.X", "pu[0]"))]),
         ("us", "up.X as *pu", [(SIDE_OLD, SIDE_OLD.replace("up.X", "(*pu)"))]),
         ("ua", "all up components through pu[]", [(SIDE_OLD, SIDE_OLD.replace("up.X", "pu[0]").replace("up.Y", "pu[1]").replace("up.Z", "pu[2]"))])]),
 ("fw", [("", "", []),
         ("fm", "forward members", [(">pf[0]", "forward.X"), (">pf[1]", "forward.Y"), (">pf[2]", "forward.Z")])]),
 ("lb", [("", "", []),
         ("lb", "same-TU LowerBound body", [(LB_OLD, LB_BODY)])]),
 ("neg", [("", "", []),
          ("nc", "component negation", [("            SideVec = -SideVec;", "            SideVec.X = -SideVec.X;\n            SideVec.Y = -SideVec.Y;\n            SideVec.Z = -SideVec.Z;")])]),
 ("orient", [("", "", []),
             ("ot", "orientation TC local", [("        const CRightHandedSet& orientation = creature.PeekTC<CTCOrientation>(2)->GetOrientation();", "        const CTCOrientation* tc = creature.PeekTC<CTCOrientation>(2);\n        const CRightHandedSet& orientation = tc->GetOrientation();")])]),
 ("pos", [("", "", []),
          ("pc", "OurPos via a const ref", [("    OurPos = creature.GetPosition();", "    const C3DVector& pos = creature.GetPosition();\n    OurPos = pos;")])]),
]
