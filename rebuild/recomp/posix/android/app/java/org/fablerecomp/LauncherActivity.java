package org.fablerecomp;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.Settings;
import android.text.InputType;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.List;
import java.util.Locale;

/** Settings screen: game folder, UI scale, touch controls and the Vulkan driver; then starts the game. */
public class LauncherActivity extends Activity {
    static final String PREFS = "launcher";
    private static final int PICK_FOLDER = 1, PICK_DRIVER = 2, PERMISSION = 3;

    private SharedPreferences prefs;
    private EditText folder;
    private TextView scaleLabel, status;
    private SeekBar scale;
    private Switch touch;
    private Spinner driver, dxvk;
    private static final String[] DXVK_IDS = {"auto", "3", "2"};
    private List<String> drivers;

    @Override
    protected void onCreate(Bundle saved) {
        super.onCreate(saved);
        prefs = getSharedPreferences(PREFS, MODE_PRIVATE);

        LinearLayout col = new LinearLayout(this);
        col.setOrientation(LinearLayout.VERTICAL);
        int pad = dp(20);
        col.setPadding(pad, pad, pad, pad);

        TextView title = new TextView(this);
        title.setText("Fable: The Lost Chapters");
        title.setTextSize(24);
        title.setTypeface(Typeface.DEFAULT_BOLD);
        col.addView(title);

        col.addView(heading("Game folder (contains Fable.exe)"));
        folder = new EditText(this);
        folder.setSingleLine(true);
        folder.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_URI);
        folder.setText(prefs.getString("gameDir", defaultGameDir()));
        col.addView(folder);
        Button browse = new Button(this);
        browse.setText("Choose folder…");
        browse.setOnClickListener(v -> {
            Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
            startActivityForResult(i, PICK_FOLDER);
        });
        col.addView(browse);

        scaleLabel = heading("");
        col.addView(scaleLabel);
        scale = new SeekBar(this);
        scale.setMax(8);  // 1.0 .. 3.0 in steps of 0.25
        scale.setProgress(Math.round((prefs.getFloat("uiScale", 1.5f) - 1f) * 4f));
        scale.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar s, int p, boolean user) { updateScaleLabel(); }
            public void onStartTrackingTouch(SeekBar s) {}
            public void onStopTrackingTouch(SeekBar s) {}
        });
        col.addView(scale);
        updateScaleLabel();

        touch = new Switch(this);
        touch.setText("On-screen touch controls (can also be toggled in game)");
        touch.setChecked(prefs.getBoolean("touch", true));
        touch.setPadding(0, dp(16), 0, 0);
        col.addView(touch);

        col.addView(heading("Vulkan driver"));
        driver = new Spinner(this);
        col.addView(driver);
        LinearLayout row = new LinearLayout(this);
        Button add = new Button(this);
        add.setText("Install driver (.zip)…");
        add.setOnClickListener(v -> {
            Intent i = new Intent(Intent.ACTION_OPEN_DOCUMENT);
            i.addCategory(Intent.CATEGORY_OPENABLE);
            i.setType("*/*");
            i.putExtra(Intent.EXTRA_MIME_TYPES, new String[] {"application/zip", "application/x-zip-compressed", "application/octet-stream"});
            startActivityForResult(i, PICK_DRIVER);
        });
        row.addView(add);
        Button remove = new Button(this);
        remove.setText("Remove");
        remove.setOnClickListener(v -> {
            int pos = driver.getSelectedItemPosition();
            if (pos <= 0) return;
            Drivers.remove(this, drivers.get(pos));
            refreshDrivers("");
        });
        row.addView(remove);
        col.addView(row);
        refreshDrivers(prefs.getString("driver", ""));
        TextView hint = new TextView(this);
        hint.setText("Snapdragon (Adreno) phones: Qualcomm's own driver lacks Vulkan features the game's renderer (DXVK) "
            + "needs. If the game shows a driver error or a black screen, install a Mesa Turnip driver .zip "
            + "(for example from github.com/K11MCH1/AdrenoToolsDrivers/releases) and select it here.");
        hint.setTextSize(12);
        col.addView(hint);

        col.addView(heading("Renderer"));
        dxvk = new Spinner(this);
        ArrayAdapter<String> dx = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item,
            new String[] {"Automatic", "DXVK 3.x (newer drivers, e.g. Turnip)", "DXVK 2.6 (drivers without shaderInt64)"});
        dx.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        dxvk.setAdapter(dx);
        dxvk.setSelection(Math.max(0, java.util.Arrays.asList(DXVK_IDS).indexOf(prefs.getString("dxvk", "auto"))));
        col.addView(dxvk);

        Button start = new Button(this);
        start.setText("Start game");
        start.setTextSize(20);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.topMargin = dp(24);
        start.setLayoutParams(lp);
        start.setOnClickListener(v -> start());
        col.addView(start);

        status = new TextView(this);
        status.setPadding(0, dp(12), 0, 0);
        status.setText("Data, saves and FableRecomp.log: " + dataDir(this).getAbsolutePath());
        col.addView(status);

        ScrollView sv = new ScrollView(this);
        sv.addView(col);
        setContentView(sv);
    }

    static File dataDir(Activity a) {
        File ext = a.getExternalFilesDir(null);
        return ext != null ? ext : new File(a.getFilesDir(), "data");
    }

    private String defaultGameDir() {
        return new File(Environment.getExternalStorageDirectory(), "Fable The Lost Chapters").getAbsolutePath();
    }

    private TextView heading(String s) {
        TextView t = new TextView(this);
        t.setText(s);
        t.setTypeface(Typeface.DEFAULT_BOLD);
        t.setPadding(0, dp(16), 0, dp(4));
        return t;
    }

    private int dp(int v) { return Math.round(v * getResources().getDisplayMetrics().density); }

    private float uiScale() { return 1f + scale.getProgress() / 4f; }

    private void updateScaleLabel() {
        scaleLabel.setText(String.format(Locale.US, "Interface scale: %.2f×", uiScale()));
    }

    private void refreshDrivers(String select) {
        drivers = Drivers.list(this);
        drivers.add(0, "");
        String[] names = new String[drivers.size()];
        for (int i = 0; i < names.length; ++i) names[i] = i == 0 ? "System driver (default)" : drivers.get(i);
        ArrayAdapter<String> ad = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, names);
        ad.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        driver.setAdapter(ad);
        int i = drivers.indexOf(select);
        driver.setSelection(Math.max(i, 0));
    }

    private boolean hasStorageAccess() {
        if (Build.VERSION.SDK_INT >= 30) return Environment.isExternalStorageManager();
        return checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
            && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
    }

    private void requestStorageAccess() {
        if (Build.VERSION.SDK_INT >= 30) {
            new AlertDialog.Builder(this)
                .setTitle("Storage access")
                .setMessage("The game reads its files from the folder you chose. Allow \"All files access\" on the next screen, then come back and press Start.")
                .setPositiveButton("Open settings", (d, w) -> {
                    try {
                        startActivity(new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION, Uri.parse("package:" + getPackageName())));
                    } catch (Exception e) {
                        startActivity(new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION));
                    }
                })
                .setNegativeButton("Cancel", null)
                .show();
        } else {
            requestPermissions(new String[] {Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION);
        }
    }

    private void save() {
        prefs.edit()
            .putString("gameDir", folder.getText().toString().trim())
            .putFloat("uiScale", uiScale())
            .putBoolean("touch", touch.isChecked())
            .putString("driver", drivers.get(Math.max(driver.getSelectedItemPosition(), 0)))
            .putString("dxvk", DXVK_IDS[Math.max(dxvk.getSelectedItemPosition(), 0)])
            .apply();
    }

    @Override
    protected void onPause() {
        super.onPause();
        save();
    }

    private void start() {
        save();
        String dir = folder.getText().toString().trim();
        if (!hasStorageAccess() && !dir.startsWith(dataDir(this).getAbsolutePath())) {
            requestStorageAccess();
            return;
        }
        if (findFile(new File(dir), "Fable.exe") == null) {
            new AlertDialog.Builder(this)
                .setTitle("Game files not found")
                .setMessage("Fable.exe is not in\n" + dir + "\n\nCopy the PC (Steam/GOG/retail) installation folder of Fable: The Lost Chapters to your device and choose it here.")
                .setPositiveButton("OK", null)
                .show();
            return;
        }
        Intent i = new Intent(this, GameActivity.class);
        i.putExtra("gameDir", dir);
        i.putExtra("uiScale", uiScale());
        i.putExtra("touch", touch.isChecked());
        i.putExtra("driver", drivers.get(Math.max(driver.getSelectedItemPosition(), 0)));
        i.putExtra("dxvk", DXVK_IDS[Math.max(dxvk.getSelectedItemPosition(), 0)]);
        startActivity(i);
    }

    static File findFile(File dir, String name) {
        String[] list = dir.list();
        if (list == null) return null;
        for (String s : list)
            if (s.equalsIgnoreCase(name)) return new File(dir, s);
        return null;
    }

    @Override
    protected void onActivityResult(int request, int result, Intent data) {
        super.onActivityResult(request, result, data);
        if (result != RESULT_OK || data == null || data.getData() == null) return;
        Uri uri = data.getData();
        if (request == PICK_FOLDER) {
            String path = treeToPath(uri);
            if (path == null) {
                Toast.makeText(this, "That location has no file path; pick a folder on the device storage or an SD card.", Toast.LENGTH_LONG).show();
                return;
            }
            folder.setText(path);
            save();
            if (!hasStorageAccess()) requestStorageAccess();
        } else if (request == PICK_DRIVER) {
            try {
                String name = Drivers.install(this, uri);
                refreshDrivers(name);
                Toast.makeText(this, "Installed driver " + name, Toast.LENGTH_SHORT).show();
            } catch (Exception e) {
                new AlertDialog.Builder(this).setTitle("Driver not installed").setMessage(String.valueOf(e.getMessage())).setPositiveButton("OK", null).show();
            }
        }
    }

    /** content://…/tree/primary:Games/Fable -> /storage/emulated/0/Games/Fable (null when not a local volume). */
    static String treeToPath(Uri uri) {
        if (!"com.android.externalstorage.documents".equals(uri.getAuthority())) return null;
        String id = DocumentsContract.getTreeDocumentId(uri);
        int colon = id.indexOf(':');
        if (colon < 0) return null;
        String volume = id.substring(0, colon), rel = id.substring(colon + 1);
        String root = "primary".equalsIgnoreCase(volume) ? Environment.getExternalStorageDirectory().getAbsolutePath() : "/storage/" + volume;
        return rel.isEmpty() ? root : root + "/" + rel;
    }
}
