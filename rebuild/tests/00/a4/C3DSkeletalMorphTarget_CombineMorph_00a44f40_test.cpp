// Behaviour test for C3DSkeletalMorphTarget::CombineMorph 0x00A44F40 (copy of the row's logic over a small array model).
#include <stdio.h>
#include <math.h>

struct Vec { float X, Y, Z; };
struct BoneMorph { unsigned long BoneNameCRC; Vec Scaling; BoneMorph() : BoneNameCRC(0) { } };
struct Target {
    BoneMorph items[16]; unsigned long count;
    Target() : count(0) { }
    unsigned long size() const { return count; }
    void insert(unsigned long at, const BoneMorph& m) {
        for (unsigned long k = count; k > at; --k) items[k] = items[k - 1];
        items[at] = m; ++count;
    }
    void add(unsigned long crc, float x, float y, float z) {
        BoneMorph m; m.BoneNameCRC = crc; m.Scaling.X = x; m.Scaling.Y = y; m.Scaling.Z = z; items[count++] = m;
    }
    void CombineMorph(const Target& target, float weight) {
        unsigned long i = 0;
        for (unsigned long t = 0; t < target.size(); ++t) {
            const BoneMorph& source = target.items[t];
            const unsigned long crc = source.BoneNameCRC;
            while (i < size() && items[i].BoneNameCRC < crc) ++i;
            if (i < size() && items[i].BoneNameCRC == crc) {
                items[i].Scaling.X += (source.Scaling.X - 1.0f) * weight;
                items[i].Scaling.Y += (source.Scaling.Y - 1.0f) * weight;
                items[i].Scaling.Z += (source.Scaling.Z - 1.0f) * weight;
            } else {
                const BoneMorph& added = target.items[t];
                BoneMorph morph;
                morph.BoneNameCRC = added.BoneNameCRC;
                morph.Scaling.X = 1.0f; morph.Scaling.Y = 1.0f; morph.Scaling.Z = 1.0f;
                morph.Scaling.X += (added.Scaling.X - 1.0f) * weight;
                morph.Scaling.Y += (added.Scaling.Y - 1.0f) * weight;
                morph.Scaling.Z += (added.Scaling.Z - 1.0f) * weight;
                insert(i, morph);
                ++i;
            }
        }
    }
};

static int near(float a, float b) { return fabs(a - b) < 1e-5f; }

int main() {
    Target a, b;
    a.add(10, 2.0f, 1.0f, 1.0f);
    a.add(30, 1.0f, 1.0f, 1.0f);
    b.add(5, 3.0f, 1.0f, 0.0f);    // before everything: inserted at 0
    b.add(10, 1.5f, 3.0f, 1.0f);   // matches: scaling += (s-1)*w
    b.add(20, 2.0f, 2.0f, 2.0f);   // between: inserted
    b.add(40, 0.0f, 1.0f, 5.0f);   // after the end: appended
    a.CombineMorph(b, 0.5f);
    int ok = a.size() == 5
        && a.items[0].BoneNameCRC == 5  && near(a.items[0].Scaling.X, 2.0f) && near(a.items[0].Scaling.Z, 0.5f)
        && a.items[1].BoneNameCRC == 10 && near(a.items[1].Scaling.X, 2.25f) && near(a.items[1].Scaling.Y, 2.0f) && near(a.items[1].Scaling.Z, 1.0f)
        && a.items[2].BoneNameCRC == 20 && near(a.items[2].Scaling.X, 1.5f) && near(a.items[2].Scaling.Y, 1.5f)
        && a.items[3].BoneNameCRC == 30 && near(a.items[3].Scaling.X, 1.0f)
        && a.items[4].BoneNameCRC == 40 && near(a.items[4].Scaling.X, 0.5f) && near(a.items[4].Scaling.Z, 3.0f);
    Target empty; a.CombineMorph(empty, 1.0f);
    ok = ok && a.size() == 5;
    if (!ok) { printf("00a44f40_TEST FAIL\n"); return 1; }
    printf("00a44f40_TEST PASS\n");
    return 0;
}