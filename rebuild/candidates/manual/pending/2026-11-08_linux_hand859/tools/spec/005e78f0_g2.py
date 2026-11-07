# hypothesis: the retail frame copies of the three targets ([esp+18h/1Ch/20h]) are user locals, not spills
R_OLD = "    RenownGainedDisplayed += (TargetRenownGained - InitialRenown) / (CMainGameComponent::GetConstantFPS() * digit_count_time);"
E_OLD = "    ExperienceGainedDisplayed += (TargetExperienceGained - InitialExperience) / (CMainGameComponent::GetConstantFPS() * digit_count_time);"
G_OLD = "    GoldGainedDisplayed += (TargetGoldGained - InitialGold) / (CMainGameComponent::GetConstantFPS() * digit_count_time);"
CL_OLD = """    if (RenownGainedDisplayed > TargetRenownGained)
        RenownGainedDisplayed = TargetRenownGained;
    if (GoldGainedDisplayed > TargetGoldGained)
        GoldGainedDisplayed = TargetGoldGained;
    if (ExperienceGainedDisplayed > TargetExperienceGained)
        ExperienceGainedDisplayed = TargetExperienceGained;"""
def locs(which, where):
    # which: set of 'r','e','g'; where: 'top' (before the three steps) or 'each' (right before its own step)
    decl = {"r": "    long target_renown = TargetRenownGained;\n", "e": "    long target_experience = TargetExperienceGained;\n", "g": "    long target_gold = TargetGoldGained;\n"}
    name = {"r": "target_renown", "e": "target_experience", "g": "target_gold"}
    edits = []
    cl = CL_OLD
    for k, old, member in (("r", R_OLD, "TargetRenownGained"), ("e", E_OLD, "TargetExperienceGained"), ("g", G_OLD, "TargetGoldGained")):
        if k in which:
            new = old.replace(member, name[k])
            if where == "each": new = decl[k] + new
            edits.append((old, new))
            cl = cl.replace(member, name[k])
    if where == "top":
        edits.append(("    float digit_count_time = ", "".join(decl[k] for k in "reg" if k in which) + "    float digit_count_time = "))
    edits.append((CL_OLD, cl))
    return edits
AXES = [
 ("loc", [("le", "experience target local", locs("e", "each")),
          ("la", "all three target locals before their steps", locs("reg", "each")),
          ("lt", "all three target locals at the top", locs("reg", "top")),
          ("lr", "renown+experience locals", locs("re", "each")),
          ("lg", "experience+gold locals", locs("eg", "each")),
          ("lb", "all three at the top, before digit_count_time", locs("reg", "top"))]),
 ("step", [("", "", []),
           ("sa", "X = X + step", [(">    RenownGainedDisplayed += (", "    RenownGainedDisplayed = RenownGainedDisplayed + ("), (">    ExperienceGainedDisplayed += (", "    ExperienceGainedDisplayed = ExperienceGainedDisplayed + ("), (">    GoldGainedDisplayed += (", "    GoldGainedDisplayed = GoldGainedDisplayed + (")])]),
]
