#!/usr/bin/env python3
"""skel.py <obj|retail> -- print a compact block skeleton of CAIStateGroup_BuyFromSeller::Update.
   skel.py retail            : from the retail bytes (chunk572.tsv)
   skel.py <path.obj>        : from a built object (objdump -dr)
Each line: offset  label:  instr  (calls named, branches -> label)."""
import sys, os, re, subprocess, csv
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ["S"]
NAMES = {0xA01B10:"IPctor",0xA01C10:"IPdtor",0xA01B50:"GetPItem",0xA01B90:"SetPItem",0x4C73D0:"GetPos",0x40F020:"LowerBound",
 0x7EE540:"GetPVillage",0x658B20:"IsWorkTime",0x97E890:"MoveToPosProx",0x8FCF40:"Terminate",0x798F50:"GetShopTC",0xBFEA0E:"malloc",
 0xBFEA14:"free",0x4BA4E0:"push_back",0x4B8050:"clear",0x8FCFD0:"GetPWorldSeed",0x4CDAE0:"GFRandom",0x7F4F80:"GetBuyersPhrase",
 0x8FCFC0:"GetWorld",0x687540:"AddMessage",0x493B5B:"RemoveMessage",0x9812C0:"Speak",0x49D870:"GetFrame",0x919A70:"GetSellersReply",
 0x97E200:"DoNothing",0x6629B0:"RemoveFromCI"}
def short(sym):
    for k in ["IPctor:??0CBaseIntelligentPointer","IPdtor:??1CBaseIntelligentPointer","GetPItem:?GetPItem@CBaseIntelligentPointer","SetPItem:?SetPItem",
              "GetPos:?GetPos@CThing","LowerBound:?LowerBound","GetPVillage:?GetPVillage","IsWorkTime:?IsWorkTime","MoveToPosProx:?AIAction_MoveToPosProximity",
              "Terminate:?Terminate","GetShopTC:?GetShopTC","malloc:_malloc","free:_free","push_back:?push_back","clear:?clear","GetPWorldSeed:?GetPWorldSeed",
              "GFRandom:?GFRandom","GetBuyersPhrase:?GetBuyersPhrase","GetWorld:?GetWorld","AddMessage:?AddMessage","RemoveMessage:?RemoveMessage",
              "Speak:?AIAction_SpeakAISpeechToTarget","GetFrame:?GetFrame","GetSellersReply:?GetSellersReply","DoNothing:?AIAction_DoNothing","RemoveFromCI:?RemoveFromCreatureInteraction"]:
        n, p = k.split(":",1)
        if sym.startswith(p): return n
    return sym[:30]
def main():
    src = sys.argv[1]
    insns = []   # (off, mnemonic, opstr, callname)
    if src == "retail":
        for r in csv.DictReader(open(f"{S}/w22/chunk572.tsv", encoding="utf-8-sig"), delimiter="\t"):
            if r["address"] == "00919ae0": b = bytes.fromhex(r["bytes"])
        md = Cs(CS_ARCH_X86, CS_MODE_32)
        for i in md.disasm(b, 0x919ae0):
            name = None
            if i.mnemonic == "call":
                t = int(i.op_str, 16); name = NAMES.get(t, hex(t))
            insns.append((i.address-0x919ae0, i.mnemonic, i.op_str, name))
    else:
        out = subprocess.run(["objdump","-dr","-M","intel","--no-show-raw-insn",src], capture_output=True, text=True).stdout
        fn = sys.argv[2] if len(sys.argv) > 2 else "?Update@CAIStateGroup_BuyFromSeller@@UAEXXZ"
        sec = re.search(r"<" + re.escape(fn) + r"[^>]*>:\n(.*?)(?:\n\n|\Z)", out, re.S).group(1)
        lines = sec.splitlines(); i = 0
        while i < len(lines):
            m = re.match(r"\s*([0-9a-f]+):\s+(\S+)\s*(.*)", lines[i])
            if m:
                off = int(m.group(1),16); mn = m.group(2); op = m.group(3).strip(); name = None
                if i+1 < len(lines) and re.match(r"\s*[0-9a-f]+:\s+(DISP32|dir32)\s+", lines[i+1]):
                    rel = re.match(r"\s*[0-9a-f]+:\s+(?:DISP32|dir32)\s+(\S+)", lines[i+1]).group(1)
                    if mn == "call": name = short(rel)
                    i += 1
                insns.append((off, mn, op, name))
            i += 1
    # branch targets
    targets = {}
    for off, mn, op, name in insns:
        if mn.startswith("j"):
            m = re.match(r"(?:0x)?([0-9a-f]+)", op)
            if m:
                t = int(m.group(1),16) - (0x919ae0 if src == "retail" else 0)
                if t not in targets: targets[t] = "L%d" % len(targets)
    for off, mn, op, name in insns:
        lab = (targets[off] + ":") if off in targets else ""
        if mn == "call": txt = "call " + (name or op)
        elif mn.startswith("j"):
            m = re.match(r"(?:0x)?([0-9a-f]+)", op); txt = mn + " -> " + (targets.get(int(m.group(1),16) - (0x919ae0 if src == "retail" else 0), op) if m else op)
        elif mn in ("ret",): txt = "ret"
        elif lab: txt = "(" + mn + " " + op[:20] + ")"
        else: continue
        print("%5x %-5s %s" % (off, lab, txt))
main()
