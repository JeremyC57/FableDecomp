#!/bin/bash
# cl_watchdog.sh [max-age-seconds=300]: every 30 s kill any Wine cl.exe / link.exe older than max-age. A VC7.1 compile of one row
# takes seconds; under Wine cl.exe spins at 100% CPU for ever on some header `#error`s, and `timeout` on the Python grader kills
# the grader but not its Wine child. Three such orphans held 3 of 4 cores for hours on 2026-11-12. Logs each kill; never fails.
age=${1:-300}; log=${S:-/tmp}/tools/cl_watchdog.log
while true; do
  for p in $(ps -eo pid,etimes,comm | awk -v a=$age '($3=="cl.exe"||$3=="link.exe"||$3=="c1xx.exe"||$3=="c2.exe") && $2>a {print $1}'); do
    kill -9 $p 2>/dev/null && echo "$(date +%H:%M:%S) killed $p" >> $log
  done
  sleep 30
done
