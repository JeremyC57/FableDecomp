#!/bin/bash
# mk.sh out.cpp leaf body.cpp [extra-models...]
OUT=$1; LEAF=$2; BODY=$3; shift 3
{ echo "// Leaf: $LEAF"; cat tu_head2.h; cat cwd_models.h; cat tu_model2.h; for f in "$@"; do cat $f; done; cat $BODY; } > $OUT
