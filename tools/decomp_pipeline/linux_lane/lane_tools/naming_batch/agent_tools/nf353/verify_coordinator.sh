cd /home/user/FableDecomp; source $S/wine/env.sh; export S; unset TMPDIR TMP; T=tools/decomp_pipeline/linux_lane/lane_tools; A=$S/w22/agentNF353; W=$S/v353; rm -rf $W; mkdir -p $W/rc $W/pk
addrs=$(for b in b1 b2 b3; do tail -n +2 $A/batches/$b/RESULTS.tsv | cut -f1; done)
python3 $T/recheck.py $W/rc $addrs >/dev/null
env LANDVERIFY_WORK=$W/wd timeout 5000 python3 -u $T/vland.py $W/rc/payload.json $W/rc/oracle.tsv --recheck-landed > $W/grade.log 2>&1
X="$S/dl/game/Fable The Lost Chapters/Fable.exe"
for b in b1 b2 b3; do for a in $(tail -n +2 $A/batches/$b/RESULTS.tsv | cut -f1); do
  f=$(ls rebuild/src/compiled/${a:0:2}/${a:2:2}/*_$a.cpp); leaf=$(grep -m1 '^// Leaf:' $f | sed 's#^// Leaf: *##; s/ .*//'); len=$(awk -F'\t' -v a=$a '$1""==a""{print $3; exit}' rebuild/oracles/auto-re-candidates.tsv)
  o=$W/wd/${a:0:2}/${a:2:2}/$a/$a.obj
  FABLE_EXE="$X" FABLE_ROOT=$PWD python3 $T/pk_all/pk_all.py $o 0x$a --row-length=$len --leaf="$leaf" > $W/pk/N_$a.pkall.tsv 2>&1
  cp $A/batches/$b/evidence/pk_gate/H_$a.pkall.tsv $W/pk/H_$a.pkall.tsv
  python3 -I rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py "$X" rebuild/oracles/reloc_bindings.tsv $o $a > $W/pk/N_$a.pvr.txt 2>&1
done; done
echo V353_DONE
