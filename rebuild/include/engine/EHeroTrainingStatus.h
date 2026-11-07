#ifndef FABLE_EHEROTRAININGSTATUS_H
#define FABLE_EHEROTRAININGSTATUS_H
// Ego_r.pdb enum EHeroTrainingStatus: the type of CTCHeroStats::TrainingStatus (+0x114, struct_layouts_egor.tsv) and of
// CHeroDef::DefaultHeroTrainingStatus (+0x2c). Enumerators as the Codex pass 12 PDB read gives them (0x0057E2F1 hand-off); the
// Linux lane has no PDB to recheck them, so treat the spellings as single-source. Rows need the type (the mangled
// ??$Transfer@W4EHeroTrainingStatus@@@CPersistContext@@QAEXPBDAAW4EHeroTrainingStatus@@@Z, retail 0x005806C4).
enum EHeroTrainingStatus {
    TRAINING_STATUS_GRADUATED = 0,
    TRAINING_STATUS_APPRENTICE = 1,
    TRAINING_STATUS_BOY = 2
};
#endif
