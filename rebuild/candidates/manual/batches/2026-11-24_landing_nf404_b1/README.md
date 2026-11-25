# Landing nf404 b1: the CCharString / CWideString destructor conflict in 0x004AF220 and 0x0056C840

Both rows printed WARN RELOC_CONFLICT. Their objects bound `??1CCharString@@QAE@XZ` to 0x0099B510, while landed 00433530 binds it to 0x0099EAE0.

NF404 decided the conflict from the retail bodies:
- 0x0099B510 is `??1CWideString`;
- the tail at 0x0099EAE0 is `??1CCharString`.

The rows now model the layout as CQuestManager::CBoast on fable_string.h, and the conflict is gone. Both rows are RELOCATION_MATCH PASS. Their selves are unchanged, so self_symbols is untouched; the respell of the stand-in selves to ~CBoast / ~pair is queued in T3 nf404.

Gates (NF404):
- vland HEAD/new;
- pk 0 NEQ, 0 reach flips;
- /FORCE link with every pick-any owner: duplicates 0 -> 0.

Coordinator: 2/2 landed, 0 LITERAL_DIFF, T3 nf401 0x004AF220 closed, baselines OK.
