# Object-order name predictor (2026-11-25)

Owner request: check whether the PDBs' per-object function order can name addresses that have no known target.
Tool: `tools/decomp_pipeline/naming/order_match.py` (input: `naming_status.py --dump-rec <file>`).

Method: an open retail address between known (`agree_pdb`, PDB-unique) anchors of the same Ego_r.pdb object, with the same number of
catalog functions between them as the PDB has, is predicted to be the PDB function at that index; the retail size must be within
0.67x-1.5x of the PDB code size.

Leave-one-out over 17,282 known anchors (Ego_r.pdb):

| guard | right | wrong | precision | predictions for open addresses |
|---|---:|---:|---:|---|
| 1 anchor each side, no size | 5,021 | 578 | 89.7% | 2,647 |
| 1 anchor each side + size | 4,735 | 98 | 98.0% | 2,163 (predictions_n1.tsv) |
| 2 anchors each side + size | 2,571 | 13 | 99.5% | 955 (predictions_n2.tsv) |

Cross-checks: on open split_fixable addresses the prediction equals the tracker's PDB spelling 96-98% of the time.
FableWin.pdb (debug build, more out-of-line functions) is weaker (86.9% without a size guard) and overlaps Ego_r on 291 addresses,
agreeing on 258.

Verdict: a useful ONE source (EXTRA_RULES needs two) but not the key to the ceiling. Of the 16,222 standin + split_unknown addresses
it predicts 335 (n1) / 139 (n2): known anchors are too sparse in those objects. Best use: the 615 rtti_identified and 676
split_fixable predictions confirm targets for the family waves, and the 335 ceiling predictions go to an agent with a second source.
