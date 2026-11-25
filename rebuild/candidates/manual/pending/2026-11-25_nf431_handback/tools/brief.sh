#!/usr/bin/env bash
# brief.sh <addr...>: compact dossier (symbol, decl, retail dis, FableWin body minus the /Od frame, HEAD row minus comments)
W=$S/w22/agentNF431
for a in "$@"; do sed -n '1,2p' $W/dos/$a.txt | cut -c1-170; sed -n '/^-- retail/,/^-- FableWin/p' $W/dos/$a.txt | grep -v "^-- FableWin"; echo "-- fw"; sed -n '/^## /,/^-- HEAD/p' $W/dos/$a.txt | grep -v "^## \|push ebp$\|mov ebp, esp$\|894dfc \|8be5  \|5d    \|^-- HEAD\|894df8 \|8b4dfc  *mov ecx, dword ptr \[ebp - 4\]$\|51  *push ecx$\|^02[0-9a-f]* [0-9a-f]*  *mov esp, ebp\|pop ebp$\|sub esp\|0x80 *$\|lea edi, \[ebp\|mov ecx, 0x\|mov eax, 0xcccccccc\|rep stosd\|pop ecx$" | cut -c1-200; echo "-- head"; sed -n '/^-- HEAD/,$p' $W/dos/$a.txt | grep -v "^//\|^-- HEAD\|^\s*$" | cut -c1-170; echo; done
