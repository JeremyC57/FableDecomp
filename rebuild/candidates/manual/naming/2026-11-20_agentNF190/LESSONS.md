# NF190 lessons (one line each)
- A row-local `void Construct();` stand-in for an out-of-line ctor respells in place to the ctor declaration; drop the explicit call (the declaration runs it at the same point). Defined-symbol sets stay equal.
- VC7.1 emits every referenced inline function as a COMDAT even when it is expanded everywhere, so a K050 vtable emitter (inline ctor + new) always brings the base-ctor chain: run the pick-any comparison before celebrating a ??_G match.
- For template instances (implicit or explicit), `#pragma inline_depth` takes the state at the end of the TU: an explicit instantiation placed before the pragma was still compiled at the final depth (0x00DFDE30). Pragma regions inside an ordinary function body still act on their own call sites (0x00D043A0).
- Build a Leaf-line index of all landed rows (tools/leafidx.py) to find owners of COMDAT copies that have no reloc_bindings line.
- When porting script rows, carry every row-local inline body (`inline long CTimer::GetTime`), not just those starting `inline CTimer`.
- A CQ_/CV_ quest-script row on a local struct ports onto its class header in one form with NSCRIPT + CLEVELSCRIPTBASE + BASE_OBJECT + CCHARSTRING and CQ_SunnyvaleMasterData.h; wide literals need FABLE_RETAIL_CWIDESTRING (implicit ctor) and the `(const unsigned short*)` casts dropped.
- gr.sh writes work/last_<addr>.txt for both the HEAD control and the form: grade the form last when you need its compiler error.
