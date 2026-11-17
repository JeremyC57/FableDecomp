# NF56 notes (in progress; rewritten at hand-back)
- State lives in STATUS.tsv / APPLIED (work/entry_notes.tsv) / work/renames1.tsv / work/name_fixes.tsv; tools in work/.
- vland: work/vl.sh <tag> <rows...> (funclets of work/funclets.txt get the include repointed to rows/<parent>.cpp). Logs work/vl_*.log.
- 00a0aa80 / 00b3b8c0: landed tests drove the old stand-ins (LINK_FAIL); rewritten as model tests rows/<addr>_test.cpp, PassPattern kept.
