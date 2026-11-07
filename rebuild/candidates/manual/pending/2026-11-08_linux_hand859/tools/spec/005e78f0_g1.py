R_OLD = "    RenownGainedDisplayed += (TargetRenownGained - InitialRenown) / (CMainGameComponent::GetConstantFPS() * digit_count_time);"
E_OLD = "    ExperienceGainedDisplayed += (TargetExperienceGained - InitialExperience) / (CMainGameComponent::GetConstantFPS() * digit_count_time);"
G_OLD = "    GoldGainedDisplayed += (TargetGoldGained - InitialGold) / (CMainGameComponent::GetConstantFPS() * digit_count_time);"
def steps(f):
    return [(R_OLD, f("RenownGainedDisplayed", "TargetRenownGained", "InitialRenown", "renown")),
            (E_OLD, f("ExperienceGainedDisplayed", "TargetExperienceGained", "InitialExperience", "experience")),
            (G_OLD, f("GoldGainedDisplayed", "TargetGoldGained", "InitialGold", "gold"))]
AXES = [
 ("step", [("", "", []),
           ("sa", "X = X + step", steps(lambda d,t,i,n: f"    {d} = {d} + ({t} - {i}) / (CMainGameComponent::GetConstantFPS() * digit_count_time);")),
           ("sc", "explicit float cast of the difference", steps(lambda d,t,i,n: f"    {d} += (float)({t} - {i}) / (CMainGameComponent::GetConstantFPS() * digit_count_time);")),
           ("sl", "step locals", steps(lambda d,t,i,n: f"    float {n}_step = ({t} - {i}) / (CMainGameComponent::GetConstantFPS() * digit_count_time);\n    {d} += {n}_step;")),
           ("sd", "long difference locals", steps(lambda d,t,i,n: f"    long {n}_left = {t} - {i};\n    {d} += {n}_left / (CMainGameComponent::GetConstantFPS() * digit_count_time);"))]),
 ("fps", [("", "", []),
          ("ff", "FPS cast to float first", [(">CMainGameComponent::GetConstantFPS() * digit_count_time", "(float)CMainGameComponent::GetConstantFPS() * digit_count_time")]),
          ("fd", "digit time first", [(">CMainGameComponent::GetConstantFPS() * digit_count_time", "digit_count_time * CMainGameComponent::GetConstantFPS()")])]),
 ("clamp", [("", "", []),
            ("ct", "clamps as ternaries", [("    if (RenownGainedDisplayed > TargetRenownGained)\n        RenownGainedDisplayed = TargetRenownGained;", "    RenownGainedDisplayed = RenownGainedDisplayed > TargetRenownGained ? (float)TargetRenownGained : RenownGainedDisplayed;"),
                                           ("    if (GoldGainedDisplayed > TargetGoldGained)\n        GoldGainedDisplayed = TargetGoldGained;", "    GoldGainedDisplayed = GoldGainedDisplayed > TargetGoldGained ? (float)TargetGoldGained : GoldGainedDisplayed;"),
                                           ("    if (ExperienceGainedDisplayed > TargetExperienceGained)\n        ExperienceGainedDisplayed = TargetExperienceGained;", "    ExperienceGainedDisplayed = ExperienceGainedDisplayed > TargetExperienceGained ? (float)TargetExperienceGained : ExperienceGainedDisplayed;")])]),
 ("time", [("", "", []),
           ("tc", "const digit time", [("    float digit_count_time = ", "    const float digit_count_time = ")])]),
 ("snd", [("", "", []),
          ("so", "counting-sound tests with the value compare first", [("    if (ExperienceCountingSound && ExperienceGainedDisplayed == TargetExperienceGained) {", "    if (ExperienceGainedDisplayed == TargetExperienceGained && ExperienceCountingSound) {"),
                                                                        ("    if (GoldCountingSound && GoldGainedDisplayed == TargetGoldGained) {", "    if (GoldGainedDisplayed == TargetGoldGained && GoldCountingSound) {"),
                                                                        ("    if (RenownCountingSound && RenownGainedDisplayed == TargetRenownGained) {", "    if (RenownGainedDisplayed == TargetRenownGained && RenownCountingSound) {")])]),
 ("frame", [("", "", []),
            ("fp", "Frame++", [("    ++Frame;", "    Frame++;")])]),
]
