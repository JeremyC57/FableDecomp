#!/bin/bash
# restore.sh <addr|path>...: put the HEAD version of landed source files back (no git state change: git show + redirect)
cd $FABLE_ROOT
for a in "$@"; do
  if [ -f "$a" ]; then p=$a; else p=$(git ls-files rebuild/src/compiled | grep -i "_${a}\.cpp$" | head -1); fi
  [ -n "$p" ] && git show HEAD:$p > $p && echo "restored $p"
done
