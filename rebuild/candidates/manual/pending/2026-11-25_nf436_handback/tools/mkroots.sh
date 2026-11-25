#!/usr/bin/env bash
# mkroots.sh <name> <table.tsv|-> <hdrdir|->: build $W/work/<name> = repo symlinks, with rebuild/oracles/reloc_bindings.tsv a copy of <table>
# (or the repo table) and rebuild/include/engine overlaid by <hdrdir>/engine/*.h (copies; everything else symlinks).
set -eu
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF436; R=/home/user/FableDecomp
N=$W/work/$1; T=$2; H=$3
rm -rf $N; mkdir -p $N/rebuild/oracles $N/rebuild/include/engine
for d in docs ghidra_out refs tools; do ln -s $R/$d $N/$d; done
for f in $R/rebuild/*; do b=$(basename $f); [ $b = oracles ] || [ $b = include ] || ln -s $f $N/rebuild/$b; done
for f in $R/rebuild/oracles/*; do b=$(basename $f); [ $b = reloc_bindings.tsv ] || ln -s $f $N/rebuild/oracles/$b; done
if [ "$T" = - ]; then cp $R/rebuild/oracles/reloc_bindings.tsv $N/rebuild/oracles/; else cp $T $N/rebuild/oracles/reloc_bindings.tsv; fi
for f in $R/rebuild/include/*; do b=$(basename $f); [ $b = engine ] || ln -s $f $N/rebuild/include/$b; done
for f in $R/rebuild/include/engine/*; do ln -s $f $N/rebuild/include/engine/; done
if [ "$H" != - ]; then for f in $H/engine/*.h; do rm -f $N/rebuild/include/engine/$(basename $f); cp $f $N/rebuild/include/engine/; done; fi
echo built $N
