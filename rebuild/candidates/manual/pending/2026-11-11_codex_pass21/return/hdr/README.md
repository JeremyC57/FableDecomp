# Shared header overlay

Apply this one `hdr/` tree over the Pass21 input `include/` tree. It contains the consolidated genuine models used for every final grade. Do not overlay per-attempt private headers on top of it. The default branch remains the supplied implementation; each selected source enables its own documented retail opt-ins.

The consolidation combines independent feature additions for CParticle/CParticleSystem/CQuaternion/CMatrix3x4, CTCDExplosion, CThingGameObject, CTCHeroExperience, CBaseObject and the Oakvale quest owner. No substitute class or new packing scope was added. Existing packed-vector support was reused where originally proven.

Original reference and access corrections include CEntityScriptBase::Game/Me, CLevelScriptBase::Game/MasterScriptData, CIsThingAlive::Thing and CPredicateBase's actual base/static data. Original PDB extracts and declaration evidence are included under evidence/ and row evidence files. New classes use their actual original members/types and offset evidence.

104 existing headers passed independent default-token comparison; 49 new headers have no predecessor. Only line-number-generated static-assert identifiers were normalized. Shared prerequisites BASE_OBJECT, DEF_CLASS_BASE, NSCRIPT and SUSPENDABLE_PROCESS were enabled equally in both inputs where required. Optional row features were disabled. See evidence/SHARED_HEADER_DEFAULT_AUDIT.json.

The final per-row compiler audits record the compiled header tree fingerprint. README documentation added afterward is not compiler input; HEADER_SHA256SUMS records the exact final .h files. SHA256SUMS at the archive root covers all returned files.
